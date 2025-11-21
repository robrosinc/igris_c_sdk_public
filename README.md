# IGRIS C SDK

IGRIS_C 통신을 위한 SDK입니다.

## 요구 사항

- **OS**: Linux (Ubuntu 24.04)
- **CMake**: 3.14 이상
- **컴파일러**: C++17 호환 (GCC 9+, Clang 10+)
- **Python**: 3.12 (Python 바인딩 사용 시)

### 필수 의존성

```bash
# Ubuntu/Debian
sudo apt install libssl-dev
```

### 예제 빌드 시 추가 의존성

```bash
# Ubuntu/Debian (example 빌드에 필요)
sudo apt install libglfw3-dev libgl1-mesa-dev
```

## 디렉토리 구조

```
igris_c_sdk_public/
├── include/igris_sdk/     # SDK 헤더 파일
├── lib/
│   ├── libigris_sdk.a     # 정적 라이브러리
│   └── cmake/             # CMake 설정 파일
├── thirdparty/
│   └── include/           # Cyclone DDS 헤더
├── examples/              # 예제 코드
├── dist/                  # Python wheel 패키지
├── licenses/              # 서드파티 라이센스
├── LICENSE
└── NOTICE
```

## C++ 프로젝트에서 사용하기

### CMake 설정

```cmake
# SDK 경로 설정 (igris_c_sdk_public 위치에 맞게 수정)
set(igris_sdk_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../igris_c_sdk_public/lib/cmake/igris_sdk")
find_package(igris_sdk REQUIRED)

# 타겟에 링크
target_link_libraries(your_target igris_sdk::igris_sdk)
```

## Python 바인딩 사용하기

### 설치

```bash
# 가상환경 생성 및 활성화
python3 -m venv venv
source venv/bin/activate

# wheel 패키지 설치
pip install dist/igris_c_sdk-*.whl
```

## 예제 빌드 및 실행

```bash
cd examples
./build.sh
```


## 라이센스

이 SDK는 Cyclone DDS 라이브러리를 정적 링크하여 포함하고 있습니다. 서드파티 라이센스는 `licenses/` 디렉토리를 참조하세요.
