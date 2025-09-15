# Momodora_nikke

DirectX 11 기반 2D 게임 엔진과 Momodora 스타일 게임 프로젝트입니다.

## 프로젝트 구조

### Client
- ImGui 기반 게임 에디터
- 레벨 에디터, 애셋 관리, 컴포넌트 편집 기능

### Engine  
- DirectX 11 기반 2D 게임 엔진
- 렌더링, 충돌처리, 사운드, 파티클 시스템
- 컴포넌트 기반 아키텍처

### Scripts
- 게임 로직 스크립트
- 플레이어, 몬스터, UI 등의 게임 오브젝트 동작

### StateMachine
- 상태 머신 시스템
- 플레이어, 몬스터의 상태 관리

### CodeGen
- 스크립트 및 상태 자동 생성 도구

## 개발 환경

- **IDE**: Visual Studio 2022
- **언어**: C++
- **API**: DirectX 11
- **UI**: ImGui
- **사운드**: FMOD
- **폰트**: FW1FontWrapper

## 빌드 방법

1. Visual Studio 2022에서 `RemindDirectX.sln` 열기
2. 솔루션 구성을 Debug 또는 Release로 설정
3. 플랫폼을 x64로 설정
4. 빌드 및 실행

## 주요 기능

- 2D 스프라이트 렌더링
- 애니메이션 시스템 (Flipbook)
- 물리 시뮬레이션 (RigidBody2D)
- 충돌 감지 시스템
- 파티클 시스템
- 사운드 시스템
- 레벨 에디터
- 상태 머신 기반 AI

## 라이선스

이 프로젝트는 학습 목적으로 제작되었습니다.
