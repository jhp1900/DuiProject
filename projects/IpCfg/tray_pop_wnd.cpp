#include "tray_pop_wnd.h"
#include "res_singleton.h"

TrayPopWnd::TrayPopWnd()
	: pa_hwnd_(0)
{
}

TrayPopWnd::~TrayPopWnd()
{
}

LRESULT TrayPopWnd::OnInit()
{
	XmlManager & xml_manager = ResSingleton::GetInstance()->GetXmlMamager();
	PDUI_CHECKBOX check_box;
	check_box = static_cast<PDUI_CHECKBOX>(m_PaintManager.FindControl(_T("hotkey")));
	if (check_box)
		check_box->Selected(xml_manager.GetPopAttr(_T("hotkey")));

	check_box = static_cast<PDUI_CHECKBOX>(m_PaintManager.FindControl(_T("minimize")));
	if (check_box)
		check_box->Selected(xml_manager.GetPopAttr(_T("minimize")));

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
	WPARAM wparam = -1;

	if (ctrl_name == _T("pop_home")) 
		wparam = kAM_PopClickHomeMsg;
	else if (ctrl_name == _T("pop_exit")) 
		wparam = kAM_PopClickExitMsg;

	::PostMessage(pa_hwnd_, kAM_TrayPopMsg, wparam, 0);
	ShowWindow(SW_HIDE);
}

void TrayPopWnd::OnSelectChanged(TNotifyUI & msg, bool & handled)
{
	CDuiString ctrl_name = msg.pSender->GetName();
	bool check = static_cast<PDUI_CHECKBOX>(msg.pSender)->GetCheck();
	WPARAM wparam = -1;

	if (ctrl_name == _T("hotkey"))
		wparam = kAM_PopSelectHotkayMsg;
	else if (ctrl_name == _T("minimize"))
		wparam = kAM_PopSelectMinMsg;

	::PostMessage(pa_hwnd_, kAM_TrayPopMsg, wparam, check);
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
