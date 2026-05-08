# SDK GUI Client

GUI example that demonstrates the main IGRIS SDK features in one application.

---

## Overview

`sdk_gui_client` combines SDK initialization, DDS pub/sub, and service calls in a single GUI.

### Covered SDK features

| Feature | Description |
|------|------|
| **Subscriber** | Subscribes to `rt/lowstate` for real-time robot state monitoring |
| **Publisher** | Publishes `rt/lowcmd` at 300 Hz for low-level control |
| **Service Client** | Calls BMS initialization, torque control, and control mode APIs |
| **ControlModeState Subscriber** | Tracks the current robot control mode from `rt/controlmodestate` |

---

## Run

```bash
# Default run (domain_id = 0)
./sdk_gui_client

# Run with an explicit domain ID
./sdk_gui_client <domain_id>
```

> **Note**: The robot controller must already be running, and the domain ID must match the controller side.

---

## UI Layout

```
┌─────────────────┬─────────────────┬─────────────────┐
│   Left Panel    │  Center Panel   │   Right Panel   │
│                 │                 │                 │
│  Motor/Joint    │   IMU State     │  Service API    │
│   Controls      │  Control Mode   │   Buttons       │
│   + Gains       │  LowCmd Status  │                 │
│                 │                 │  Response Log   │
└─────────────────┴─────────────────┴─────────────────┘
```

### Left panel

- 31 sliders for motor-space or joint-space position targets
- Toggle between **Motor State** and **Joint State**
- **Reset Pos** sets all targets to zero with a fixed 5 second transition
- **Reset Gains** restores all Kp/Kd values to the built-in defaults
- **Home** moves the robot to a simple home pose with a fixed 5 second transition
- **Duration** controls how quickly commands converge to the target during normal slider-driven moves

**Behavior**

- Before LOW_LEVEL publishing starts, the sliders show live state only
- During LOW_LEVEL publishing, sliders become command inputs
- Switching between motor mode and joint mode synchronizes targets to the current state to avoid jumps

### Center panel

**Status**

- LowState receive counter
- Current control mode from `ControlModeState`
- LowCmd publish state and message count

**IMU**

- Quaternion (w, x, y, z)
- Gyroscope (rad/s)
- Accelerometer (m/s²)
- Roll, pitch, yaw (rad)

**LowCmd details**

- publish status (ACTIVE/INACTIVE)
- published messave count
- target commands for each joint (q, tau, kp, kd)

### Right panel

| Button | Action |
|------|------|
| 1. Init BMS | Initialize only the BMS |
| 2. Init Motor | Initialize only the motors |
| 3. Init BMS and Motor | Initialize both BMS and motors |
| 4. BMS OFF | Send the shutdown request |
| 5. Set Torque ON | Enable motor torque |
| 6. Set Torque OFF | Disable motor torque |
| 7. Control Mode: LOW_LEVEL | Switch to low-level control mode |
| LowLevel + Start | Switch to LOW_LEVEL and immediately start `LowCmd` publishing |
| LowCmd Publish Start | Start publishing if the robot is already in LOW_LEVEL mode |
| LowCmd Publish Stop | Stop publishing commands |
| 8. Control Mode: HIGH_LEVEL | Switch back to high-level control mode |

**Response Log**

- Service call results are appended with timestamps

---

## Code Structure

### Core setup

```cpp
// SDK Initialization
ChannelFactory::Instance()->Init(domain_id);

// Service Client
IgrisC_Client client;
client.Init();

// Subscriber
Subscriber<LowState> lowstate_sub("rt/lowstate");
lowstate_sub.init(LowStateCallback);

// Publisher
Publisher<LowCmd> lowcmd_pub("rt/lowcmd");
lowcmd_pub.init();
```

### LowState callback

The callback keeps the latest robot state, updates the live display, and captures the initial reference when the first message arrives.

```cpp
void LowStateCallback(const LowState &state) {
    // Current Joint/Motor position update
    for (int i = 0; i < 31; i++) {
        g_current_joint_pos[i] = state.joint_state()[i].q();
        g_current_motor_pos[i] = state.motor_state()[i].q();
    }
}
```

### LowCmd Publishing (300Hz)

When LOW_LEVEL publishing is active, a background thread builds and writes `LowCmd` messages at about 300 Hz.

```cpp
void LowCmdPublishThread(Publisher<LowCmd>* publisher) {
    const auto period = std::chrono::microseconds(3333); // ~300Hz

    while (g_running) {
        if (g_lowlevel_active) {
            LowCmd cmd;
            for (int i = 0; i < 31; i++) {
                cmd.motors()[i].id(i);
                cmd.motors()[i].kinematic_mode(KinematicMode::PJS); // or MS
                cmd.motors()[i].q(target_pos);
                cmd.motors()[i].kp(default_kp[i]);
                cmd.motors()[i].kd(default_kd[i]);
            }
            publisher->write(cmd);
        }
        std::this_thread::sleep_until(next_time);
    }
}
```

### Service API 호출 (비동기)

Service calls run on detached worker threads so the GUI does not freeze during long operations such as BMS initialization.

```cpp
void CallSetControlModeAsync(IgrisC_Client *client, ControlMode mode) {
    std::thread([client, mode]() {
        auto res = client->SetControlMode(mode, 60000); // 60 seconds timeout
        // manage output
    }).detach();
}
```

---

## Default PD Gains

The example ships with built-in gains. They are only starting points and should be tuned for the actual robot.

```cpp
// Kp gains
static const std::array<float, 31> default_kp = {
    50.0, 25.0, 25.0,                          // Waist
    100.0, 100.0, 50.0, 100.0, 25.0, 300.0,    // Left leg
    500.0, 200.0, 50.0, 500.0, 300.0, 300.0,   // Right leg
    50.0, 50.0, 30.0, 30.0, 25.0, 10.0, 10.0,  // Left arm
    50.0, 50.0, 30.0, 30.0, 25.0, 10.0, 10.0,  // Right arm
    2.0, 5.0                                   // Neck
};
```

---

## Motor Mapping

### Motor Names (31)

| Index | Name | Index | Name |
|-------|------|-------|------|
| 0 | Waist_Yaw | 16 | Shoulder_Roll_L |
| 1 | Waist_L | 17 | Shoulder_Yaw_L |
| 2 | Waist_R | 18 | Elbow_Pitch_L |
| 3 | Hip_Pitch_L | 19 | Wrist_Yaw_L |
| 4 | Hip_Roll_L | 20 | Wrist_Front_L |
| 5 | Hip_Yaw_L | 21 | Wrist_Back_L |
| 6 | Knee_Pitch_L | 22 | Shoulder_Pitch_R |
| 7 | Ankle_Out_L | 23 | Shoulder_Roll_R |
| 8 | Ankle_In_L | 24 | Shoulder_Yaw_R |
| 9 | Hip_Pitch_R | 25 | Elbow_Pitch_R |
| 10 | Hip_Roll_R | 26 | Wrist_Yaw_R |
| 11 | Hip_Yaw_R | 27 | Wrist_Front_R |
| 12 | Knee_Pitch_R | 28 | Wrist_Back_R |
| 13 | Ankle_Out_R | 29 | Neck_Yaw |
| 14 | Ankle_In_R | 30 | Neck_Pitch |
| 15 | Shoulder_Pitch_L | | |

---

## Dependencies

- IGRIS SDK
- ImGui
- GLFW
- OpenGL 3.3+

---

## Notes

- `LowCmd` messages are published continuously while low-level publishing is active
- Run BMS and motor initialization before enabling torque
- Tune Kp/Kd for the target robot before using this example for real motion
