#pragma once
#include <memory>
#include "window_impl_base.h"
#include "native_control_ui.h"
#include "msg_header.h"
#include "home_wnd.h"
#include "login_or_logon.h"

class Manager : public WindowImplBase
{
public:
	Manager() {}
	~Manager() {}

	BEGIN_DUICONTROL_CREATE(Manager)
		DUICONTROL_CREATE(_T("Panel"), NativeControlUI)
		END_DUICONTROL_CREATE()

	BEGIN_DUIMSG_MAP(Manager)
		DUIMSG_HANDLER(kAM_InitOtherWndMsg, OnInitOtherWndMsg)
    DUIMSG_HANDLER(kAM_LoginMsg, OnLoginMsg)
	END_DUIMSG_MAP()

public:
	virtual LPCTSTR GetWindowClassName() const override { return _T("Manager"); }	// 复写纯虚函数， 获取窗体类名
	virtual CDuiString GetSkinFile() override { return _T("manager.xml"); }		// 复写纯虚函数， 获取界面（.xml）文件名
	virtual CDuiString GetSkinFolder() override { return _T("skin"); }			// 复写纯虚函数， 获取界面文件路径

private:
	virtual LRESULT OnInit() override;					// 初始化窗体
	virtual void OnUserClick(const TNotifyUI& msg) override;		// 用户控件消息响应

private:
	LRESULT OnInitOtherWndMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);		// 自定义事件响应
  LRESULT OnLoginMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	void OnClickTest();			// 测试按钮响应
	void OnClickAction();		// 动作按钮响应

private:
  std::unique_ptr<LoginOrLogon> log_wnd_;
  std::unique_ptr<HomeWnd> home_wnd_;
};

