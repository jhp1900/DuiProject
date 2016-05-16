#pragma once
#include "window_impl_base.h"

class AddAdminWnd : public WindowImplBase
{
public:
  AddAdminWnd(HWND pa_hwnd);
  ~AddAdminWnd() {}

  DECLARE_DUIWND_INFO(_T("AddAdminWnd"), CS_DBLCLKS, _T("admin_edit_wnd.xml"))

public:
  bool DoModal();

private:
  virtual void Notify(TNotifyUI& msg) override;		// DuiLib 控件消息路由
  virtual void OnUserClick(const TNotifyUI& msg) override;

private:
  void OnComboSelect(const CControlUI * pSender);
  void OnRadioChange(const CControlUI * pSender);
  void OnClickAddBtn(const CControlUI * pSender);

private:
  HWND pa_hwnd_;
  CDuiString sex_;
};

