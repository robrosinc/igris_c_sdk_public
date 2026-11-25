# RECONCILED: kinematic_modes was a 32-bit IDL enum and a uint8 in msg.
# uint8 + constants wins - it is what rosidl can express, and it removes the
# 4-byte/1-byte wire mismatch between the two lanes.
uint8 KINEMATIC_MODE_MS=0
uint8 KINEMATIC_MODE_PJS=1
std_msgs/Header header
uint8[5] kinematic_modes   # waist, L_ankle, R_ankle, L_wrist, R_wrist
MotorCmd[31] motors        # N_JOINTS
