#ifndef CHAI3D_SERIALIZATION_H
#define CHAI3D_SERIALIZATION_H

#include "stdafx.h"
#include "boost.h"
#include "chai3d.h"

// This header provides serialization for all chai3d types
// that we need to transmit over the network or need save to
// a replay file.

namespace boost 
{
	namespace serialization 
	{
		template <typename Archive>
		void serialize(Archive& ar, cVector3d& vector, const unsigned int version)
		{
			ar & vector.x;
			ar & vector.y;
			ar & vector.z;
			return;
		}
	}
}

#endif;