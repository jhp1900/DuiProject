#pragma once
#include "window_impl_base.h"
#include "ip_control_ui.h"
#include "msg_head.h"
#include "tray_pop_wnd.h"

class Manager : public WindowImplBase
{
public:
	Manager();
	~Manager() {}

	BEGIN_DUICONTROL_CREATE(Manager)
		DUICONTROL_CREATE_FROM_XML(_T("IpControl"), _T("ip_control.xml"))
		DUICONTROL_CREATE(_T("IpControlLayout"), IpControlUI)
	END_DUICONTROL_CREATE()

	BEGIN_DUIMSG_MAP(Manager)
		DUIMSG_HANDLER(kAM_TrayCallbackMsg, OnTray)
		DUIMSG_HANDLER(kAM_TrayPopMsg, OnRespondPopMsg)
	END_DUIMSG_MAP()

public:
	virtual LRESULT OnInit() override;
	virtual CDuiString GetSkinFolder() override { return _T("skin"); }
	virtual CDuiString GetSkinFile() override { return _T("manager.xml"); }
	virtual LPCTSTR GetWindowClassName(void) const override { return _T("Manager"); }

private:
	LRESULT OnTray(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnRespondPopMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	void CreatTray();

private:
	TrayPopWnd tray_wnd_;
};