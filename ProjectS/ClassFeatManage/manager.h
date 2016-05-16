#pragma once
#include "native_control_ui.h"
#include "msg_header.h"
#include "log_wnd.h"
#include "admin_win.h"
#include "teacher_win.h"
#include "student_win.h"

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
    DUIMSG_HANDLER(kAM_LogCloseMsg, OnLogCloseMsg)
  END_DUIMSG_MAP()

  static Manager* GetInstance();
  static void DestroyInstance();

  virtual CDuiString GetSkinFolder() override { return _T("skin"); }
  virtual CDuiString GetSkinFile() override { return _T("manager.xml"); }
  virtual LPCTSTR GetWindowClassName(void) const override { return _T("Manager"); }

private:
	virtual LRESULT OnInit() override;

private:
  LRESULT OnInitOtherWndMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
  LRESULT OnLoginMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
  LRESULT OnLogCloseMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

private:
  static Manager *instance_;    // 单例模式的实例  

  LogWnd *log_wnd_;
  AdminWin *admin_win_;
  TeacherWin *teachter_win_;
  StudentWin *student_win_;
};