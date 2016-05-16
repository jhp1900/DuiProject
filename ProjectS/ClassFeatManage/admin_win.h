#pragma once
#include "list_menu_wnd.h"
#include "msg_header.h"

class AdminWin : public WindowImplBase
{
public:
  AdminWin();
  ~AdminWin() {}

  DECLARE_DUIWND_INFO(_T("AdminWin"), CS_DBLCLKS, _T("admin_wnd.xml"))

  BEGIN_DUIMSG_MAP(Manager)
    DUIMSG_HANDLER(kAM_AddTeacherMsg, OnAddTeacherMsg)
  END_DUIMSG_MAP()

public:
  void Init(HWND pa_hwnd);

private:
  virtual void Notify(TNotifyUI& msg) override;		// DuiLib 控件消息路由
  virtual void OnUserClick(const TNotifyUI& msg);

private:
  LRESULT OnAddTeacherMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

  void OnSelChanged(const CControlUI* pSender);
  void OnHeaderItem(const CControlUI * pSender);
  void ReloadTeacherList();    // 刷新列表内容
  void ReloadStudentList();
  void ReloadSubjectList();
  void OnEditBtn(const CControlUI * pSender);
  void OnDelBtn(const CControlUI * pSender);

private:
  HWND pa_hwnd_;
  ListMenuWnd *admin_menu_;
  PDUI_LIST teacher_list_;
  PDUI_LIST student_list_;
  PDUI_LIST college_list_;
  PDUI_LIST classes_list_;
  PDUI_LIST subject_list_;
  PDUI_VERTLAYOUT list_elem_;
};

