# Low-Level Control Example

Pub/sub example that subscribes to `LowState` and publishes `LowCmd` for direct robot control.

---

## Overview

This example demonstrates the IGRIS SDK DDS workflow for low-level motion control.

### Covered features

- **Subscriber**: receive robot state from `rt/lowstate`
- **Publisher**: send motor commands to `rt/lowcmd` at 300 Hz
- **Motion demo**: apply a sine-wave nodding motion to the neck pitch joint while holding the other joints at their initial positions

---

## Prerequisites

Before running this example, prepare the robot with either `service_example` or `sdk_gui_client`:

1. Initialize BMS and motors
    ```bash
    ./service_example init_all
    ```
2. Enable torque
    ```bash
    ./service_example torque_on
    ```
3. Switch the robot to `LOW_LEVEL` mode
    ```bash
    ./service_example mode_lowlevel
    ```
This example assumes the robot is already ready to accept `LowCmd` messages.
Require to `sdk_gui_client` for the entire operating flow.

---

## Run

```bash
# Default run (domain_id = 0)
./lowlevel_example

# Run with an explicit domain ID
./lowlevel_example 1
```

> **Note**: The domain ID must match the robot channel configuration. The default is `0`.

---

## Code Structure

### 1. SDK initialization

```cpp
#include <igris_sdk/channel_factory.hpp>
#include <igris_sdk/publisher.hpp>
#include <igris_sdk/subscriber.hpp>

// initialize
ChannelFactory::Instance()->Init(domain_id);
```

### 2. Create the subscriber

```cpp
Subscriber<LowState> state_sub("rt/lowstate");
state_sub.init(LowStateCallback);

void LowStateCallback(const LowState &state) {
    // handle robot state
    for (int i = 0; i < NUM_MOTORS; i++) {
        float pos = state.joint_state()[i].q();
        float vel = state.joint_state()[i].dq();
    }
    // IMU data 
    auto &imu = state.imu_state();
    // imu.rpy(), imu.gyroscope(), imu.accelerometer()
}
```

### 3. Create the publisher

```cpp
Publisher<LowCmd> cmd_pub("rt/lowcmd");
cmd_pub.init();
```

### 4. Publish control commands

```cpp
LowCmd cmd;
cmd.kinematic_mode(KinematicMode::PJS); // Joint Space

for (int i = 0; i < NUM_MOTORS; i++) {
    auto &motor_cmd = cmd.motors()[i];
    motor_cmd.id(i);
    motor_cmd.q(target_position);         // target position [rad]
    motor_cmd.dq(0.0f);                   // Target velocity [rad/s]
    motor_cmd.tau(0.0f);                  // Feedforward torque [Nm]
    motor_cmd.kp(kp[i]);                  // kP
    motor_cmd.kd(kd[i]);                  // kD
}

cmd_pub.write(cmd);
```

---

## Control Parameters

### Kinematic mode

| Mode | Description |
|------|------|
| `KinematicMode::MS` | Motor space |
| `KinematicMode::PJS` | Joint space |

### PD gains

The example includes built-in gains that should be tuned for the real robot:

```cpp
static const std::array<float, 31> kp = {
    50.0,  25.0,  25.0,
    500.0, 200.0, 50.0, 500.0, 300.0, 300.0,
    500.0, 200.0, 50.0, 500.0, 300.0, 300.0,
    50.0,  50.0,  30.0, 30.0,  5.0,   5.0,   5.0,
    50.0,  50.0,  30.0, 30.0,  5.0,   5.0,   5.0,
    2.0,   5.0
};
```

---

## Runtime Behavior

1. Initialize the SDK and DDS entities
2. Wait for the first `LowState`
3. Save the current joint positions as the initial pose
4. Start a 300 Hz control loop
5. Hold every joint at its initial position
6. Drive only neck pitch with a sine wave
7. Print IMU and neck status once per second

---

## Example Output

```
=== IGRIS SDK Low-Level Example ===
Domain ID: 0
LowState subscriber initialized
LowCmd publisher initialized
Waiting for robot state...
Initial state captured

Starting control loop (300Hz)
Neck pitch will nod up and down
Press Ctrl+C to stop

Time: 1.0s | IMU RPY: [0.01, -0.02, 0.00] | Neck Pitch: 0.15
Time: 2.0s | IMU RPY: [0.01, -0.02, 0.00] | Neck Pitch: -0.15
```

---

## Notes

- Torque must already be enabled before you run this example
- The example only works in `LOW_LEVEL` mode
- Tune PD gains for the target robot before real operation
- Stop the example safely with `Ctrl+C`
