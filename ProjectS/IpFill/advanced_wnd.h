#pragma once
#include "window_impl_base.h"

class AdvancedWnd : public WindowImplBase
{
public:
  AdvancedWnd();
  AdvancedWnd(LPCTSTR play_name);
  ~AdvancedWnd();

  DECLARE_DUIWND_INFO(_T("AdvancedWnd"), CS_DBLCLKS, _T("advanced_wnd.xml"))

private:
  virtual LRESULT OnInit() override;
  virtual void OnUserClick(const TNotifyUI& msg) override;

public:
  BOOL DoModal(HWND pa_hwnd);

private:
  void FlushList();

private:
  CDuiString play_name_;
  vector<CDuiString> ip_;
  vector<CDuiString> netmask_;
};

