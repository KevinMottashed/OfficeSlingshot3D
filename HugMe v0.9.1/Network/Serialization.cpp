#include "Serialization.h"

using namespace std;

// the cVector3d will be serialized as <x><y><z>
void Serialization::serialize(const cVector3d& data, vector<BYTE>& bytes)
{
	bytes.insert(bytes.end(), (BYTE*) &data.x, ((BYTE*) &data.x) + sizeof(data.x));
	bytes.insert(bytes.end(), (BYTE*) &data.y, ((BYTE*) &data.y) + sizeof(data.x));
	bytes.insert(bytes.end(), (BYTE*) &data.z, ((BYTE*) &data.z) + sizeof(data.x));
	return;
}

// the projectile will be serialized as <position><speed>
void Serialization::serialize(const Projectile& data, std::vector<BYTE>& bytes)
{
	serialize(data.getPosition(), bytes);
	serialize(data.getSpeed(), bytes);
	return;
}

template <>
unsigned int Serialization::getSerializedSize<cVector3d>()
{
	// a vector is serialized as <x><y><z>
	return 3 * sizeof(double);
}

template <>
unsigned int Serialization::getSerializedSize<Projectile>()
{
	// a projectile is serialized as <position><speed>
	return 2 * getSerializedSize<cVector3d>();
}
