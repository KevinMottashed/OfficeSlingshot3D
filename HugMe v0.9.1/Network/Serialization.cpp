#include "Serialization.h"

using namespace std;
using namespace boost;

static const unsigned int cVector3d_serialized_size = 3 * sizeof(double);
static const unsigned int projectile_serialized_size = 2 * cVector3d_serialized_size;
static const unsigned int videoData_serialized_size = IMAGE_ARRAY_SIZE;

// the cVector3d will be serialized as <x><y><z>
void Serialization::serialize(const cVector3d& vec, shared_ptr<vector<BYTE> >& data)
{
	data->insert(data->end(), (BYTE*) &vec.x, ((BYTE*) &vec.x) + sizeof(vec.x));
	data->insert(data->end(), (BYTE*) &vec.y, ((BYTE*) &vec.y) + sizeof(vec.y));
	data->insert(data->end(), (BYTE*) &vec.z, ((BYTE*) &vec.z) + sizeof(vec.z));
	return;
}

// the projectile will be serialized as <position><speed>
void Serialization::serialize(const Projectile& projectile, shared_ptr<vector<BYTE> >& data)
{
	serialize(projectile.getPosition(), data);
	serialize(projectile.getSpeed(), data);
	return;
}

void Serialization::serialize(const VideoData& video, shared_ptr<vector<BYTE> >& data)
{
	data = video.rgb;
	return;
}

void Serialization::serialize(const string& str, shared_ptr<vector<BYTE> >& data)
{
	data->insert(data->end(), str.begin(), str.end());
	return;
}

void Serialization::deserialize(shared_ptr<vector<BYTE> > data, string& str)
{
	str = "";
	str.insert(str.end(), data->begin(), data->end());
	return;
}

void Serialization::deserialize(shared_ptr<vector<BYTE> > data, cVector3d& vec)
{
	// we are expecting 3 doubles
	assert(data->size() == cVector3d_serialized_size);

	// get the 3 doubles from the bytes
	vector<BYTE>::const_iterator it = data->begin();
	copy(it, it + sizeof(double), (BYTE*) &vec.x);
	it += sizeof(double);
	copy(it, it + sizeof(double), (BYTE*) &vec.y);
	it += sizeof(double);
	copy(it, it + sizeof(double), (BYTE*) &vec.z);
	return;
}

void Serialization::deserialize(shared_ptr<vector<BYTE> > data, Projectile& projectile)
{
	// we are expecting 2 cVector3d
	assert(data->size() == projectile_serialized_size);

	cVector3d temp;
	vector<BYTE>::const_iterator it = data->begin();
	
	// deserialize the position
	copy(it, it + sizeof(double), (BYTE*) &temp.x);
	it += sizeof(double);
	copy(it, it + sizeof(double), (BYTE*) &temp.y);
	it += sizeof(double);
	copy(it, it + sizeof(double), (BYTE*) &temp.z);
	it += sizeof(double);
	projectile.setPosition(temp);
	
	// deserialize the speed
	copy(it, it + sizeof(double), (BYTE*) &temp.x);
	it += sizeof(double);
	copy(it, it + sizeof(double), (BYTE*) &temp.y);
	it += sizeof(double);
	copy(it, it + sizeof(double), (BYTE*) &temp.z);
	it += sizeof(double);
	projectile.setSpeed(temp);

	return;
}

void Serialization::deserialize(shared_ptr<vector<BYTE> > data, VideoData& video)
{
	// we are expecting an instance of VideoData
	assert(data->size() == videoData_serialized_size);

	// get the video
	video.rgb = data;

	return;
}

