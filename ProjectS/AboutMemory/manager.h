#pragma once
#include "window_impl_base.h"
#include "native_control_ui.h"
#include "msg_header.h"
#include "login_or_logon.h"
#include "home_wnd.h"
#include "user_data_manage.h"

class Manager : public WindowImplBase
{
public:
  Manager();
	~Manager() {}

  BEGIN_DUICONTROL_CREATE(Manager)
    DUICONTROL_CREATE(_T("Panel"), NativeControlUI)
    END_DUICONTROL_CREATE()

  BEGIN_DUIMSG_MAP(Manager)
    DUIMSG_HANDLER(kAM_InitOtherWndMsg, OnInitOtherWndMsg)
    DUIMSG_HANDLER(kAM_LoginMsg, OnLoginMsg)
  END_DUIMSG_MAP()

  static Manager* GetInstance();
  static void DestroyInstance();

  virtual CDuiString GetSkinFolder() override { return _T("skin"); }
  virtual CDuiString GetSkinFile() override { return _T("manager.xml"); }
  virtual LPCTSTR GetWindowClassName(void) const override { return _T("Manager"); }

public:
  void SetOwner(LPCTSTR owner) { owner_ = owner; }
  LPCTSTR GetOwner() { return owner_; }
  void SetUserInfo(DataFileInfo user_info) { user_info_ = user_info; }
  DataFileInfo GetUserInfo() const { return user_info_; }

private:
	virtual LRESULT OnInit() override;

private:
  LRESULT OnInitOtherWndMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
  LRESULT OnLoginMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

private:
  static Manager *instance_;    // ����ģʽ��ʵ��  
  LoginOrLogon *log_wnd_;
  HomeWnd *home_wnd_;

private:    // ĳʱ��Ψһ���ڵĹ�����Դ
  DataFileInfo user_info_;    // ��½�û�������Ϣ
  CDuiString owner_;  // ��ǰ�����ļ����������
  vector<CDuiString> vector_owner_;   // ��ǰ�����ļ��������Ⱥ
};