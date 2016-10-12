#include "tray_pop_wnd.h"

TrayPopWnd::TrayPopWnd()
	: pa_hwnd_(0)
{
}

TrayPopWnd::~TrayPopWnd()
{
}

LRESULT TrayPopWnd::OnInit()
{
	return 0;
}

HWND TrayPopWnd::CreateWithStyle(HWND pa_hwnd, DWORD style)
{
	pa_hwnd_ = pa_hwnd;
	return Create(pa_hwnd_, nullptr, style, 0);
}

HWND TrayPopWnd::CreateWithDefaltStyle(HWND pa_hwnd)
{
	pa_hwnd_ = pa_hwnd;
	return Create( pa_hwnd_, nullptr,
		WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW | WS_EX_TOPMOST);
}

LRESULT TrayPopWnd::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	return LRESULT();
}

LRESULT TrayPopWnd::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	//if (wParam == VK_ESCAPE)
	//	ShowWindow(SW_HIDE);

	//bHandled = FALSE;
	return 0;
}

LRESULT TrayPopWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	ShowWindow(SW_HIDE);
	bHandled = FALSE;
	return 0;
}

void TrayPopWnd::OnClickBtn(TNotifyUI & msg, bool & handled)
{
	CDuiString ctrl_name = msg.pSender->GetName();
	if (ctrl_name == _T("pop_home")) {
		::PostMessage(pa_hwnd_, kAM_TrayPopHomeMsg, 0, 0);
	} else if (ctrl_name == _T("pop_exit")) {
		::PostMessage(pa_hwnd_, kAM_TrayPopExitMsg, 0, 0);
	}
	ShowWindow(SW_HIDE);
}

void TrayPopWnd::OnSelectChanged(TNotifyUI & msg, bool & handled)
{
	CDuiString ctrl_name = msg.pSender->GetName();
	bool check = static_cast<PDUI_CHECKBOX>(msg.pSender)->GetCheck();
	WPARAM wparam = 0;
	if (ctrl_name == _T("hotkey"))
		wparam = 1;
	else if (ctrl_name == _T("minimize"))
		wparam = 2;
	::PostMessage(pa_hwnd_, kAM_TrayPostPopMsg, wparam, check);
}

void TrayPopWnd::PopupWindow(PPOINT point, bool left_bottom)
{
	int srceen_width = ::GetSystemMetrics(SM_CXSCREEN);
	int srceen_height = ::GetSystemMetrics(SM_CYSCREEN);
	int x = point->x;
	int y = point->y;

	if (x + 125 > srceen_width)
		x -= 125;
	if (y + 150 > srceen_height)
		y -= 150;

	::SetWindowPos(m_hWnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	ShowWindow();
}
