import sys

import igris_c_sdk as igc_sdk

print(igc_sdk.__dir__())

domain_id = 0

channel_instance = igc_sdk.ChannelFactory.Instance()
channel_instance.Init(domain_id)

client = igc_sdk.IgrisC_Client()
client.Init()
client.SetTimeout(5.0)

lowstate_subscriber = igc_sdk.LowStateSubscriber("LowStateTopic")
lowstate_cnt = 0


def lowstate_callback(msg):
    lowstate_cnt += 1
    if (lowstate_cnt % 100) == 0:
        print(f"LowState message count: {lowstate_cnt}")
        print("Received LowState message:")
        print(f"  position: {msg.position}")
        print(f"  velocity: {msg.velocity}")
        print(f"  torque: {msg.torque}")


lowstate_subscriber.init(lowstate_callback)

lowcmd_publisher = igc_sdk.LowCmdPublisher("LowCmdTopic")
lowcmd_publisher.init()


def call_init_bms(
    client: igc_sdk.IgrisC_Client, bms_init_type: igc_sdk.BmsInitType, type_name: str
):
    print(f"Calling Init with {type_name}...")
    res = client.InitBms(bms_init_type, 5000)

    result = "SUCCESS" if res.success() else "FAILED" + " - " + res.message()
    print(f"Init {type_name} result: {result}")


def call_set_torque(client: igc_sdk.IgrisC_Client, torque_type: igc_sdk.TorqueType, type_name: str):
    print(f"Calling Set Torque with {type_name}...")
    res = client.SetTorque(torque_type, 5000)

    result = "SUCCESS" if res.success() else "FAILED" + " - " + res.message()
    print(f"Set Torque {type_name} result: {result}")


def call_set_control_mode(
    client: igc_sdk.IgrisC_Client, control_mode: igc_sdk.ControlMode, mode_name: str
):
    print(f"Calling Set Control Mode with {mode_name}...")
    res = client.SetControlMode(control_mode, 5000)

    result = "SUCCESS" if res.success() else "FAILED" + " - " + res.message()
    print(f"Set Control Mode {mode_name} result: {result}")


print("Select menu: ")
print("1. Init BMS")
print("2. Init Motor")
print("3. Init BMS and Motor")
print("4. BMS OFF")
print("5. Set Torque On")
print("6. Set Torque Off")
print("7. Control Mode: LOW_LEVEL")
print("8. Control Mode: HIGH_LEVEL")
print("9. Publish Low Cmd Message")
print("0. Exit")

choice = input("Enter choice: ")
if choice == "1":
    call_init_bms(client, igc_sdk.BmsInitType.BMS_INIT, "BMS_INIT")
if choice == "2":
    call_init_bms(client, igc_sdk.BmsInitType.MOTOR_INIT, "MOTOR_INIT")
if choice == "3":
    call_init_bms(client, igc_sdk.BmsInitType.BMS_AND_MOTOR_INIT, "BMS_AND_MOTOR_INIT")
if choice == "4":
    call_init_bms(client, igc_sdk.BmsInitType.BMS_INIT_NONE, "BMS_OFF")
if choice == "5":
    call_set_torque(client, igc_sdk.TorqueType.TORQUE_ON, "TORQUE_ON")
if choice == "6":
    call_set_torque(client, igc_sdk.TorqueType.TORQUE_OFF, "TORQUE_OFF")
if choice == "7":
    call_set_control_mode(
        client, igc_sdk.ControlMode.CONTROL_MODE_LOW_LEVEL, "CONTROL_MODE_LOW_LEVEL"
    )
if choice == "8":
    call_set_control_mode(
        client, igc_sdk.ControlMode.CONTROL_MODE_HIGH_LEVEL, "CONTROL_MODE_HIGH_LEVEL"
    )
if choice == "9":
    low_cmd_msg = igc_sdk.LowCmd()
    for i in range(0, 31):
        motor_cmd = low_cmd_msg.motors()[i]
        motor_cmd.q(0.0)
        motor_cmd.dq(0.0)
        motor_cmd.tau(0.0)
        motor_cmd.kp(0.0)
        motor_cmd.kd(0.0)
        motor_cmd.id(i)
    low_cmd_msg.kinematic_mode(igc_sdk.KinematicMode.MS)
    lowcmd_publisher.write(low_cmd_msg)

if choice == "0":
    print("Exiting...")
    sys.exit(0)
