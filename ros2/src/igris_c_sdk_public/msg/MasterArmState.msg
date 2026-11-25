# MasterArmConnState enum -> uint8 + constants (rosidl has no enum type).
uint8 MASTERARM_NOT_INITIALIZED=0   # not initialized
uint8 MASTERARM_INITIALIZED=1       # initialized, not publishing LowCmd
uint8 MASTERARM_PUBLISHING=2        # publishing LowCmd
std_msgs/Header header
uint8 state
sensor_msgs/Imu imu_state
MotorState[31] motor_state   # N_JOINTS
JointState[31] joint_state   # N_JOINTS
sensor_msgs/Joy joy_state    # replaces the custom JoyState - fields are identical
