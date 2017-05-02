#include "madgwick_wrapper.h"

static volatile imu_scaled_data_struct imu_data;

extern volatile float q0, q1, q2, q3;

static volatile float mx_offset, my_offset, mz_offset;

void init_madgwick(ACC_SCALE a, GYRO_SCALE g, MAG_SCALE m, 
					float dt_sec, float beta_value)
{
	set_sampleFreq(1.0f/dt_sec);
	set_beta(beta_value);

	q0 = 1.0f;
	q1 = 0.0f;
	q2 = 0.0f;
	q3 = 0.0f;

	mx_offset = 0.0f;
	my_offset = 0.0f;
	mz_offset = 0.0f;

	initialize_imu(a, g, m, &imu_data);
}

// void mag_cal(void)
// {
	
// }

void quaternionToEuler(float* euler_angles, float* quaternion_input)
{
	float q_0 = quaternion_input[0];
	float q_1 = quaternion_input[1];
	float q_2 = quaternion_input[2];
	float q_3 = quaternion_input[3];

	euler_angles[0] = atan2f(2.0f*(q_0*q1 + q_2*q_3), 1.0f-(2.0f*(q_1*q_1 + q_2*q_2)));
	euler_angles[1] = asinf(2.0f*(q_0*q2 - q_3*q_1));
	euler_angles[2] = atan2f(2.0f*(q_0*q3 + q_1*q_2), 1.0f-(2.0f*(q_2*q_2 + q_3*q_3)));
}

void update_madgwick(void)
{
	get_scaled_imu_data(&imu_data);
	MadgwickAHRSupdate(imu_data.gyro_data[0], imu_data.gyro_data[1], imu_data.gyro_data[2],
						imu_data.accel_data[0], imu_data.accel_data[1], imu_data.accel_data[2],
						// 0.0f, 0.0f, 0.0f);
						imu_data.magnetometer_data[0], imu_data.magnetometer_data[1], imu_data.magnetometer_data[2]);
}

void madgwick_get_attitude(float* attitude)
{
	float att_quat[4];
	get_attitude_quaternion(att_quat);
	quaternionToEuler(attitude, att_quat);
}