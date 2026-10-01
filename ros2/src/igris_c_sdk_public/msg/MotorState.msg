# Motor error flags carried in status_bits. These were a C++-only enum in types.hpp before the
# contract refactoring; restoring them here puts them on BOTH lanes and in the Python bindings,
# which is where the manuals already tell readers to use them ("per-motor fault/limit bits - use
# them in a GUI/logger for alarms"). Values are unchanged from that enum.
# Bit flags: test with (status_bits & FLAG), not equality.
uint32 NO_ERROR=0
uint32 INITIALIZE_ERROR=1
uint32 PACKET_NOT_RECEIVED_ERROR=2
# Mask over the two robot-level bits above.
uint32 ROBOT_LEVEL_MASK=3
# Current reached the stall current and speed stayed very low for a period: the load is too large.
uint32 MOTOR_STALL_ERROR=4
# Power input below the configured undervoltage value.
uint32 LOW_VOLTAGE_ERROR=8
# Power input above the configured overvoltage value.
uint32 OVER_VOLTAGE_ERROR=16
# Motor current over the limit: possible short circuit, phase loss, loss of control, motor damage.
uint32 OVER_CURRENT_ERROR=32
# Supply input current over the limit: load too large or speed too high.
uint32 POWER_OVERRUN_ERROR=64
# Writing parameters failed, causing parameter loss.
uint32 CALIBRATION_PARAMETER_WRITING_ERROR=128
# Running speed over the limit: possible overpressure or drag use.
uint32 SPEEDING_ERROR=256
# Motor temperature over the set value: possible short circuit, parameter error, long overload.
uint32 COMPONENT_OVERTEMPERATURE_ERROR=512
uint32 MOTOR_TEMPERATURE_OVER_TEMPERATURE_ERROR=1024
# Encoder calibration result deviates too far from the standard value.
uint32 ENCODER_CALIBRATION_ERROR=2048
uint32 ENCODER_DATA_ERROR=4096
uint32 MOTOR_BRAKE_VOLTAGE_TOO_HIGH_ERROR=8192
uint32 DRV_DRIVE_ERROR=16384

float32 q            # rad
float32 dq           # rad/s
float32 tau_est      # Nm
int16 temperature    # degC
uint32 status_bits   # CAN status word
