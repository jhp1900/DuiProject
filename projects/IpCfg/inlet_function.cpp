#include "manager.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	::CoInitialize(NULL);
	UNREFERENCED_PARAMETER(hPrevInstance);		// �����������뾯��,
	UNREFERENCED_PARAMETER(lpCmdLine);

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));

	Manager home_wnd;
	home_wnd.Create(NULL, _T("IpCfg"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	home_wnd.CenterWindow();
	home_wnd.ShowModal();

	::CoUninitialize();

	return 0;
}