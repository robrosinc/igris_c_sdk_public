# RENAMED from BmsState. BMS_* constants become PDU_*, bms_init_state becomes
# pdu_init_state. shm/CAN already use the PDU vocabulary.
# RECONCILED: five 32-bit IDL enums became uint8 + constants.
uint8 RELAY_OFF=0
uint8 RELAY_ON=1
uint8 ESTOP_RELEASED=0
uint8 ESTOP_PRESSED=1
uint8 PDU_DISCONNECTED=0
uint8 PDU_CONNECTED=1
uint8 PDU_NOT_INITIALIZED=0   # 0b00: neither (bit0=PDU, bit1=motor)
uint8 PDU_INITIALIZED=1       # 0b01: only PDU initialized
uint8 MOTOR_INITIALIZED=2     # 0b10: only motor initialized
uint8 BOTH_INITIALIZED=3      # 0b11: PDU | motor
std_msgs/Header header
uint8 body_power
uint8 legs_power
uint8 estop
uint8 connect
float32 battery         # percent, 0.0 - 100.0
uint8 pdu_init_state
