#include "login_or_logon.h"

void LoginOrLogon::Init()
{
	Create(hwnd_parent_, _T(""), UI_WNDSTYLE_CHILD & ~WS_VISIBLE, 0, 0, 0, 0, 0, (HMENU)(0));
	::SetWindowPos(*this, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);

	login_layout_ = static_cast<CVerticalLayoutUI *>(m_PaintManager.FindControl(_T("login_layout")));
}

void LoginOrLogon::OnUserClick(const TNotifyUI& msg)
{
	if (msg.pSender->GetName() == _T("login_btn"))
		::PostMessage(hwnd_parent_, kAM_LoginMsg, 0, 0);
}



