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

// the video data will be serialized as <width><height><rgb>
void Serialization::serialize(const VideoData& data, std::vector<BYTE>& bytes)
{
	serialize(data.width, bytes); // <width>
	serialize(data.height, bytes); // <height>
	bytes.insert(bytes.end(), data.rgb, data.rgb + data.width * data.height * BYTES_PER_PIXEL); // <rgb>
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

template <>
unsigned int Serialization::getSerializedSize<VideoData>()
{
	// video data is serialized as <width><height><rgb>	
	return 2 * sizeof(unsigned int) + IMAGE_ARRAY_SIZE;
}
