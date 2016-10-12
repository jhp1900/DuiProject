#include "manager.h"
#include "resource.h"
#include "res_singleton.h"
#include <shellapi.h>

Manager::Manager()
{
	ResSingleton::GetInstance();
}

LRESULT Manager::OnInit()
{
	::SendMessage(*this, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon((HINSTANCE)GetWindowLongPtr(m_hWnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDI_LOGO)));

	CreatTray();

	return 0;
}

LRESULT Manager::OnTray(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	if (wParam != 1)
		return 0;

	switch (lParam) {
		case WM_LBUTTONUP:
			ShowWindow(SW_SHOW);
			break;
		case WM_RBUTTONUP:
			LPPOINT lpoint = new tagPOINT;
			::GetCursorPos(lpoint);
			tray_wnd_.PopupWindow(lpoint);
			break;
	}

	return LRESULT();
}

LRESULT Manager::OnRespondPopMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	return LRESULT();
}

void Manager::CreatTray()
{
	tray_wnd_.CreateWithDefaltStyle(m_hWnd);

	NOTIFYICONDATA tray_data;
	tray_data.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	tray_data.hWnd = m_hWnd;
	tray_data.uID = 1;
	tray_data.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	tray_data.uCallbackMessage = kAM_TrayCallbackMsg;
	tray_data.hIcon = LoadIcon((HINSTANCE)GetWindowLongPtr(m_hWnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDI_LOGO));
	wcscpy_s(tray_data.szTip, _T("IpCfg"));

	Shell_NotifyIcon(NIM_ADD, &tray_data);
}

