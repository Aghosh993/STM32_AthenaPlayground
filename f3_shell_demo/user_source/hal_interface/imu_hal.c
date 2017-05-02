#include "imu_hal.h"
#if defined USE_STDIO_DEBUG
	#include <stdio.h>
#endif

/*
	IMU Hardware abstraction layer for the STM32CubeF3 FW library, in conjunction with the STM32F303VCT6 MCU on a Discovery F3 board.
	Currently only Polling I/O is supported, with timeouts to monitor errors and attempt to correct them.

	(c) Abhimanyu Ghosh, 2017

	Available configuration symbols (use "-D[SYMBOL]" in build system to enable, with [SYMBOL] replaced by the following symbols):
		- USE_STDIO_DEBUG - Enables status/error print statements over STDIO
 */

static volatile I2C_HandleTypeDef imu_i2c_bus;
static volatile SPI_HandleTypeDef imu_spi_bus;

static void i2c_setup(i2cbaud b)
{
	imu_i2c_bus.Instance = 				I2C1;
	imu_i2c_bus.Init.OwnAddress1 = 		0x32;
	imu_i2c_bus.Init.AddressingMode = 	I2C_ADDRESSINGMODE_7BIT;
	imu_i2c_bus.Init.GeneralCallMode = 	I2C_GENERALCALL_DISABLE;
	imu_i2c_bus.Init.NoStretchMode = 	I2C_NOSTRETCH_DISABLE;
	imu_i2c_bus.Mode = 					HAL_I2C_MODE_MASTER;

	switch(b)
	{
		case BAUD_100KHZ:
			imu_i2c_bus.Init.Timing = 0x10321111; // Experimentally-verified SCL of around 97 kHz at System CLK of 72 MHz
			break;
		case BAUD_400KHZ:
			imu_i2c_bus.Init.Timing = 0x00320202; // Experimentally-verified SCL of around 398 kHz at System CLK of 72 MHz
			break;
	}

	if(HAL_I2C_Init(&imu_i2c_bus) != HAL_OK)
	{
		#if defined USE_STDIO_DEBUG
			printf("Error in I2C bus init!!\r\n");
		#endif
		imu_i2c_bus_clear();
	}
}

static void spi_setup(spibaud b)
{
	imu_spi_bus.Instance = 				SPI1;
	imu_spi_bus.Init.Mode = 			SPI_MODE_MASTER;
	imu_spi_bus.Init.Direction = 		SPI_DIRECTION_2LINES;
	imu_spi_bus.Init.DataSize = 		SPI_DATASIZE_8BIT;
	imu_spi_bus.Init.CLKPolarity = 		SPI_POLARITY_LOW;
	imu_spi_bus.Init.CLKPhase = 		SPI_PHASE_1EDGE;
	imu_spi_bus.Init.NSS = 				SPI_NSS_SOFT;
	imu_spi_bus.Init.FirstBit = 		SPI_FIRSTBIT_MSB;
	imu_spi_bus.Init.TIMode = 			SPI_TIMODE_DISABLED;
	imu_spi_bus.Init.CRCCalculation = 	SPI_CRCCALCULATION_DISABLED;
	imu_spi_bus.Init.CRCPolynomial = 	7;

	switch(b)
	{
		case BAUD_1M:
			imu_spi_bus.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
			break;
		case BAUD_2M:
			imu_spi_bus.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
			break;
		case BAUD_5M:
			imu_spi_bus.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
			break;
	}
	HAL_SPI_Init(&imu_spi_bus);
}

static int spi_write_to_l3gd20_register(uint8_t reg, uint8_t data)
{
	/*
		Select L3GD20 for SPI transaction:
	 */
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);

	/*
		Send register and data contents:
	 */

	uint8_t tx_data[2];
	tx_data[0] = reg;
	tx_data[1] = data;

	uint8_t rx_data[2];

	if(HAL_SPI_TransmitReceive(&imu_spi_bus, tx_data, rx_data, 2U, 100U) != HAL_OK)
	{
		return -1;
	}

	/*
		De-select the L3GD20 for SPI transaction:
	 */

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);
	return 0;
}

/*
	Function to read a sequence of bytes from l3gd20.
 */
static int spi_read_from_l3gd20_register(uint8_t start_reg, uint8_t len, uint8_t *data_buffer)
{
	uint8_t dummy_byte = 0U;
	uint8_t i = 0U;

	/*
		Select L3GD20 for SPI transaction:
	 */
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);

	/*
		Send register we wish to start read sequence from, logical
		OR with masks to enable address auto-increment and read mode:
	 */
	uint8_t receive_cmd = start_reg | L3GD20_GYRO_READ_MASK;

	if(len > 1)
	{
		receive_cmd |= L3GD20_GYRO_SEQUENTIAL_RW_MASK;
	}
	
	if(HAL_SPI_TransmitReceive(&imu_spi_bus, &receive_cmd, &dummy_byte, 1U, 100U) != HAL_OK)
	{
		return -1;
	}

	/*
		Read data:
	 */
	for(i=0U; i<len; ++i)
	{
		/*
			Send zeroes and keep clocking data in until we fill the data buffer:
		 */
		if(HAL_SPI_TransmitReceive(&imu_spi_bus, (uint8_t *)&dummy_byte, (uint8_t *)&data_buffer[i], 1U, 100U) != HAL_OK)
		{
			return -3;
		}
	}

	/*
		De-select the L3GD20 for SPI transaction:
	 */
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);

	return 0;
}

