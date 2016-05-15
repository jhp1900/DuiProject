#pragma once
#include "window_impl_base.h"
#include "msg_header.h"

class LogWnd : public WindowImplBase
{
public:
  LogWnd(HWND pa_hwnd);
  ~LogWnd() {}

  DECLARE_DUIWND_INFO(_T("LogWnd"), CS_DBLCLKS, _T("log_wnd.xml"))

public:
  bool DoModal();

private:
  virtual void Notify(TNotifyUI& msg) override;

private:
  void ShowInit();
  void OnClick(CControlUI* pSender);
  void OnRadioChange(CControlUI* pSender);
  void OnClickLoginBtn();

private:
  HWND pa_hwnd_;
  CDuiString radio_type_;
};

