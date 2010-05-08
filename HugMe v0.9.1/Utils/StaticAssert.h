#ifndef STATIC_ASSERT_H
#define STATIC_ASSERT_H

#include "stdafx.h"

// Utility for compile time assertions
// usage: static_assert(bool);
// example: static_assert(1 == 2) will cause the compilation to fail

template <bool> struct StaticAssert;
template <> struct StaticAssert<true> {};

#define static_assert(expr) {\
	(void)(sizeof(StaticAssert<expr != 0>));\
	}

#endif