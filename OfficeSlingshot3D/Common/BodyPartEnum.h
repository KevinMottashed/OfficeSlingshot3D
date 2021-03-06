#ifndef BODY_PART_ENUM_H
#define BODY_PART_ENUM_H

#include "stdafx.h"

/**
 * Namespace for holding the BodyPart enum.
 * This prevents naming conflicts.
 */
namespace BodyPart
{
	/**
	 * @ingroup Common
	 * @b public
	 * Enum representing different parts of the human body.
	 */
	enum BodyPartEnum
	{
		HEAD, /**< Head */
		CHEST, /**< Chest */
		UPPER_RIGHT_ARM, /**< Upper right arm, shoulder to elbow */
		UPPER_LEFT_ARM, /**< Upper left arm, shoulder to elbow */
	};

	/**
	 * Get a string representation of the enum
	 * @param[in] e The enum to convert to a string
	 * @return The string representation
	 */
	std::string lookup(BodyPartEnum e);
}

/**
 * Typedef for convinience.
 */
typedef BodyPart::BodyPartEnum BodyPart_t;

#endif