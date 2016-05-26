#pragma once
#include "window_impl_base.h"

class AdvancedWnd : public WindowImplBase, public IListCallbackUI
{
public:
  AdvancedWnd();
  AdvancedWnd(LPCTSTR play_name);
  ~AdvancedWnd();

  DECLARE_DUIWND_INFO(_T("AdvancedWnd"), CS_DBLCLKS, _T("advanced_wnd.xml"))

private:
  virtual LRESULT OnInit() override;
  virtual CDuiString GetItemText(CControlUI* pList, int iItem, int iSubItem) override;

public:
  BOOL DoModal(HWND pa_hwnd);

private:
  CDuiString play_name_;
  vector<CDuiString> ip_;
  vector<CDuiString> netmask_;
};

