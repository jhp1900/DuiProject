#pragma once
#include "window_impl_base.h"
#include "ip_control_ui.h"

class EditMoreWnd : public WindowImplBase
{
public:
  EditMoreWnd(LPCTSTR play_name);
  EditMoreWnd(LPCTSTR play_name, int index);
  ~EditMoreWnd();

  DECLARE_DUIWND_INFO(_T("EditMoreWnd"), CS_DBLCLKS, _T("edit_more_wnd.xml"))

  BEGIN_DUICONTROL_CREATE(EditMoreWnd)
    DUICONTROL_CREATE_FROM_XML(_T("IpControl"), _T("ip_control.xml"))
    DUICONTROL_CREATE(_T("IpControlLayout"), IpControlUI)
    END_DUICONTROL_CREATE()

private:
  virtual void OnUserClick(const TNotifyUI& msg) override;
  virtual LRESULT OnInit() override;

public:
  BOOL DoModal(HWND pa_hwnd);
  BOOL GetFlag() { return flag_; }

private:
  void OnClickOKBtn();

private:
  CDuiString play_name_;    // 当前方案名
  int index_;
  CDuiString ipaddr_;       // ip地址，
  CDuiString netmask_;      // 子网掩码
  BOOL flag_;               // 标识位，新增了IP或修改了IP信息，为 TRUE； 反之为 FALSE；
  BOOL type_is_add_;        // 类型标识，为TRUE表示为添加类型； 反之为编辑类型；
};
