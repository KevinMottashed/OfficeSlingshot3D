#include "PhysicsSync.h"

PhysicsSync::PhysicsSync()
{
}

PhysicsSync::~PhysicsSync()
{
}

unsigned long PhysicsSync::age() const
{
	return _age;
}

void PhysicsSync::age(unsigned long age)
{
	_age = age;
}

std::ostream& operator<<(std::ostream& os, const PhysicsSync& sync)
{
	os << "age=" << sync.age();
	return os;
}