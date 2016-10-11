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

void TrayPopWnd::PopupWindow(PPOINT point, bool left_bottom)
{
	int srceen_width = ::GetSystemMetrics(SM_CXSCREEN);
	int srceen_height = ::GetSystemMetrics(SM_CYSCREEN);
	int x = point->x;
	int y = point->y;

	if (x + 70 > srceen_width)
		x -= 70;
	if (y + 124 > srceen_height)
		y -= 124;

	::SetWindowPos(m_hWnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	ShowWindow();
}
