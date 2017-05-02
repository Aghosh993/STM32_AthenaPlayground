#ifndef IMU_H
#define IMU_H	1

#include <math.h>

#include "imu_hal.h"

/*
	Accelerometer conversion factor results in m/s units:
	NOTE: All datasheet mg/LSB specs must be divided by 16!!
 */

#define ACC_SCALE_2G				0.000613125f
#define ACC_SCALE_4G				0.00122625f
#define ACC_SCALE_8G				0.0024525f
#define ACC_SCALE_16G				0.0073575f

/*
	Gyro conversion factor results in degrees/s units:
 */

#define GYRO_SCALE_250_DPS			0.00875f
#define GYRO_SCALE_500_DPS			0.01750f
#define GYRO_SCALE_2000_DPS			0.07000f

/*
	Magnetometer conversion factor results in Milli-Gauss units:
 */

#define MAG_SCALE_1_3_GAUSS_XY		0.000909091f
#define MAG_SCALE_1_3_GAUSS_Z		0.001020408f
#define MAG_SCALE_1_9_GAUSS_XY		0.001169591f
#define MAG_SCALE_1_9_GAUSS_Z		0.001315789f
#define MAG_SCALE_2_5_GAUSS_XY		0.001492537f
#define MAG_SCALE_2_5_GAUSS_Z		0.001666667f
#define MAG_SCALE_4_0_GAUSS_XY		0.002222222f
#define MAG_SCALE_4_0_GAUSS_Z		0.002500000f
#define MAG_SCALE_4_7_GAUSS_XY		0.002500000f
#define MAG_SCALE_4_7_GAUSS_Z		0.002857143f
#define MAG_SCALE_5_6_GAUSS_XY		0.003030303f
#define MAG_SCALE_5_6_GAUSS_Z		0.003389831f
#define MAG_SCALE_8_1_GAUSS_XY		0.004347826f
#define MAG_SCALE_8_1_GAUSS_Z		0.004878049f

int get_scaled_imu_data(imu_scaled_data_struct* buffer);

#endif