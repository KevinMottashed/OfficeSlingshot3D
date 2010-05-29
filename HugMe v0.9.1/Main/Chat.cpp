#include "stdafx.h"
#include "boost.h"
#include "Chat.h"
#include "OfficeSlingshot3D.h"
#include "OfficeSlingshot3DFactory.h"

using namespace std;
using namespace boost;

/////////////////////////////////////////////////////////////////////////////
// CChatApp

BEGIN_MESSAGE_MAP(CChatApp, CWinApp)
	//{{AFX_MSG_MAP(CChatApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatApp construction

CChatApp::CChatApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CChatApp object

CChatApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CChatApp initialization

BOOL CChatApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return EXIT_FAILURE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	CoInitialize(NULL);
	AfxInitRichEdit();

	// the file name for the configuration
	string configFileName;

	namespace po = boost::program_options;

	po::options_description cmdLineOptions("Allowed Options");
	cmdLineOptions.add_options()
		("config", po::value<string>(&configFileName)->default_value("config.ini"), "configuration file name");

	// convert the windows style command arguments to a vector of arguments
	vector<string> args = po::split_winmain(m_lpCmdLine);	

	// create a variable map and fill it in by parsing the command line options
	po::variables_map cmdVMap;
	po::store(po::command_line_parser(args).options(cmdLineOptions).run(), cmdVMap);
	po::notify(cmdVMap);

	// create the app from the config file
	shared_ptr<OfficeSlingshot3D> app = OfficeSlingshot3DFactory::createFromConfigFile(configFileName);

	// run the app
	app->run();

	// the app has finished (dialog box closed)
	return EXIT_SUCCESS;
}

