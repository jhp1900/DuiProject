#pragma once
#include "window_impl_base.h"
#include "msg_header.h"

class LoginOrLogon : public WindowImplBase
{
public:
	LoginOrLogon(HWND hwnd_parent) : hwnd_parent_(hwnd_parent) {}
  ~LoginOrLogon() {}

	DECLARE_DUIWND_INFO(_T("LoginOrLogon"), CS_DBLCLKS, _T("log_wnd.xml"))

public:
	void Init();

private:
	virtual void OnUserClick(const TNotifyUI& msg) override;

private:
	HWND hwnd_parent_;
	CVerticalLayoutUI *login_layout_;
};


