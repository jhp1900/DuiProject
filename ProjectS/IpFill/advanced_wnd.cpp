#include "advanced_wnd.h"

AdvancedWnd::AdvancedWnd()
{
}

AdvancedWnd::~AdvancedWnd()
{
}

LRESULT AdvancedWnd::OnInit()
{
  

  return 0;
}

BOOL AdvancedWnd::DoModal(HWND pa_hwnd)
{
  if (!Create(pa_hwnd, _T(""), WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_ACCEPTFILES))
    return false;
  CenterWindow(pa_hwnd);

  return ShowModal() != 0;
}