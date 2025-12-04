# SDK GUI Client

IGRIS SDK의 주요 기능을 시연하는 GUI 클라이언트 예제입니다.

---

## 개요

`sdk_gui_client`는 IGRIS SDK의 핵심 기능들을 하나의 GUI 애플리케이션에서 테스트하고 확인할 수 있는 예제입니다.

### 시연하는 SDK 기능

| 기능 | 설명 |
|------|------|
| **Subscriber** | `rt/lowstate` 토픽 구독 (로봇 상태 실시간 모니터링) |
| **Publisher** | `rt/lowcmd` 토픽 발행 (300Hz low-level 제어) |
| **Service Client** | BMS 초기화, 토크 제어, 제어 모드 전환 |

---

## 실행 방법

```bash
# 기본 실행 (domain_id = 0)1
./sdk_gui_client

# domain_id 지정
./sdk_gui_client <domain_id>
```

> **Note**: 로봇이 실행 중이어야 합니다.

---

## GUI 구조

```
┌─────────────────┬─────────────────┬─────────────────┐
│   Left Panel    │  Center Panel   │   Right Panel   │
│                 │                 │                 │
│  Motor/Joint    │   IMU State     │  Service API    │
│   Sliders       │                 │   Buttons       │
│   (31개)        │   LowCmd Info   │                 │
│                 │                 │  Response Log   │
└─────────────────┴─────────────────┴─────────────────┘
```

### Left Panel - Motor/Joint State Sliders

- **31개 슬라이더**: 각 모터/조인트의 위치 표시 및 제어
- **Motor/Joint 토글**: Motor Space(MS) / Joint Space(PJS) 전환
- **Reset 버튼**: 초기 위치로 복귀 (LOW_LEVEL 모드에서만 동작)

**동작 모드:**
- **Display Mode**: 현재 로봇 상태를 실시간으로 표시 (읽기 전용)
- **Control Mode**: LOW_LEVEL 모드 활성화 시 슬라이더로 목표 위치 설정

### Center Panel - IMU State & LowCmd Info

**IMU State:**
- Quaternion (w, x, y, z)
- Gyroscope (rad/s)
- Accelerometer (m/s²)
- Roll-Pitch-Yaw (rad)

**LowCmd Publishing:**
- 발행 상태 (ACTIVE/INACTIVE)
- 발행된 메시지 수
- 각 조인트별 명령 값 (q, tau, kp, kd)

### Right Panel - Service API Commands

| 버튼 | 기능 |
|------|------|
| 1. Init BMS | BMS 초기화 |
| 2. Init Motor | 모터 초기화 |
| 3. Init BMS and Motor | BMS + 모터 동시 초기화 |
| 4. BMS OFF | BMS 종료 |
| 5. Set Torque ON | 토크 활성화 |
| 6. Set Torque OFF | 토크 비활성화 |
| 7. Control Mode: LOW_LEVEL | Low-level 제어 모드 전환 |
| 8. Control Mode: HIGH_LEVEL | High-level 제어 모드 전환 |

**Response Log:** 서비스 호출 결과를 타임스탬프와 함께 표시

---

## 코드 구조

### 주요 컴포넌트

```cpp
// SDK 초기화
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

### LowState Callback

로봇 상태를 수신하여 GUI 업데이트 및 제어 루프에 사용:

```cpp
void LowStateCallback(const LowState &state) {
    // 현재 조인트/모터 위치 업데이트
    for (int i = 0; i < 31; i++) {
        g_current_joint_pos[i] = state.joint_state()[i].q();
        g_current_motor_pos[i] = state.motor_state()[i].q();
    }
}
```

### LowCmd Publishing (300Hz)

LOW_LEVEL 모드에서 목표 위치를 로봇에 전송:

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

GUI 프리징 방지를 위해 별도 스레드에서 실행:

```cpp
void CallSetControlModeAsync(IgrisC_Client *client, ControlMode mode) {
    std::thread([client, mode]() {
        auto res = client->SetControlMode(mode, 30000); // 30초 타임아웃
        // 결과 처리
    }).detach();
}
```

---

## PD Gains 설정

예제 기본값이 제공되며, 로봇 구성에 맞게 조정이 필요합니다:

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

## 모터/조인트 매핑

### Motor Names (31개)

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

## 의존성

- IGRIS SDK
- ImGui
- GLFW
- OpenGL 3.3+

---

## 주의사항

- LOW_LEVEL 모드에서는 300Hz로 LowCmd가 지속적으로 발행됩니다
- 토크 활성화 전 반드시 BMS 및 모터 초기화를 수행하세요
- PD gains는 로봇 구성에 맞게 조정이 필요합니다
