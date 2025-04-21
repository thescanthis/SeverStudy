#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include "RefCounting.h"
#include "Allocator.h"
#include "Memory.h"

using TL = TypeList<class Player, class Mage, class Knight, class Archer>;

class Player
{
public:
	Player()
	{
		INIT_TL(Player);
	}
	virtual ~Player() {}

	DECLARE_TL
};

class Knight : public Player
{
public:
	Knight()
	{
		INIT_TL(Knight);
	}
};

class Mage : public Player
{
public:
	Mage()
	{
		INIT_TL(Mage);
	}
};

class Archer : public Player
{
public:
	Archer()
	{
		INIT_TL(Archer);
	}
};

class Dog
{

};

int main()
{
	//TypeList<Mage, Knight>::Head whoAMI;
	//TypeList<Mage, Knight>::Tail whoAMI2;
	//TypeList<Mage, TypeList<Knight, Archer>>::Head whoAMI3;
	//TypeList<Mage, TypeList<Knight, Archer>>::Tail::Head whoAMI4;
	//TypeList<Mage, TypeList<Knight, Archer>>::Tail::Tail whoAMI5;
	//
	//int32 len1 = Length<TypeList<Mage, Knight>>::value;
	//int32 len2 = Length<TypeList<Mage, Knight, Archer>>::value;
	//using TL = TypeList<Player ,Mage, Knight, Archer>;
	//TypeAt<TL, 0>::Result;
	//
	//int32 idx1 = IndexOf<TL, Mage>::value;
	//
	//bool b1 = Conversion<Player, Knight>::exists;
	//bool b2 = Conversion<Knight, Player>::exists;
	//bool b3 = Conversion<Knight, Dog>::exists;

	{
		Player* player = new Knight();
		bool canCast = CanCast<Knight*>(player);
		Knight* knight = TypeCast<Knight*>(player);

		delete player;
	}


	return 0;
}