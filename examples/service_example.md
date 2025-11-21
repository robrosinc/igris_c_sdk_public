# Service API Example

IgrisC_Client를 사용한 서비스 API 호출 예제입니다.

---

## 개요

이 예제는 IGRIS SDK의 Service Client를 사용하여 로봇 시스템을 제어하는 방법을 보여줍니다.

### 시연 기능

- **BMS 초기화**: 배터리 관리 시스템 및 모터 초기화
- **토크 제어**: 모터 토크 활성화/비활성화
- **제어 모드 전환**: LOW_LEVEL / HIGH_LEVEL 모드 전환

---

## 실행 방법

```bash
# 기본 실행 (domain_id = 0)
./service_example

# domain_id 지정
./service_example 1
```

> **Note**: Domain ID는 robot controller(bridge)와 동일해야 합니다. 기본값은 0입니다.

---

## 메뉴

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

## 코드 구조

### 1. SDK 및 Client 초기화

```cpp
#include <igris_sdk/channel_factory.hpp>
#include <igris_sdk/igris_c_client.hpp>

// SDK 초기화
ChannelFactory::Instance()->Init(domain_id);

// Service Client 초기화
IgrisC_Client client;
client.Init();
client.SetTimeout(10.0f);  // 타임아웃 설정 (초)
```

### 2. BMS 초기화

```cpp
// BMS만 초기화
ServiceResponse res = client.InitBms(BmsInitType::BMS_INIT, 30000);

// 모터만 초기화
res = client.InitBms(BmsInitType::MOTOR_INIT, 30000);

// BMS + 모터 동시 초기화
res = client.InitBms(BmsInitType::BMS_AND_MOTOR_INIT, 30000);

// BMS 종료
res = client.InitBms(BmsInitType::BMS_INIT_NONE, 30000);
```

### 3. 토크 제어

```cpp
// 토크 활성화
ServiceResponse res = client.SetTorque(TorqueType::TORQUE_ON, 30000);

// 토크 비활성화
res = client.SetTorque(TorqueType::TORQUE_OFF, 30000);
```

### 4. 제어 모드 전환

```cpp
// LOW_LEVEL 모드 (직접 모터 제어)
ServiceResponse res = client.SetControlMode(
    ControlMode::CONTROL_MODE_LOW_LEVEL, 30000);

// HIGH_LEVEL 모드 (상위 제어기 사용)
res = client.SetControlMode(
    ControlMode::CONTROL_MODE_HIGH_LEVEL, 30000);
```

### 5. 응답 처리

```cpp
ServiceResponse res = client.SomeService(...);

if (res.success()) {
  std::cout << "Success: " << res.message() << std::endl;
} else {
  std::cerr << "Failed: " << res.message() << std::endl;
}
```

---

## 일반적인 사용 순서

로봇을 LOW_LEVEL 모드로 제어하기 위한 일반적인 순서:

1. **3번** - Init BMS and Motor
2. **5번** - Torque ON
3. **7번** - Control Mode: LOW_LEVEL
4. (lowlevel_example 실행)
5. **6번** - Torque OFF

---

## 출력 예시

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

## API 타임아웃

모든 서비스 호출은 타임아웃을 지정할 수 있습니다:

```cpp
// 클라이언트 기본 타임아웃 설정
client.SetTimeout(10.0f);  // 10초

// 개별 호출 타임아웃 (밀리초)
client.InitBms(BmsInitType::BMS_INIT, 30000);  // 30초
```

> **Note**: BMS 및 모터 초기화는 시간이 오래 걸릴 수 있으므로 충분한 타임아웃을 설정하세요.

---

## 주의사항

- 서비스 호출은 동기 방식으로 완료될 때까지 블로킹됩니다
- BMS 및 모터 초기화는 수십 초가 소요될 수 있습니다
- 토크 비활성화 시 로봇이 무력화되므로 주의하세요
- 제어 모드 전환 전 현재 상태를 확인하세요
