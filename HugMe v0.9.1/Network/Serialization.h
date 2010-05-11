#ifndef SERIALIZE_H
#define SERIALIZE_H

#include "stdafx.h"
#include "chai3d.h"
#include "StaticAssert.h"
#include "Projectile.h"
#include "VideoData.h"

namespace Serialization
{
	//--------------------------------------------------------------------
	// SERIALIZATION
	//--------------------------------------------------------------------

	// Templates parameters
	// T : primitive type

	// serializes a cVector3d
	void serialize(const cVector3d& data, std::vector<BYTE>& bytes);

	// serializes a projectile
	void serialize(const Projectile& data, std::vector<BYTE>& bytes);

	// serializes video data
	void serialize(const VideoData& data, std::vector<BYTE>& bytes);

	// serializes a type T
	template <typename T>
	void serialize(const T& data, std::vector<BYTE>& bytes);

	// get the size of a serialized data type
	template <typename T>
	unsigned int getSerializedSize();

	//--------------------------------------------------------------------
	// DESERIALIZATION
	//--------------------------------------------------------------------

	// Templates parameters
	// T : primitive type
	// Iterator : random access iterator to a single byte

	// deserialize a cVector3d
	template <typename Iterator>
	void deserialize(Iterator begin, Iterator end, cVector3d& data);

	// deserialize a projectile
	template <typename Iterator>
	void deserialize(Iterator begin, Iterator end, Projectile& data);

	// deserialize a video data
	template <typename Iterator>
	void deserialize(Iterator begin, Iterator end, VideoData& data);

	// deserialize a type T
	template <typename Iterator, typename T>
	void deserialize(Iterator begin, Iterator end, T& data);
}

//--------------------------------------------------------------------
// TEMPLATE IMPLEMENTATIONS
//--------------------------------------------------------------------

template <typename T>
void Serialization::serialize(const T& data, std::vector<BYTE>& bytes)
{
	bytes.insert(bytes.end(), (BYTE*) &data, ((BYTE*) &data) + sizeof(T));
	return;
}

template <typename Iterator, typename T>
void Serialization::deserialize(Iterator begin, Iterator end, T& data)
{
	// the iterator must point to a single byte
	static_assert(sizeof(*begin) == 1);

	// for a primitive the serialized data is equal to the non-serialized data
	// so we expect the sizes to be the same
	assert(end - begin == sizeof(T));

	std::copy(begin, end, (BYTE*) &data);
	return;
}

template <typename Iterator>
void Serialization::deserialize(Iterator begin, Iterator end, cVector3d& data)
{
	// the iterator must point to a single byte
	static_assert(sizeof(*begin) == 1);

	// we are expecting 3 doubles
	assert(end - begin == sizeof(double) * 3);

	// get the 3 doubles from the bytes
	Iterator it = begin;
	std::copy(it, it + sizeof(double), (BYTE*) &data.x);
	it += sizeof(double);
	std::copy(it, it + sizeof(double), (BYTE*) &data.y);
	it += sizeof(double);
	std::copy(it, it + sizeof(double), (BYTE*) &data.z);
	return;
}

template <typename Iterator>
void Serialization::deserialize(Iterator begin, Iterator end, Projectile& data)
{
	// the iterator must point to a single byte
	static_assert(sizeof(*begin) == 1);

	// we are expecting 2 cVector3d
	assert(end - begin == 2 * getSerializedSize<cVector3d>());

	cVector3d temp;
	Iterator it = begin;
	
	// deserialize the position
	deserialize(it, it + getSerializedSize<cVector3d>(), temp);
	data.setPosition(temp);
	it += getSerializedSize<cVector3d>();
	
	// deserialize the speed
	deserialize(it, it + getSerializedSize<cVector3d>(), temp);
	data.setSpeed(temp);	
	return;
}

template <typename Iterator>
void Serialization::deserialize(Iterator begin, Iterator end, VideoData& data)
{
	// the iterator must point to a single byte
	static_assert(sizeof(*begin) == 1);

	// we are expecting an instance of VideoData
	assert(end - begin == getSerializedSize<VideoData>());

	Iterator it = begin;
	std::copy(it, it + sizeof(unsigned int), (BYTE*) &data.width);
	it += sizeof(unsigned int);
	std::copy(it, it + sizeof(unsigned int), (BYTE*) &data.height);
	it += sizeof(unsigned int);

	// fix this mess later
	data.rgb = const_cast<char*>((const char*) &(*it));

	return;
}

#endif