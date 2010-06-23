#ifndef BOOST_H
#define BOOST_H

// microsoft likes to deprecate stuff in the STL so we ignore those warnings
#pragma warning (disable : 4996)

//------------------------------------------------------------------------
// Boost Foreach
// http://www.boost.org/doc/libs/1_42_0/doc/html/foreach.html
//------------------------------------------------------------------------
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

//------------------------------------------------------------------------
// Boost Smart Ptr
// http://www.boost.org/doc/libs/1_43_0/libs/smart_ptr/smart_ptr.htm
//------------------------------------------------------------------------
#include <boost/scoped_array.hpp>
#include <boost/shared_ptr.hpp>

//------------------------------------------------------------------------
// Boost Iostreams
// http://www.boost.org/doc/libs/1_43_0/libs/iostreams/doc/index.html
//------------------------------------------------------------------------
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/device/back_inserter.hpp>

//------------------------------------------------------------------------
// Boost Date Time
// http://www.boost.org/doc/libs/1_43_0/doc/html/date_time.html
//------------------------------------------------------------------------
#include <boost/date_time/posix_time/posix_time.hpp>

//------------------------------------------------------------------------
// Boost Thread
// http://www.boost.org/doc/libs/1_43_0/doc/html/thread.html
//------------------------------------------------------------------------
#include <boost/thread.hpp>

//------------------------------------------------------------------------
// Boost Asio
// http://www.boost.org/doc/libs/1_43_0/doc/html/boost_asio.html
//------------------------------------------------------------------------
#include <boost/asio.hpp>

//------------------------------------------------------------------------
// Boost Program Options
// http://www.boost.org/doc/libs/1_43_0/doc/html/program_options.html
//------------------------------------------------------------------------
#include <boost/program_options.hpp>

//------------------------------------------------------------------------
// Boost String Algorithms
// http://www.boost.org/doc/libs/1_43_0/doc/html/string_algo.html
//------------------------------------------------------------------------
#include <boost/algorithm/string.hpp>

//------------------------------------------------------------------------
// Boost Serialization
// http://www.boost.org/doc/libs/1_43_0/libs/serialization/doc/index.html
//------------------------------------------------------------------------
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/binary_object.hpp>

// serialization for types that come from 3rd party libraries
#include "chai3DSerialization.h"

// stop ignoring the deprecation warning
#pragma warning (default : 4996)

#endif