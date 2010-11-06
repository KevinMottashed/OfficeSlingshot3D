#include "UserPreferences.h"

#include "boost.h"

const std::string UserPreferences::default_ip_address = "127.0.0.1";
const std::string UserPreferences::default_name = "Player";
const unsigned int UserPreferences::default_arm_band_port = 3;
const unsigned int UserPreferences::default_jacket_port = 97;
const bool UserPreferences::default_muted = false;
const float UserPreferences::default_volume = 5.0f;

void UserPreferences::setDefaults()
{
	ipAddress = default_ip_address;
	name = default_name;
	armBandPort = default_arm_band_port;
	jacketPort = default_jacket_port;
	muted = default_muted;
	volume = default_volume;
	return;
}

std::ostream& operator<<(std::ostream& os, const UserPreferences& pref)
{
	os << "IpAddress=" << pref.ipAddress << std::endl;
	os << "Name=" << pref.name << std::endl;
	os << "ArmBandPort=" << pref.armBandPort << std::endl;
	os << "JacketPort=" << pref.jacketPort << std::endl;
	os << "Muted=" << (pref.muted ? "1" : "0") << std::endl;
	os << "Volume=" << pref.volume << std::endl;
	return os;
}

std::istream& operator>>(std::istream& is, UserPreferences& pref)
{
	// create a namespace alias to make things easier
	namespace po = boost::program_options;

	po::options_description preferencesOptions("Allowed Options");
	preferencesOptions.add_options()
		("IpAddress",		po::value<string>(),		"The default IP to connect to")
		("Name",			po::value<string>(),		"The player's name")
		("ArmBandPort",		po::value<unsigned int>(),	"The blue tooth port for the armband")
		("JacketPort",		po::value<unsigned int>(),	"The blue tooth port for the jacket")
		("Muted",			po::value<unsigned int>(),	"True if the application should be muted")
		("Volume",			po::value<float>(),			"The volume on a scale of 0 to 1");

	// the variable map for this configuration
	po::variables_map configVMap;
	
	try
	{
		// parse the file to fill the variable map
		po::store(po::parse_config_file(is, preferencesOptions), configVMap);
	}
	catch (po::error&)
	{
		// malformed config file, set defaults and return
		pref.setDefaults();
		return is;
	}
	
	po::notify(configVMap);

	
	if (configVMap.count("IpAddress") == 0 ||
		configVMap.count("Name") == 0 ||
		configVMap.count("ArmBandPort") == 0 ||
		configVMap.count("JacketPort") == 0 || 
		configVMap.count("Muted") == 0 ||
		configVMap.count("Volume") == 0)
	{
		// missing option, set the defaults and return
		pref.setDefaults();
		return is;
	}

	pref.ipAddress = configVMap["IpAddress"].as<string>();
	pref.name = configVMap["Name"].as<string>();
	pref.armBandPort = configVMap["ArmBandPort"].as<unsigned int>();
	pref.jacketPort = configVMap["JacketPort"].as<unsigned int>();
	pref.muted = (configVMap["Muted"].as<unsigned int>() == 1);
	pref.volume = configVMap["Volume"].as<float>();
	return is;
}

