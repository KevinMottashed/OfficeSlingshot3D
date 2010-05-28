#include "stdafx.h"
#include "boost.h"
#include "Chat.h"
#include "MediatorProxy.h"
#include "NetworkProxy.h"
#include "UserInterfaceProxy.h"
#include "ZCameraProxy.h"
#include "ReplayProxy.h"

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
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	CoInitialize(NULL);
	AfxInitRichEdit();

	// create the configuration
	shared_ptr<Configuration> configuration(new Configuration("userPreferences.txt"));
	
	// Initialize the components	
	shared_ptr<Network> network(new WinsockNetwork());
	shared_ptr<MFCUserInterface> userInterface(new MFCUserInterface(configuration->getUserPreferences()));
	shared_ptr<Falcon> falcon(new NovintFalcon());

	//shared_ptr<Replayer> replayer(new Replayer("Sample.replay", configuration->getUserPreferences()));
	//shared_ptr<Replayer> network(replayer);
	//shared_ptr<Replayer> userInterface(replayer);
	//shared_ptr<Replayer> falcon(replayer);
		
	shared_ptr<IZCamera> zcamera(new ZCamera());

	// this will initialize the Mediator class and it will initialize the system
	Mediator mediator(network, falcon, zcamera, userInterface, configuration);

	CDialog* mainWindow = userInterface->getMainWindow();

	m_pMainWnd = mainWindow;

	//replayer->startReplay();

	int nResponse = mainWindow->DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

