#pragma once
#include "window_impl_base.h"
class AddNewPlayWnd : public WindowImplBase
{
public:
  AddNewPlayWnd();
  ~AddNewPlayWnd();

  DECLARE_DUIWND_INFO(_T("AddNewPalyWnd"), CS_DBLCLKS, _T("new_play_name.xml"))

public:
  BOOL DoModal(HWND pa_hwnd);
  LPCTSTR GetNewName() { return new_name_; }

private:
  virtual void OnUserClick(const TNotifyUI& msg) override;

private:
  CDuiString new_name_;
};

