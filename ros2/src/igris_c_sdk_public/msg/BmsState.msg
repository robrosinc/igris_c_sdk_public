uint8 RELAY_OFF=0
uint8 RELAY_ON=1
uint8 ESTOP_RELEASED=0
uint8 ESTOP_PRESSED=1
uint8 BMS_DISCONNECTED=0
uint8 BMS_CONNECTED=1
uint8 BMS_NOT_INITIALIZED=0   # 0b00: neither (bit0=BMS, bit1=motor)
uint8 BMS_INITIALIZED=1       # 0b01: only BMS initialized
uint8 MOTOR_INITIALIZED=2     # 0b10: only motor initialized
uint8 BOTH_INITIALIZED=3      # 0b11: BMS | motor
Header header
uint8 body_power
uint8 legs_power
uint8 estop
uint8 connect
float32 battery
uint8 bms_init_state
