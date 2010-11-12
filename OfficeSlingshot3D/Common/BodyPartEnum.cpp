#include "BodyPartEnum.h"

std::string BodyPart::lookup(BodyPart::BodyPartEnum e)
{
	switch (e)
	{
		case BodyPart::HEAD:
		{
			return "head";
		}
		case BodyPart::CHEST:
		{
			return "chest";
		}
		case BodyPart::UPPER_RIGHT_ARM:
		{
			return "upper right arm";
		}
		case BodyPart::UPPER_LEFT_ARM:
		{
			return "upper left arm";
		}
	}
	return "unknown";
}