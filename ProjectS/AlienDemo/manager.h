#pragma once
#include "window_impl_base.h"

class Manager : public WindowImplBase
{
public:
	Manager() {}
	~Manager() {}

  virtual CDuiString GetSkinFolder() override { return _T("skin"); }
  virtual CDuiString GetSkinFile() override { return _T("manager.xml"); }
  virtual LPCTSTR GetWindowClassName(void) const override { return _T("Manager"); }

private:
	virtual LRESULT OnInit() override;

private:
  LRESULT OnInitOtherWndMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
  LRESULT OnLoginMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
private:
};