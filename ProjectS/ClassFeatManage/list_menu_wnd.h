#pragma once
#include "window_impl_base.h"

class ListMenuWnd : public WindowImplBase
{
public:
  ListMenuWnd(HWND pa_hwnd, LPCTSTR xml_file);
  ~ListMenuWnd() {}

  DECLARE_DUIWND_INFO(_T("ListMenuWnd"), CS_DBLCLKS, xml_file_)

  BEGIN_DUIMSG_MAP(ListMenuWnd)
    DUIMSG_HANDLER(WM_SHOWWINDOW, OnShowWindow)
    DUIMSG_HANDLER(WM_DESTROY, OnDestroy)
  END_DUIMSG_MAP()

private:
  virtual void Notify(TNotifyUI& msg) override;
  virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
  virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
  virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;

public:
  void Init(POINT point);
  void Reload(POINT point);

private:
  LRESULT OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  void OnItemClick(CControlUI* pSender);

private:
  HWND pa_hwnd_;
  LPCTSTR xml_file_;
};

