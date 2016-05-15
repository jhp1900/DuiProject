#include "list_menu_wnd.h"
#include "add_admin_wnd.h"

ListMenuWnd::ListMenuWnd(HWND pa_hwnd, LPCTSTR xml_file)
  : pa_hwnd_(pa_hwnd)
  , xml_file_(xml_file)
{
}

void ListMenuWnd::Init(POINT point)
{
  Create(pa_hwnd_, _T(""), WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 0, 0, 0, (HMENU)(0));
  Reload(point);
}

void ListMenuWnd::Reload(POINT point)
{
  ::ClientToScreen(pa_hwnd_, &point);  // 将鼠标事件坐标从客户区坐标系换算到屏幕坐标系
  ::SetWindowPos(*this, NULL, point.x, point.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
  this->ShowWindow(true);
}

void ListMenuWnd::Notify(TNotifyUI& msg)
{
  if (msg.sType == _T("itemclick"))
    OnItemClick(msg.pSender);

  __super::Notify(msg);
}

LRESULT ListMenuWnd::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
  if (wParam == VK_ESCAPE)
    this->ShowWindow(SW_HIDE);
  bHandled = TRUE;
  return 0;
}

LRESULT ListMenuWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
  this->ShowWindow(SW_HIDE);
  bHandled = FALSE;
  return 0;
}

LRESULT ListMenuWnd::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
  return LRESULT();
}

void ListMenuWnd::OnItemClick(CControlUI * pSender)
{
  if (pSender->GetName() == _T("admin_add")) {
    AddAdminWnd add_admin(pa_hwnd_);
    add_admin.DoModal();
  } else if (pSender->GetName() == _T("")) {

  }

  this->ShowWindow(SW_HIDE);
}

LRESULT ListMenuWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
  return LRESULT();
}
