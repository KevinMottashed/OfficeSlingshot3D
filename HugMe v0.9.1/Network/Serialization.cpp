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