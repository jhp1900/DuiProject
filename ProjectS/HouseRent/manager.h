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
	virtual LPCTSTR GetWindowClassName() const override { return _T("Manager"); }	// ��д���麯���� ��ȡ��������
	virtual CDuiString GetSkinFile() override { return _T("manager.xml"); }		// ��д���麯���� ��ȡ���棨.xml���ļ���
	virtual CDuiString GetSkinFolder() override { return _T("skin"); }			// ��д���麯���� ��ȡ�����ļ�·��

private:
	virtual LRESULT OnInit() override;					// ��ʼ������
	virtual void OnUserClick(const TNotifyUI& msg) override;		// �û��ؼ���Ϣ��Ӧ

private:
	LRESULT OnInitOtherWndMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);		// �Զ����¼���Ӧ
  LRESULT OnLoginMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	void OnClickTest();			// ���԰�ť��Ӧ
	void OnClickAction();		// ������ť��Ӧ

private:
  std::unique_ptr<LoginOrLogon> log_wnd_;
  std::unique_ptr<HomeWnd> home_wnd_;
};

