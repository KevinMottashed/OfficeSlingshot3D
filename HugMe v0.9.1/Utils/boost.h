#ifndef BOOST_H
#define BOOST_H

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
// microsoft STL deprecation strikes again
#pragma warning (disable : 4996)
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/concepts.hpp>
#pragma warning (default : 4996)

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

#endif