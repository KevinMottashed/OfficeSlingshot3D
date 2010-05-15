#ifndef SERIALIZE_H
#define SERIALIZE_H

#include "stdafx.h"
#include "chai3d.h"
#include "Projectile.h"
#include "VideoData.h"
#include "boost.h"

namespace Serialization
{
	//--------------------------------------------------------------------
	// SERIALIZATION
	//--------------------------------------------------------------------

	// Templates parameters
	// T : primitive type

	// serializes a string
	void serialize(const std::string& str, boost::shared_ptr<std::vector<BYTE> >& data);

	// serializes a vector3d
	void serialize(const cVector3d& vec, boost::shared_ptr<std::vector<BYTE> >& data);

	// serializes a projectile
	void serialize(const Projectile& projectile, boost::shared_ptr<std::vector<BYTE> >& data);

	// seializes video data
	void serialize(const VideoData& video, boost::shared_ptr<std::vector<BYTE> >& data);

	// serializes a type T
	template <typename T>
	void serialize(const T& t, boost::shared_ptr<std::vector<BYTE> >& data);

	//--------------------------------------------------------------------
	// DESERIALIZATION
	//--------------------------------------------------------------------

	// deserialize a type T
	template <typename T>
	void deserialize(boost::shared_ptr<std::vector<BYTE> > data, T& t);

	// deserialize a string
	void deserialize(boost::shared_ptr<std::vector<BYTE> > data, std::string& str);

	// deserialize a vector3d
	void deserialize(boost::shared_ptr<std::vector<BYTE> > data, cVector3d& vec);

	// deserialize a projectile
	void deserialize(boost::shared_ptr<std::vector<BYTE> > data, Projectile& projectile);

	// deserialize video data
	void deserialize(boost::shared_ptr<std::vector<BYTE> > data, VideoData& video);
}

//--------------------------------------------------------------------
// TEMPLATE IMPLEMENTATIONS
//--------------------------------------------------------------------

template <typename T>
void Serialization::serialize(const T& t, boost::shared_ptr<std::vector<BYTE> >& data)
{
	data->insert(data.end(), (BYTE*) &t, ((BYTE*) &t) + sizeof(T));
	return;
}

template <typename T>
void Serialization::deserialize(boost::shared_ptr<std::vector<BYTE> > data, T& t)
{
	// for a primitive the serialized data is equal to the non-serialized data
	// so we expect the sizes to be the same
	assert(data->size() == sizeof(T));

	std::copy(data->begin(), data->end(), (BYTE*) &t);
	return;
}

#endif