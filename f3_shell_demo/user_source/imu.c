#include "imu.h"

int get_scaled_imu_data(imu_scaled_data_struct* buffer)
{
	imu_raw_data_struct data_buf;
	int read_res = get_raw_imu_data(&data_buf);

	if(read_res >= 0)
	{
		switch(buffer->acc_meas_scale)
		{
			case SCALE_2G:
				buffer->accel_data[0] = (float)data_buf.accel_data[ACCEL_AXIS_X] * (float)ACC_SCALE_2G;
				buffer->accel_data[1] = (float)data_buf.accel_data[ACCEL_AXIS_Y] * (float)ACC_SCALE_2G;
				buffer->accel_data[2] = (float)data_buf.accel_data[ACCEL_AXIS_Z] * (float)ACC_SCALE_2G;
				break;
			case SCALE_4G:
				buffer->accel_data[0] = (float)data_buf.accel_data[ACCEL_AXIS_X] * (float)ACC_SCALE_4G;
				buffer->accel_data[1] = (float)data_buf.accel_data[ACCEL_AXIS_Y] * (float)ACC_SCALE_4G;
				buffer->accel_data[2] = (float)data_buf.accel_data[ACCEL_AXIS_Z] * (float)ACC_SCALE_4G;
				break;
			case SCALE_8G:
				buffer->accel_data[0] = (float)data_buf.accel_data[ACCEL_AXIS_X] * (float)ACC_SCALE_8G;
				buffer->accel_data[1] = (float)data_buf.accel_data[ACCEL_AXIS_Y] * (float)ACC_SCALE_8G;
				buffer->accel_data[2] = (float)data_buf.accel_data[ACCEL_AXIS_Z] * (float)ACC_SCALE_8G;
				break;
			case SCALE_16G:
				buffer->accel_data[0] = (float)data_buf.accel_data[ACCEL_AXIS_X] * (float)ACC_SCALE_16G;
				buffer->accel_data[1] = (float)data_buf.accel_data[ACCEL_AXIS_Y] * (float)ACC_SCALE_16G;
				buffer->accel_data[2] = (float)data_buf.accel_data[ACCEL_AXIS_Z] * (float)ACC_SCALE_16G;
				break;
			default:
				break;
		}

		switch(buffer->gyro_meas_scale)
		{
			case SCALE_250_DPS:
				buffer->gyro_data[0] = (float)data_buf.gyro_data[AXIS_ROLL] * (float)GYRO_SCALE_250_DPS;
				buffer->gyro_data[1] = (float)data_buf.gyro_data[AXIS_PITCH] * (float)GYRO_SCALE_250_DPS;
				buffer->gyro_data[2] = (float)data_buf.gyro_data[AXIS_YAW] * (float)GYRO_SCALE_250_DPS;
				break;
			case SCALE_500_DPS:
				buffer->gyro_data[0] = (float)data_buf.gyro_data[AXIS_ROLL] * (float)GYRO_SCALE_500_DPS;
				buffer->gyro_data[1] = (float)data_buf.gyro_data[AXIS_PITCH] * (float)GYRO_SCALE_500_DPS;
				buffer->gyro_data[2] = (float)data_buf.gyro_data[AXIS_YAW] * (float)GYRO_SCALE_500_DPS;
				break;
			case SCALE_2000_DPS:
				buffer->gyro_data[0] = (float)data_buf.gyro_data[AXIS_ROLL] * (float)GYRO_SCALE_2000_DPS;
				buffer->gyro_data[1] = (float)data_buf.gyro_data[AXIS_PITCH] * (float)GYRO_SCALE_2000_DPS;
				buffer->gyro_data[2] = (float)data_buf.gyro_data[AXIS_YAW] * (float)GYRO_SCALE_2000_DPS;
				break;
			default:
				break;
		}

		switch(buffer->mag_meas_scale)
		{
			case SCALE_1POINT3_GAUSS:
				buffer->magnetometer_data[0] = (float)data_buf.magnetometer_data[MAG_AXIS_X] * (float)MAG_SCALE_1_3_GAUSS_XY;
				buffer->magnetometer_data[1] = (float)data_buf.magnetometer_data[MAG_AXIS_Y] * (float)MAG_SCALE_1_3_GAUSS_XY;
				buffer->magnetometer_data[2] = (float)data_buf.magnetometer_data[MAG_AXIS_Z] * (float)MAG_SCALE_1_3_GAUSS_Z;
				break;
			case SCALE_1POINT9_GAUSS:
				buffer->magnetometer_data[0] = (float)data_buf.magnetometer_data[MAG_AXIS_X] * (float)MAG_SCALE_1_9_GAUSS_XY;
				buffer->magnetometer_data[1] = (float)data_buf.magnetometer_data[MAG_AXIS_Y] * (float)MAG_SCALE_1_9_GAUSS_XY;
				buffer->magnetometer_data[2] = (float)data_buf.magnetometer_data[MAG_AXIS_Z] * (float)MAG_SCALE_1_9_GAUSS_Z;
				break;
			case SCALE_2POINT5_GAUSS:
				buffer->magnetometer_data[0] = (float)data_buf.magnetometer_data[MAG_AXIS_X] * (float)MAG_SCALE_2_5_GAUSS_XY;
				buffer->magnetometer_data[1] = (float)data_buf.magnetometer_data[MAG_AXIS_Y] * (float)MAG_SCALE_2_5_GAUSS_XY;
				buffer->magnetometer_data[2] = (float)data_buf.magnetometer_data[MAG_AXIS_Z] * (float)MAG_SCALE_2_5_GAUSS_Z;
				break;
			case SCALE_4POINT0_GAUSS:
				buffer->magnetometer_data[0] = (float)data_buf.magnetometer_data[MAG_AXIS_X] * (float)MAG_SCALE_4_0_GAUSS_XY;
				buffer->magnetometer_data[1] = (float)data_buf.magnetometer_data[MAG_AXIS_Y] * (float)MAG_SCALE_4_0_GAUSS_XY;
				buffer->magnetometer_data[2] = (float)data_buf.magnetometer_data[MAG_AXIS_Z] * (float)MAG_SCALE_4_0_GAUSS_Z;
				break;
			case SCALE_4POINT7_GAUSS:
				buffer->magnetometer_data[0] = (float)data_buf.magnetometer_data[MAG_AXIS_X] * (float)MAG_SCALE_4_7_GAUSS_XY;
				buffer->magnetometer_data[1] = (float)data_buf.magnetometer_data[MAG_AXIS_Y] * (float)MAG_SCALE_4_7_GAUSS_XY;
				buffer->magnetometer_data[2] = (float)data_buf.magnetometer_data[MAG_AXIS_Z] * (float)MAG_SCALE_4_7_GAUSS_Z;
				break;
			case SCALE_5POINT6_GAUSS:
				buffer->magnetometer_data[0] = (float)data_buf.magnetometer_data[MAG_AXIS_X] * (float)MAG_SCALE_5_6_GAUSS_XY;
				buffer->magnetometer_data[1] = (float)data_buf.magnetometer_data[MAG_AXIS_Y] * (float)MAG_SCALE_5_6_GAUSS_XY;
				buffer->magnetometer_data[2] = (float)data_buf.magnetometer_data[MAG_AXIS_Z] * (float)MAG_SCALE_5_6_GAUSS_Z;
				break;
			case SCALE_8POINT1_GAUSS:
				buffer->magnetometer_data[0] = (float)data_buf.magnetometer_data[MAG_AXIS_X] * (float)MAG_SCALE_8_1_GAUSS_XY;
				buffer->magnetometer_data[1] = (float)data_buf.magnetometer_data[MAG_AXIS_Y] * (float)MAG_SCALE_8_1_GAUSS_XY;
				buffer->magnetometer_data[2] = (float)data_buf.magnetometer_data[MAG_AXIS_Z] * (float)MAG_SCALE_8_1_GAUSS_Z;
				break;
			default:
				break;
		}

		buffer->accel_data[0] *= (float)ACC_X_SIGN;
		buffer->accel_data[1] *= (float)ACC_Y_SIGN;
		buffer->accel_data[2] *= (float)ACC_Z_SIGN;

		buffer->gyro_data[0] *= (float)GYRO_ROLL_SIGN;
		buffer->gyro_data[1] *= (float)GYRO_PITCH_SIGN;
		buffer->gyro_data[2] *= (float)GYRO_YAW_SIGN;

		buffer->magnetometer_data[0] *= (float)MAG_X_SIGN;
		buffer->magnetometer_data[1] *= (float)MAG_Y_SIGN;
		buffer->magnetometer_data[2] *= (float)MAG_Z_SIGN;

		return 0; // Return READ_SUCCESS
	}
	return -1; // Failure
}
