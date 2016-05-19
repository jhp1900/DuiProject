#pragma once
#include "window_impl_base.h"

class AdvancedWnd : public WindowImplBase
{
public:
  AdvancedWnd();
  ~AdvancedWnd();

  DECLARE_DUIWND_INFO(_T("AdvancedWnd"), CS_DBLCLKS, _T("advanced_wnd.xml"))

private:
  virtual LRESULT OnInit() override;

public:
  BOOL DoModal(HWND pa_hwnd);
};