void imu_i2c_bus_clear(void)
{

}

int initialize_imu(ACC_SCALE a, GYRO_SCALE g, MAG_SCALE m, imu_scaled_data_struct* buf)
{
	buf->acc_meas_scale = a;
	buf->gyro_meas_scale = g;
	buf->mag_meas_scale = m;
	
	int setup_error = 0;

	i2c_setup(BAUD_400KHZ);
	spi_setup(BAUD_5M);

	uint8_t acc_ctrl_reg1_mask = LSM303_ACC_XEN | LSM303_ACC_YEN | LSM303_ACC_ZEN |
									LSM303_ACC_ODR_1344HZ_MASK;

	uint8_t acc_ctrl_reg4_mask = LSM303_ACC_HIGHRES_MODE_MASK;

	switch(a)
	{
		case SCALE_2G:
			acc_ctrl_reg4_mask |= LSM303_ACC_2G_MASK;
			break;
		case SCALE_4G:
			acc_ctrl_reg4_mask |= LSM303_ACC_4G_MASK;
			break;
		case SCALE_8G:
			acc_ctrl_reg4_mask |= LSM303_ACC_8G_MASK;
			break;
		case SCALE_16G:
			acc_ctrl_reg4_mask |= LSM303_ACC_16G_MASK;
			break;
	}

	uint8_t mag_cra_mask = LSM303_MAG_DO_220HZ_MASK;
	
	uint8_t mag_crb_mask = 0U;

	switch(m)
	{
		case SCALE_1POINT3_GAUSS:
			mag_crb_mask |= LSM303_MAG_1POINT3_GAUSS_MASK;
			break;
		case SCALE_1POINT9_GAUSS:
			mag_crb_mask |= LSM303_MAG_1POINT9_GAUSS_MASK;
			break;
		case SCALE_2POINT5_GAUSS:
			mag_crb_mask |= LSM303_MAG_2POINT5_GAUSS_MASK;
			break;
		case SCALE_4POINT0_GAUSS:
			mag_crb_mask |= LSM303_MAG_4POINT0_GAUSS_MASK;
			break;
		case SCALE_4POINT7_GAUSS:
			mag_crb_mask |= LSM303_MAG_4POINT7_GAUSS_MASK;
			break;
		case SCALE_5POINT6_GAUSS:
			mag_crb_mask |= LSM303_MAG_5POINT6_GAUSS_MASK;
			break;
		case SCALE_8POINT1_GAUSS:
			mag_crb_mask |= LSM303_MAG_8POINT1_GAUSS_MASK;
			break;
	}

	uint8_t mag_mr_mask = LSM303_MAG_CONTINUOUS_CONV_MASK;

	/*
		Write initialization data to accelerometer:
	 */

	if(HAL_I2C_Mem_Write(&imu_i2c_bus, ADDR_LSM303DLHC_ACC << 1, LSM303_CTRL_REG1_A, I2C_MEMADD_SIZE_8BIT, &acc_ctrl_reg1_mask, 1U, 100U) != HAL_OK)
	{
		setup_error = 1;
	}

	if(HAL_I2C_Mem_Write(&imu_i2c_bus, ADDR_LSM303DLHC_ACC << 1, LSM303_CTRL_REG4_A, I2C_MEMADD_SIZE_8BIT, &acc_ctrl_reg4_mask, 1U, 100U) != HAL_OK)
	{
		setup_error = 2;
	}

	/*
		Write initialization data to magnetometer:
	 */

	if(HAL_I2C_Mem_Write(&imu_i2c_bus, ADDR_LSM303DLHC_MAG << 1, LSM303_CRA_REG_M, I2C_MEMADD_SIZE_8BIT, &mag_cra_mask, 1U, 100U) != HAL_OK)
	{
		setup_error = 3;
	}

	if(HAL_I2C_Mem_Write(&imu_i2c_bus, ADDR_LSM303DLHC_MAG << 1, LSM303_CRB_REG_M, I2C_MEMADD_SIZE_8BIT, &mag_crb_mask, 1U, 100U) != HAL_OK)
	{
		setup_error = 4;
	}

	if(HAL_I2C_Mem_Write(&imu_i2c_bus, ADDR_LSM303DLHC_MAG << 1, LSM303_MR_REG_M, I2C_MEMADD_SIZE_8BIT, &mag_mr_mask, 1U, 100U) != HAL_OK)
	{
		setup_error = 5;
	}

	/*
		Gyro intialization:
	 */

	uint8_t gyro_ctrl_reg1_mask = L3GD20_GYRO_XEN | L3GD20_GYRO_YEN | L3GD20_GYRO_ZEN | LSGD20_PD_NORMAL_MODE;

	uint8_t gyro_ctrl_reg4_mask = 0U;

	switch(g)
	{
		case SCALE_250_DPS:
			gyro_ctrl_reg4_mask = L3GD20_GYRO_250DPS_MASK;
			break;
		case SCALE_500_DPS:
			gyro_ctrl_reg4_mask = L3GD20_GYRO_500DPS_MASK;
			break;
		case SCALE_2000_DPS:
			gyro_ctrl_reg4_mask = L3GD20_GYRO_2000DPS_MASK;
			break;
	}

	if(spi_write_to_l3gd20_register(L3GD20_GYRO_CTRL_REG1, gyro_ctrl_reg1_mask) < 0)
	{
		setup_error = 6;
		#if defined USE_STDIO_DEBUG
			printf("Error setting CTRL Reg 1 on L3GD20 via SPI!!\r\n");
		#endif
	}

	if(spi_write_to_l3gd20_register(L3GD20_GYRO_CTRL_REG4, gyro_ctrl_reg4_mask) < 0)
	{
		setup_error = 7;
		#if defined USE_STDIO_DEBUG
			printf("Error setting CTRL Reg 4 on L3GD20 via SPI!!\r\n");
		#endif
	}

	if(setup_error)
	{
		#if defined USE_STDIO_DEBUG
			printf("ERROR %d\r\n", setup_error);
		#endif
		imu_i2c_bus_clear();
	}

	return setup_error;
}

