#pragma once
#include "window_impl_base.h"

enum ShowType
{
  add_admin,
  admin_edit,
  self_edit
};

class EditTeacherWnd : public WindowImplBase
{
public:
  EditTeacherWnd(HWND pa_hwnd, ShowType show_type);
  ~EditTeacherWnd() {}

  DECLARE_DUIWND_INFO(_T("EditTeacherWnd"), CS_DBLCLKS, _T("admin_edit_wnd.xml"))

public:
  bool DoModal();

private:
  virtual void Notify(TNotifyUI& msg) override;		// DuiLib 控件消息路由
  virtual void OnUserClick(const TNotifyUI& msg) override;

private:
  void InitShow();
  void OnComboSelect(const CControlUI * pSender);
  void OnRadioChange(const CControlUI * pSender);

private:
  HWND pa_hwnd_;
  ShowType show_type_;
  CDuiString sex_;
};

