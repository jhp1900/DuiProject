#pragma once
#include "stdafx.h"
#include "window_impl_base.h"

class ListMenuWnd : public WindowImplBase//, public AutoHideWndT<ListMenuWnd>
{
public:
  ListMenuWnd(const HWND pa_hwnd) : pa_hwnd_(pa_hwnd) {}
  ~ListMenuWnd() {}

  DECLARE_DUIWND_INFO(_T("ListMenuWnd"), CS_DBLCLKS, _T("list_menu_wnd.xml"))

  BEGIN_DUIMSG_MAP(ListMenuWnd)
    DUIMSG_HANDLER(WM_SHOWWINDOW, OnShowWindow)
    DUIMSG_HANDLER(WM_DESTROY, OnDestroy)
  END_DUIMSG_MAP()

private:
  virtual void Notify(TNotifyUI& msg) override;
  virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
  virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
  virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;

private:
  LRESULT OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

  void OnItemClick(CControlUI* pSender);

public:
  void Init(POINT point);
private:
  HWND pa_hwnd_;
};

