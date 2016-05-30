#pragma once
#include "window_impl_base.h"
#include "ip_control_ui.h"

class EditMoreWnd : public WindowImplBase
{
public:
  EditMoreWnd();
  ~EditMoreWnd();

  DECLARE_DUIWND_INFO(_T("EditMoreWnd"), CS_DBLCLKS, _T("edit_more_wnd.xml"))

  BEGIN_DUICONTROL_CREATE(EditMoreWnd)
    DUICONTROL_CREATE_FROM_XML(_T("IpControl"), _T("ip_control.xml"))
    DUICONTROL_CREATE(_T("IpControlLayout"), IpControlUI)
    END_DUICONTROL_CREATE()

public:
  BOOL DoModal(HWND pa_hwnd);

private:
  virtual void OnUserClick(const TNotifyUI& msg) override;
};
