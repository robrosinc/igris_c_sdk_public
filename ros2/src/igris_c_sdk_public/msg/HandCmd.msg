# RECONCILED: idl had `sequence<MotorCmd>` (unbounded), msg had MotorCmd[12].
# Fixed [12] wins - the hand has exactly HAND_V1_JOINT_NUM = 12 joints.
std_msgs/Header header
MotorCmd[12] motor_cmd
