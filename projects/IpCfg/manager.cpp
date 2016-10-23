#include "manager.h"
#include "resource.h"
#include "res_singleton.h"

Manager::Manager()
	: minimize_(false)
{
	ResSingleton::GetInstance();
}

Manager::~Manager()
{
	Shell_NotifyIcon(NIM_DELETE, &tray_data_);
}

LRESULT Manager::OnInit()
{
	::SendMessage(*this, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon((HINSTANCE)GetWindowLongPtr(m_hWnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDI_LOGO)));
	minimize_ = ResSingleton::GetInstance()->GetXmlMamager().GetPopAttr(_T("minimize"));
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
	switch (wParam)
	{
		case kAM_PopClickExitMsg:
			Close();
			break;
		case kAM_PopClickHomeMsg:
			ShowWindow(SW_SHOW);
			break;
		case kAM_PopSelectHotkayMsg:
			OnChangeHotkey((bool)lParam);
			break;
		case kAM_PopSelectMinMsg:
			OnChangeMinimize((bool)lParam);
			break;
	}
	return LRESULT();
}

void Manager::OnClickSysBtn(TNotifyUI & msg, bool & handled)
{
	CDuiString name = msg.pSender->GetName();
	if (name == _T("minbtn"))
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
	else if (name == _T("closebtn")) {
		if(minimize_)
			ShowWindow(SW_HIDE);
		else 
			Close();
	}
}

void Manager::OnClickUserBtn(TNotifyUI & msg, bool & handled)
{
	CDuiString name = msg.pSender->GetName();
	if (name == _T("test_btn")) {
		
	}
}

void Manager::OnSelectPlay(TNotifyUI & msg, bool & handled)
{
}

void Manager::CreatTray()
{
	tray_wnd_.CreateWithDefaltStyle(m_hWnd);

	tray_data_.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	tray_data_.hWnd = m_hWnd;
	tray_data_.uID = 1;
	tray_data_.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	tray_data_.uCallbackMessage = kAM_TrayCallbackMsg;
	tray_data_.hIcon = LoadIcon((HINSTANCE)GetWindowLongPtr(m_hWnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDI_LOGO));
	wcscpy_s(tray_data_.szTip, _T("IpCfg"));

	Shell_NotifyIcon(NIM_ADD, &tray_data_);
}

void Manager::OnChangeHotkey(bool check)
{
	ResSingleton::GetInstance()->GetXmlMamager().SetPopAttr(_T("hotkey"), check);
}

void Manager::OnChangeMinimize(bool check)
{
	minimize_ = check;
	ResSingleton::GetInstance()->GetXmlMamager().SetPopAttr(_T("minimize"), check);
}
