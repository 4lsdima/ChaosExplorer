#include "wx/wxprec.h"
#include <string>
#include "GL/glew.h"
#include "ChaosExplorerApp.h"
#include "ChaosExplorerWindow.h"

#ifdef _UNICODE
#ifdef NDEBUG
#pragma comment(lib, "wxbase31u.lib")
#else
#pragma comment(lib, "wxbase31ud.lib")
#endif
#else
#ifndef NDEBUG
#pragma comment(lib, "wxbase31.lib")
#else
#pragma comment(lib, "wxbase31d.lib")
#endif
#endif

#pragma comment(lib, "glew32.lib")


ChaosExplorerApp::ChaosExplorerApp()
{
}


ChaosExplorerApp::~ChaosExplorerApp()
{
}

bool ChaosExplorerApp::OnInit()
{
    try {
        ChaosExplorerWindow* mainFrame = new ChaosExplorerWindow(nullptr, wxID_ANY, L"ChaosExplorer");
        mainFrame->Show(true);
    }
    catch (std::exception& e) {
        std::string error = std::string(e.what()) + "\nProgram will terminate.";
        wxMessageBox(error.c_str(), "Error");
    }
    return true;

}

wxIMPLEMENT_APP(ChaosExplorerApp);
