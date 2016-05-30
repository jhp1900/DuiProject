#include "edit_more_wnd.h"



EditMoreWnd::EditMoreWnd()
{
}


EditMoreWnd::~EditMoreWnd()
{
}

BOOL EditMoreWnd::DoModal(HWND pa_hwnd)
{
  if (!Create(pa_hwnd, _T(""), WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_ACCEPTFILES))
    return false;
  CenterWindow(pa_hwnd);

  return ShowModal() != 0;
}

void EditMoreWnd::OnUserClick(const TNotifyUI & msg)
{
  if (msg.pSender->GetName() == _T("")) {

  }
}
