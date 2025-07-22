#pragma once

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#include "CorePch.h"
#include "Enum.pb.h"

using GameSessionRef = shared_ptr<class GameSession>;
using PlayerRef = shared_ptr<class Player>;

#if 0 SpinLock
처음에 어떤 값이 되기를 기대한다.(expected)
if (expected == _locked)
{
	expected = _locked;
	_locked = desired;
	return true;
}
else {
	return false;
}
#endif

#if 0 condition_variable
값을 밀어넣을때
Lock을잡고
공유변수 값을 수정
Lock을 풀고
조건변수 통해 다른 쓰레드에게 통지

값을 빼올때
락을잡으려고 시도
조건확인
만족 O->빠져나와서 이어서 코드진행
만족 X->Lock을 풀어주고 대기상태로 전환.
#endif

#if 0 future
async 방식으로 함수가 비동기 형식으로 실행 (실행시점이 뒤로밀리는것이 비동기)
deferred -> 지연해서 실행하세요
async -> 별도의 쓰레드를 만들어서 실행하세요 (멀티쓰레드 환경과 같음 병렬로 같이 실행함.)
deferred | async  알아서 골라주세요

결론
mutex,condition_varialbe까지 가지않고 단순한 애들을 처리할 수 있는
특히나, 한번 발생하는 이벤트에 유용하다.!
1 async : 원하는 함수를 비동기적으로 실행
2 promise : 결과물을 promise를 통해 future로 받아줌
3 packaged_task : 원하는 함수의 실행 결과를 packaged_task를 통해 future로 받아줌.
#endif

#if 0 memorey model
메모리 모델 : atomic 연산에 한해, 모든쓰레드가 동일 객체에 대해서 동일한 수정순서를 관찰한다. 절대법칙.!

weak는  Spurious Failure : 가짜실패가 있을수있음
다른쓰레드의 Interruption을 받아서 중간에 실패하는 묘한상황
strong은 내부에서 성공할때까지 돌아감  거의 비슷하다!

Memory Model 정책
 1) seq_cst          ↓ 가장엄격~
 가시성문제 해결, 코드 재배치 해결
 
 2) acquier-release  ↓ 덜엄격함~
 releace 명령이전의 메모리 명령들이 해당 명령 이후로 재배 치되는것을 금지
 그리고 acquire로 같은 변수를 읽는 쓰레드가 있다면,
 releace이전의 명령들이 -> acquireㅎ하는순간에 관찰가능(가시성 보장)
 
 3) relaxed		   ↓ 아주프리~
 너무나도 자유로움.
 코드재배치X 가시성 X
 동일객체에대해 동일한 수정순서만 보장.
#endif

#if 0 TLS Thread Local Storage
	쓰레드마다 자신만 접근할수있는 공간이 있음.
#endif

#if 0 memory.
	빌드이벤트
	CALL $(SolutionDir)Common\Protobuf\bin\GenPackets.bat
#endif