# RECONCILED: two 32-bit IDL enums became uint8 + constants.
#
# The state values mirror robros::lib::common::RobotControlState (igris_c_lib,
# common/robotControlState.h) ONE FOR ONE, because the bridge publishes that enum's
# raw value: dds_manager.cpp does `rs.state() = static_cast<uint8_t>(state_value)`.
# The list below used to stop at 9, so WALK and the ten CUSTOM states went out as
# numbers a client had no name for. Add here whenever that enum grows.
uint8 ROBOT_STATE_NOT_READY=0
uint8 ROBOT_STATE_IDLE=1
uint8 ROBOT_STATE_STOP=2
uint8 ROBOT_STATE_LOW=3
uint8 ROBOT_STATE_HIGH_MOTION_ACTIVE=4
uint8 ROBOT_STATE_HIGH_MOTION_STANDBY=5
uint8 ROBOT_STATE_WALK_LOW=6
uint8 ROBOT_STATE_WALK_HIGH_MOTION_ACTIVE=7
uint8 ROBOT_STATE_WALK_HIGH_MOTION_STANDBY=8
uint8 ROBOT_STATE_WHOLE_BODY_CONTROL=9
# WALKMODE_ON (as opposed to LOW_LEVEL_ / HIGH_LEVEL_WALKMODE_ON) lands here.
uint8 ROBOT_STATE_WALK=10
# One state per ControlModeCommandRequest CUSTOM_MODE_N.
uint8 ROBOT_STATE_CUSTOM_1=11
uint8 ROBOT_STATE_CUSTOM_2=12
uint8 ROBOT_STATE_CUSTOM_3=13
uint8 ROBOT_STATE_CUSTOM_4=14
uint8 ROBOT_STATE_CUSTOM_5=15
uint8 ROBOT_STATE_CUSTOM_6=16
uint8 ROBOT_STATE_CUSTOM_7=17
uint8 ROBOT_STATE_CUSTOM_8=18
uint8 ROBOT_STATE_CUSTOM_9=19
uint8 ROBOT_STATE_CUSTOM_10=20
uint8 ROBOT_ENV_REAL=0
uint8 ROBOT_ENV_SIM=1
std_msgs/Header header
uint8 state
uint8 environment
