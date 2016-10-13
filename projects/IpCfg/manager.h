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

	BEGIN_DUINOTIFY_MAP(Manager)
		DUINOTIFY_HANDLER(_T("minbtn"), DUINOTIFY_CLICK, OnClickSysBtn)
		DUINOTIFY_HANDLER(_T("closebtn"), DUINOTIFY_CLICK, OnClickSysBtn)
		DUINOTIFY_HANDLER(_T("start_play_btn"), DUINOTIFY_CLICK, OnClickUserBtn)
		DUINOTIFY_HANDLER(_T("advanced_btn"), DUINOTIFY_CLICK, OnClickUserBtn)
		DUINOTIFY_HANDLER(_T("edit_play_btn"), DUINOTIFY_CLICK, OnClickUserBtn)
		DUINOTIFY_HANDLER(_T("update_play_btn"), DUINOTIFY_CLICK, OnClickUserBtn)
		DUINOTIFY_HANDLER(_T("del_play_btn"), DUINOTIFY_CLICK, OnClickUserBtn)
		DUINOTIFY_HANDLER(_T("add_play_btn"), DUINOTIFY_CLICK, OnClickUserBtn)
		DUINOTIFY_HANDLER(_T("test_btn"), DUINOTIFY_CLICK, OnClickUserBtn)
		DUINOTIFY_HANDLER(_T("play_list"), DUINOTIFY_ITEMSELECT, OnSelectPlay)
	END_DUINOTIFY_MAP()

public:
	virtual LRESULT OnInit() override;
	virtual CDuiString GetSkinFolder() override { return _T("skin"); }
	virtual CDuiString GetSkinFile() override { return _T("manager.xml"); }
	virtual LPCTSTR GetWindowClassName(void) const override { return _T("Manager"); }

private:
	LRESULT OnTray(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnRespondPopMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	void OnClickSysBtn(TNotifyUI &msg, bool &handled);
	void OnClickUserBtn(TNotifyUI &msg, bool &handled);
	void OnSelectPlay(TNotifyUI &msg, bool &handled);

private:
	void CreatTray();

private:
	TrayPopWnd tray_wnd_;
};