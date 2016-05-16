#include "home_wnd.h"

LRESULT HomeWnd::OnInit()
{
  tab_layout_ = static_cast<CTabLayoutUI *>(m_PaintManager.FindControl(_T("table_layout")));
  return LRESULT();
}

/**
  功能：消息路由函数，这里更具消息类型路由了点击事件、左键双击事件、右键菜单事件以及DuiLib默认事件。
  参数：TNotifyUI& msg 事件消息结构体
*/
void HomeWnd::Notify(TNotifyUI& msg)
{
  if (msg.sType == _T("selectchanged"))
    OnSelectChange(msg.pSender);
  __super::Notify(msg);
}

void HomeWnd::OnUserClick(const TNotifyUI & msg)
{
}

void HomeWnd::OnSelectChange(const CControlUI *pSender)
{
  CDuiString str_selecte_name = pSender->GetName();
  (static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("home_page"))))->SetVisible(false);
  (static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("table_layout"))))->SetVisible(true);
  if (str_selecte_name == _T("house_res"))
    tab_layout_->SelectItem(0);
  else if (str_selecte_name == _T("guest_res"))
    tab_layout_->SelectItem(1);
}

void HomeWnd::Init(HWND hwnd_parent)
{
	Create(hwnd_parent, _T(""), UI_WNDSTYLE_CHILD & ~WS_VISIBLE, 0, 0, 0, 0, 0, (HMENU)(1));
	::SetWindowPos(*this, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}

void HomeWnd::GotoHomePage()
{
  (static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("table_layout"))))->SetVisible(false);
  (static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("home_page"))))->SetVisible(true);
}

