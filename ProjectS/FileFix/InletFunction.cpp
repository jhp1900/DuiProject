#include "HomePageWnd.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	::CoInitialize(NULL);
	UNREFERENCED_PARAMETER(hPrevInstance);		// 用来消除编译警告,
	UNREFERENCED_PARAMETER(lpCmdLine);

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));

	Manager *home_wnd = new Manager(TEXT("file_fix.xml"));
	home_wnd->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	home_wnd->CenterWindow();
	home_wnd->ShowModal();

	delete home_wnd;
	::CoUninitialize();

	return 0;
}