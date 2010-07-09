#include "OfficeSlingshot3DFactory.h"
#include "NetworkProxy.h"
#include "UserInterfaceProxy.h"
#include "FalconProxy.h"
#include "ZCameraProxy.h"

using namespace std;
using namespace boost;

shared_ptr<OfficeSlingshot3D> OfficeSlingshot3DFactory::createFromConfigFile(const string& fileName)
{
	// open up the config file
	ifstream configFile(fileName.c_str());
	if (!configFile.is_open())
	{
		// we can't create from a non-existant file
		assert(false);
		return shared_ptr<OfficeSlingshot3D>();
	}

	// the different components which may or may not be needed to create the OfficeSlingshot3D instance
	boost::shared_ptr<Mediator> mediator; // must initialize
	boost::shared_ptr<Logger> logger; // may initialize
	boost::shared_ptr<Replayer> replayer; // may initialize
	CDialog* dialog(NULL); // must initialize

	// create a namespace alias to make things easier
	namespace po = boost::program_options;

	// define the different configuration options
	// note that nothing is optional, so the file must contain all options,
	// unless the option is not applicable
	// for example if Logger.Enabled = 0 then Logger.File doesn't have to be in the config file
	po::options_description configFileOptions("Allowed Options");
	configFileOptions.add_options()
		("UserPreferences.File",	po::value<string>(),	"the file name for the user preferences")
		("Logger.Enabled",			po::value<int>(),		"0 to disable the logger, 1 to enable it")
		("Logger.Format",			po::value<string>(),	"Human/Replay")
		("Logger.Output",			po::value<string>(),	"File/Console")
		("Logger.File",				po::value<string>(),	"File Name for the output")
		("Replayer.Enabled",		po::value<int>(),		"0 to disable the replayer, 1 to enable it")
		("Replayer.File",			po::value<string>(),	"The name of the replay file")
		("Replayer.ReplayNetwork",	po::value<int>(),		"1 to replay network, 0 otherwise")
		("Replayer.ReplayUI",		po::value<int>(),		"1 to replay UI, 0 otherwise")
		("Replayer.ReplayFalcon",	po::value<int>(),		"1 to replay falcon, 0 otherwise")
		("Replayer.ReplayZCamera",	po::value<int>(),		"1 to replay zcamera, 0 otherwise");

	// the variable map for this configuration
	po::variables_map configVMap;

	// parse the file to fill the variable map
	po::store(po::parse_config_file(configFile, configFileOptions), configVMap);
	po::notify(configVMap);

	if (!configVMap.count("UserPreferences.File"))
	{
		// the user preferences file was not specified
		assert(false);
		return shared_ptr<OfficeSlingshot3D>();
	}

	// create the Configuration object
	string userPreferencesFile = configVMap["UserPreferences.File"].as<string>();
	shared_ptr<Configuration> configuration(new Configuration(userPreferencesFile));

	if (!configVMap.count("Replayer.Enabled"))
	{
		// missing attribute
		assert(false);
		return shared_ptr<OfficeSlingshot3D>();
	}

	// we need all these to create the mediator
	boost::shared_ptr<Network> network;
	boost::shared_ptr<MFCUserInterface> userInterface;
	boost::shared_ptr<Falcon> falcon;
	boost::shared_ptr<IZCamera> zcamera;

	// check if the user wants a replayer
	if (configVMap["Replayer.Enabled"].as<int>())
	{
		// the replayer is enabled, proceed to create it
		
		// all these attributes must now be present
		if (configVMap.count("Replayer.File")			== 0 ||
			configVMap.count("Replayer.ReplayNetwork")	== 0 ||
			configVMap.count("Replayer.ReplayUI")		== 0 ||
			configVMap.count("Replayer.ReplayFalcon")	== 0 ||
			configVMap.count("Replayer.ReplayZCamera")	== 0)
		{
			// missing attribute
			assert(false);
			return shared_ptr<OfficeSlingshot3D>();
		}

		// create the replayer
		replayer = shared_ptr<Replayer>(new Replayer(configVMap["Replayer.File"].as<string>(), configuration->getUserPreferences()));

		if (configVMap["Replayer.ReplayNetwork"].as<int>())
		{
			// the user wants to replay network events
			network = replayer->getNetworkReplayer();
		}

		if (configVMap["Replayer.ReplayUI"].as<int>())
		{
			// the user wants to replay UI events
			userInterface = replayer->getUserInterfaceReplayer();
		}

		if (configVMap["Replayer.ReplayFalcon"].as<int>())
		{
			// the user wants to replay falcon events
			falcon = replayer->getFalconReplayer();
		}

		if (configVMap["Replayer.ReplayZCamera"].as<int>())
		{
			// the user wants to replay zcamera events
			zcamera = replayer->getZCameraReplayer();
		}
	}

	// All the mediator components that have not be initialized will now be initialized
	// A component is only already initialized if it is being replayed
	if (!network)
	{
		network = shared_ptr<Network>(new WinsockNetwork());
	}

	if (!userInterface)
	{
		userInterface = shared_ptr<MFCUserInterface>(new MFCUserInterface(configuration->getUserPreferences()));
	}

	if (!falcon)
	{
		falcon = shared_ptr<Falcon>(new NovintFalcon());
	}

	if (!zcamera)
	{
		zcamera = shared_ptr<IZCamera>(new ZCamera());
	}

	// we now have enough stuff to create the mediator
	mediator = shared_ptr<Mediator>(new Mediator(network, falcon, zcamera, userInterface, configuration));

	// check if the user wants a logger
	if (configVMap["Logger.Enabled"].as<int>())
	{
		// The user wants a logger

		// all these attributes must now be present
		if (configVMap.count("Logger.Format") == 0 ||
			configVMap.count("Logger.Output") == 0)
		{
			// missing attribute
			assert(false);
			return shared_ptr<OfficeSlingshot3D>();
		}

		// determines what kind of log the user wants
		bool logToFile;
		bool logWithHumanOutput;

		// the name of the log file, might not be applicable
		string logFileName;

		// find out if the user wants to log to file or to console
		// use case insensitive comparisons for a more user friendly log file	
		if (iequals(configVMap["Logger.Output"].as<string>(),"File"))
		{
			// the user wants the log to go to a file
			// the file attribute must now exists
			if (configVMap.count("Logger.File")	== 0)
			{
				// missing attribute
				assert(false);
				return shared_ptr<OfficeSlingshot3D>();
			}

			// grab the log file name
			logFileName = configVMap["Logger.File"].as<string>();

			// the user wants to log to a file
			logToFile = true;
		}
		else if (iequals(configVMap["Logger.Output"].as<string>(),"Console"))
		{
			// the user wants to log to console
			logToFile = false;
		}
		else
		{
			// invalid attribute value
			assert(false);
			return shared_ptr<OfficeSlingshot3D>();
		}

		// find out if the user wants to log in a human readable format or the replay format
		// use case insensitive comparisons for a more user friendly config file
		if (iequals(configVMap["Logger.Format"].as<string>(),"Human"))
		{
			// the user wants human readable output
			logWithHumanOutput = true;
		}
		else if (iequals(configVMap["Logger.Format"].as<string>(),"Replay"))
		{
			// the user wants replayer readable output
			logWithHumanOutput = false;
		}
		else
		{
			// invalid attribute value
			assert(false);
			return shared_ptr<OfficeSlingshot3D>();
		}

		if (logToFile && logWithHumanOutput)
		{
			// the user wants a human logger to file
			logger = shared_ptr<Logger>(new HumanFormatFileLogger(logFileName.c_str()));
		}
		else if (logToFile && !logWithHumanOutput)
		{
			// the user wants a replay logger to file
			// note that the file must be opened in binary mode
			logger = shared_ptr<Logger>(new ReplayFormatFileLogger(logFileName.c_str(), ios::out | ios::binary));
		}
		else if (!logToFile && logWithHumanOutput)
		{
			// the user wants a human logger to console
			logger = shared_ptr<Logger>(new HumanFormatConsoleLogger());
		}
		else if (!logToFile && !logWithHumanOutput)
		{
			// the user wants a replay logger to console
			logger = shared_ptr<Logger>(new ReplayFormatConsoleLogger());
		}

		// attach the logger to the components
		network->attach(logger.get());
		userInterface->attach(logger.get());
		falcon->attach(logger.get());
		zcamera->attach(logger.get());
	}

	// grab the dialog
	dialog = userInterface->getMainWindow();

	// we now finally have everything we need to create a OfficeSlingshot3D object
	return shared_ptr<OfficeSlingshot3D>(new OfficeSlingshot3D(mediator, logger, replayer, dialog));
}
