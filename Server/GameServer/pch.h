#pragma once

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif
#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#include "CorePch.h"
#include "Enum.pb.h"

using GameSessionRef = shared_ptr<class GameSession>;
using PlayerRef = shared_ptr<class Player>;

#if 0 SpinLock
ó���� � ���� �Ǳ⸦ ����Ѵ�.(expected)
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
���� �о������
Lock�����
�������� ���� ����
Lock�� Ǯ��
���Ǻ��� ���� �ٸ� �����忡�� ����

���� ���ö�
������������ �õ�
����Ȯ��
���� O->�������ͼ� �̾ �ڵ�����
���� X->Lock�� Ǯ���ְ� �����·� ��ȯ.
#endif

#if 0 future
async ������� �Լ��� �񵿱� �������� ���� (��������� �ڷιи��°��� �񵿱�)
deferred -> �����ؼ� �����ϼ���
async -> ������ �����带 ���� �����ϼ��� (��Ƽ������ ȯ��� ���� ���ķ� ���� ������.)
deferred | async  �˾Ƽ� ����ּ���

���
mutex,condition_varialbe���� �����ʰ� �ܼ��� �ֵ��� ó���� �� �ִ�
Ư����, �ѹ� �߻��ϴ� �̺�Ʈ�� �����ϴ�.!
1 async : ���ϴ� �Լ��� �񵿱������� ����
2 promise : ������� promise�� ���� future�� �޾���
3 packaged_task : ���ϴ� �Լ��� ���� ����� packaged_task�� ���� future�� �޾���.
#endif

#if 0 memorey model
�޸� �� : atomic ���꿡 ����, ��羲���尡 ���� ��ü�� ���ؼ� ������ ���������� �����Ѵ�. �����Ģ.!

weak��  Spurious Failure : ��¥���а� ����������
�ٸ��������� Interruption�� �޾Ƽ� �߰��� �����ϴ� ���ѻ�Ȳ
strong�� ���ο��� �����Ҷ����� ���ư�  ���� ����ϴ�!

Memory Model ��å
 1) seq_cst          �� �������~
 ���ü����� �ذ�, �ڵ� ���ġ �ذ�
 
 2) acquier-release  �� ��������~
 releace ��������� �޸� ��ɵ��� �ش� ��� ���ķ� ��� ġ�Ǵ°��� ����
 �׸��� acquire�� ���� ������ �д� �����尡 �ִٸ�,
 releace������ ��ɵ��� -> acquire���ϴ¼����� ��������(���ü� ����)
 
 3) relaxed		   �� ��������~
 �ʹ����� �����ο�.
 �ڵ����ġX ���ü� X
 ���ϰ�ü������ ������ ���������� ����.
#endif

#if 0 TLS Thread Local Storage
	�����帶�� �ڽŸ� �����Ҽ��ִ� ������ ����.
#endif

#if 0 memory.
	�����̺�Ʈ
	CALL $(SolutionDir)Common\Protobuf\bin\GenPackets.bat
#endif