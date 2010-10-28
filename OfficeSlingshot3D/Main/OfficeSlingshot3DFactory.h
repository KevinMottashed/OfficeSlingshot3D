#ifndef OFFICE_SLINGSHOT_3D_FACTORY_H
#define OFFICE_SLINGSHOT_3D_FACTORY_H

#include "stdafx.h"
#include "boost.h"
#include "OfficeSlingshot3D.h"

/**
 * A factory to create instances of OfficeSlingshot3D
 */
class OfficeSlingshot3DFactory
{
public:
	/**
	 * Create an instance of OfficeSlingshot3D from a configuration file
	 * see ReadMe.txt for the format of the configuration file
	 * @param fileName The configuration file name
	 * @return a reference to the OfficeSlingshot3D object
	 */
	static boost::shared_ptr<OfficeSlingshot3D> createFromConfigFile(const std::string& fileName);
};

#endif