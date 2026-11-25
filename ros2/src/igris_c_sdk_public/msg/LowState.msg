std_msgs/Header header
sensor_msgs/Imu imu_state
MotorState[31] motor_state   # N_JOINTS
JointState[31] joint_state   # N_JOINTS
# NOTE: imu_state is sensor_msgs/Imu now. Consequences:
#  - orientation is a named-field Quaternion (x/y/z/w), so the [w,x,y,z] vs
#    [x,y,z,w] ordering class of bugs becomes structurally impossible.
#  - rpy is GONE - sensor_msgs/Imu has no such field. It is derivable from
#    orientation; consumers that displayed it must compute it.
#  - the three 9-element covariances are now required fields. shm's
#    imuShmData_st already carries all three, so they are fillable.