int get_raw_imu_data(imu_raw_data_struct* buffer)
{
	uint8_t accelerometer_buffer[6];
	uint8_t magnetometer_buffer[6];
	uint8_t gyro_buffer[6];

	union {
		uint8_t input[2];
		int16_t output;
	} u8_to_i16;

	if(HAL_I2C_Mem_Read(&imu_i2c_bus, ADDR_LSM303DLHC_ACC << 1, LSM303_ACC_OUT_X_L | 0x80, I2C_MEMADD_SIZE_8BIT, accelerometer_buffer, 6U, 100U) != HAL_OK)
	{
		imu_i2c_bus_clear();
		return -1;
		#if defined USE_STDIO_DEBUG
			printf("Accelerometer acquisition error!!\r\n");
		#endif
	}
	else
	{
		u8_to_i16.input[0] = accelerometer_buffer[0];
		u8_to_i16.input[1] = accelerometer_buffer[1];

		buffer->accel_data[0] = u8_to_i16.output;

		u8_to_i16.input[0] = accelerometer_buffer[2];
		u8_to_i16.input[1] = accelerometer_buffer[3];

		buffer->accel_data[1] = u8_to_i16.output;

		u8_to_i16.input[0] = accelerometer_buffer[4];
		u8_to_i16.input[1] = accelerometer_buffer[5];

		buffer->accel_data[2] = u8_to_i16.output;
	}

	if(HAL_I2C_Mem_Read(&imu_i2c_bus, ADDR_LSM303DLHC_MAG << 1, LSM303_MAG_OUT_X_H, I2C_MEMADD_SIZE_8BIT, magnetometer_buffer, 6U, 100U) != HAL_OK)
	{
		imu_i2c_bus_clear();
		return -1;
		#if defined USE_STDIO_DEBUG
			printf("Magnetometer acquisition error!!\r\n");
		#endif
	}
	else
	{
		u8_to_i16.input[0] = magnetometer_buffer[0];
		u8_to_i16.input[1] = magnetometer_buffer[1];

		buffer->magnetometer_data[0] = u8_to_i16.output;

		u8_to_i16.input[0] = magnetometer_buffer[2];
		u8_to_i16.input[1] = magnetometer_buffer[3];

		buffer->magnetometer_data[1] = u8_to_i16.output;

		u8_to_i16.input[0] = magnetometer_buffer[4];
		u8_to_i16.input[1] = magnetometer_buffer[5];

		buffer->magnetometer_data[2] = u8_to_i16.output;
	}

	if(spi_read_from_l3gd20_register(L3GD20_GYRO_OUT_X_L, 6, gyro_buffer) < 0)
	{
		return -1;
		#if defined USE_STDIO_DEBUG
			printf("Gyro acquisition error!!\r\n");
		#endif
	}
	else
	{
		u8_to_i16.input[0] = gyro_buffer[0];
		u8_to_i16.input[1] = gyro_buffer[1];

		buffer->gyro_data[0] = u8_to_i16.output;

		u8_to_i16.input[0] = gyro_buffer[2];
		u8_to_i16.input[1] = gyro_buffer[3];

		buffer->gyro_data[1] = u8_to_i16.output;

		u8_to_i16.input[0] = gyro_buffer[4];
		u8_to_i16.input[1] = gyro_buffer[5];

		buffer->gyro_data[2] = u8_to_i16.output;
	}

	return 0;
}