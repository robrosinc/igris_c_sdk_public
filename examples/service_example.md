# Service API Example

Console example for calling the IGRIS SDK service APIs through `IgrisC_Client`.

---

## Overview

This example shows how to control robot system state from a blocking command-line menu.

### Covered operations

- **BMS initialization**: initialize the BMS, the motors, or both
- **Torque control**: enable or disable motor torque
- **Control mode switching**: move between `LOW_LEVEL` and `HIGH_LEVEL`

---

## Run

```bash
# Default run (domain_id = 0)
./service_example

# Run with an explicit domain ID
./service_example 1
```

> **Note**: The domain ID must match the robot controller or bridge. The default is `0`.

---

## Menu

```
=== Service API Menu ===
1. Init BMS
2. Init Motor
3. Init BMS and Motor
4. BMS OFF
5. Torque ON
6. Torque OFF
7. Control Mode: LOW_LEVEL
8. Control Mode: HIGH_LEVEL
9. Exit

Select (1-9):
```

---

## Code Structure

### 1. SDK and client initialization

```cpp
#include <igris_sdk/channel_factory.hpp>
#include <igris_sdk/igris_c_client.hpp>

// SDK Initialization
ChannelFactory::Instance()->Init(domain_id);

// Service Client Initialization
IgrisC_Client client;
client.Init();
client.SetTimeout(10.0f); // set timeout (s)
```

### 2. BMS initialization calls

```cpp
// Only Initialize BMS
ServiceResponse res = client.InitBms(BmsInitType::BMS_INIT, 60000);

// Only Initialize Motor
res = client.InitBms(BmsInitType::MOTOR_INIT, 60000);

// Initialize BMS & Motor
res = client.InitBms(BmsInitType::BMS_AND_MOTOR_INIT, 60000);

// Stop BMS
res = client.InitBms(BmsInitType::BMS_INIT_NONE, 60000);
```

### 3. Torque control calls

```cpp
// Activate Torque
ServiceResponse res = client.SetTorque(TorqueType::TORQUE_ON, 60000);

// Deactivate Torque
res = client.SetTorque(TorqueType::TORQUE_OFF, 60000);
```

### 4. Control mode switching

```cpp
// LOW_LEVEL mode (control motor directly)
ServiceResponse res = client.SetControlMode(
    ControlMode::CONTROL_MODE_LOW_LEVEL, 60000);

// HIGH_LEVEL mode (control with high-level controller)
res = client.SetControlMode(
    ControlMode::CONTROL_MODE_HIGH_LEVEL, 60000);
```

### 5. Response handling

```cpp
if (res.success()) {
    std::cout << "Success: " << res.message() << std::endl;
} else {
    std::cerr << "Failed: " << res.message() << std::endl;
}
```

---

## Typical Sequence

Typical order when preparing the robot for low-level control:

1. Select `3` for **Init BMS and Motor**
2. Select `5` for **Torque ON**
3. Select `7` for **Control Mode: LOW_LEVEL**
4. Run `lowlevel_example` or start low-level publishing from `sdk_gui_client`
5. Select `6` for **Torque OFF** when finished

---

## Example Output

```
=== IGRIS SDK Service Example ===
Domain ID: 0
Service client initialized (timeout: 10s)

=== Service API Menu ===
1. Init BMS
2. Init Motor
3. Init BMS and Motor
4. BMS OFF
5. Torque ON
6. Torque OFF
7. Control Mode: LOW_LEVEL
8. Control Mode: HIGH_LEVEL
9. Exit

Select (1-9): 3
Calling InitBms(BMS_AND_MOTOR_INIT)...

[Init BMS and Motor] SUCCESS - BMS and motor initialized successfully

=== Service API Menu ===
...
```

---

## Timeouts

All service calls allow a timeout value.

```cpp
client.SetTimeout(10.0f);  // default client timeout in seconds
client.InitBms(BmsInitType::BMS_INIT, 60000);  // per-call timeout in ms
```

> **Note**: BMS and motor initialization can take a long time. Use a timeout that is large enough for the real hardware.

---

## Notes

- Service calls are synchronous and block until they finish or time out
- BMS and motor initialization may take tens of seconds
- Disabling torque can make the robot go limp; use it carefully
- Confirm the current robot state before switching control modes
