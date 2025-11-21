# Low-Level Control Example

LowState 구독과 LowCmd 발행을 통한 저수준 제어 예제입니다.

---

## 개요

이 예제는 IGRIS SDK의 Pub/Sub 패턴을 사용하여 로봇을 제어하는 방법을 보여줍니다.

### 시연 기능

- **Subscriber**: `rt/lowstate` 토픽에서 로봇 상태 수신
- **Publisher**: `rt/lowcmd` 토픽으로 모터 명령 발행 (300Hz)
- **Motion**: Neck pitch 조인트에 sine wave 모션 적용 (끄덕끄덕)

---

## 사전 조건

이 예제는 Pub/Sub API 사용법을 보여주는 것으로, 실행 전 다음이 필요합니다:

```bash
# 1. BMS 및 모터 초기화
./service_example init_all

# 2. 토크 활성화
./service_example torque_on

# 3. LOW_LEVEL 모드 전환
./service_example mode_lowlevel
```

전체 동작 플로우는 `sdk_gui_client` 예제를 참고하세요.

---

## 실행 방법

```bash
# 기본 실행 (domain_id = 0)
./lowlevel_example

# domain_id 지정
./lowlevel_example 1
```

> **Note**: Domain ID는 robot의 channel과 동일해야 합니다(robot sdk domain : 0). 기본값은 0입니다.

---

## 코드 구조

### 1. SDK 초기화

```cpp
#include <igris_sdk/channel_factory.hpp>
#include <igris_sdk/subscriber.hpp>
#include <igris_sdk/publisher.hpp>

// 초기화
ChannelFactory::Instance()->Init(domain_id);
```

### 2. Subscriber 생성

```cpp
Subscriber<LowState> state_sub("rt/lowstate");
state_sub.init(LowStateCallback);

void LowStateCallback(const LowState &state) {
  // 로봇 상태 처리
  for (int i = 0; i < NUM_MOTORS; i++) {
    float pos = state.joint_state()[i].q();
    float vel = state.joint_state()[i].dq();
  }

  // IMU 데이터
  auto& imu = state.imu_state();
  // imu.rpy(), imu.gyroscope(), imu.accelerometer()
}
```

### 3. Publisher 생성

```cpp
Publisher<LowCmd> cmd_pub("rt/lowcmd");
cmd_pub.init();
```

### 4. 제어 명령 발행

```cpp
LowCmd cmd;
cmd.kinematic_mode(KinematicMode::PJS);  // Joint Space (전체 적용)

for (int i = 0; i < NUM_MOTORS; i++) {
  auto& motor_cmd = cmd.motors()[i];
  motor_cmd.id(i);
  motor_cmd.q(target_position);                  // 목표 위치
  motor_cmd.dq(0.0f);                            // 목표 속도
  motor_cmd.tau(0.0f);                           // 피드포워드 토크
  motor_cmd.kp(kp_gain);                         // P 게인
  motor_cmd.kd(kd_gain);                         // D 게인
}

cmd_pub.write(cmd);
```

---

## 제어 파라미터

### Kinematic Mode

| Mode | 설명 |
|------|------|
| `KinematicMode::MS` | Motor Space - 모터 좌표계 |
| `KinematicMode::PJS` | Joint Space - 조인트 좌표계 |

### PD Gains

```cpp
// 예제 기본값 - 로봇에 맞게 조정 필요
static const std::array<float, 31> kp = {
  50.0, 25.0, 25.0,                          // Waist
  100.0, 100.0, 50.0, 100.0, 25.0, 300.0,    // Left leg
  ...
};
```

---

## 동작 설명

1. SDK 및 Pub/Sub 초기화
2. 첫 번째 LowState 수신 대기
3. 초기 위치 저장
4. 300Hz 제어 루프 시작
   - 모든 조인트: 초기 위치 유지
   - Neck pitch: sine wave 모션 적용 (끄덕끄덕)
5. 매초 상태 출력 (IMU, Neck pitch 위치)

---

## 출력 예시

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
...
```

---

## 주의사항

- 실행 전 토크가 활성화되어 있어야 합니다
- LOW_LEVEL 모드에서만 동작합니다
- PD gains는 로봇에 맞게 조정하세요
- Ctrl+C로 안전하게 종료할 수 있습니다
