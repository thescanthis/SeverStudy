#pragma once
#include "BufferReader.h"
#include "BufferWriter.h"

enum
{
	S_TEST = 1
};

template<typename T, typename C>
class PacketIterator
{
public:
	PacketIterator(C& container, uint16 index) : _container(container), _index(index) {}

	bool				operator!=(const PacketIterator& other) const { return _index != other._index; }
	const T& operator*()	 const { return _container[_index]; }
	T& operator*() { return _container[_index]; }
	T* operator->() { return &_container[_index]; }
	PacketIterator& operator++() { _index++; return *this; }
	PacketIterator		operator++(int32) { PacketIterator  ret = *this; ++_index; return ret; }
private:
	C& _container;
	uint16	_index;
};

template<typename T>
class PacketList
{
public:
	PacketList() : _data(nullptr), _count(0) {}
	PacketList(T* data, uint16 count) : _data(data), _count(count) {};

	T& operator[](uint16 index)
	{
		ASSERT_CRASH(index < _count);
		return _data[index];
	}

	uint16 Count() { return _count; };

	//ranged-base
	PacketIterator<T, PacketList<T>> begin() { return PacketIterator<T, PacketList<T>>(*this, 0); }
	PacketIterator<T, PacketList<T>> end() { return PacketIterator<T, PacketList<T>>(*this, _count); }

private:
	T* _data;
	uint16 _count;
};

class ServerPacketHandler
{
public:
	static void HandlePacket(BYTE* buffer, int32 len);
};

#pragma pack(1)
//    고정			  가변
// [PKT_S_TEST][              ]
struct PKT_S_TEST
{
	struct BuffListItem
	{
		uint64 buffId;
		float remainTime;
	};

	uint16 packetSize; //공용헤더
	uint16 packetId; //공용헤더
	uint64 id;
	uint32 hp;
	uint16 attack;
	uint16 buffsOffset;
	uint16 buffsCount;
};

class PKT_S_TEST_WRITE
{
public:
	using BuffsListItem = PKT_S_TEST::BuffListItem;
	using BuffsList = PacketList<PKT_S_TEST::BuffListItem>;

	PKT_S_TEST_WRITE(uint64 id, uint32 hp, uint16 attack)
	{
		_sendBuffer = GSendBufferManager->Open(4096);
		_bw = BufferWriter(_sendBuffer->Buffer(), _sendBuffer->AllocSize());

		_pkt = _bw.Reserve<PKT_S_TEST>();
		_pkt->packetSize = 0;
		_pkt->packetId = S_TEST;
		_pkt->id = id;
		_pkt->hp = hp;
		_pkt->attack = attack;
		_pkt->buffsOffset = 0; //To Fill
		_pkt->buffsCount = 0; //To Fill
	}

	//내가 사용할 버퍼의 갯수를 지정.
	BuffsList ReserveBuffList(uint16 buffCount)
	{
		BuffsListItem* firstBuffsListItme = _bw.Reserve<BuffsListItem>(buffCount);
	}

private:
	PKT_S_TEST* _pkt = nullptr;
	SendBufferRef _sendBuffer;
	BufferWriter _bw;
};