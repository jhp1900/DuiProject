#pragma once

#include "window_impl_base.h"

class TrayPopWnd : public WindowImplBase
{
public:
	TrayPopWnd();
	~TrayPopWnd();

	DECLARE_DUIWND_INFO(_T("TrayPopWnd"), CS_DBLCLKS, _T("tray_pop_wnd.xml"))

	BEGIN_DUIMSG_MAP(TrayPopWnd)
		DUIMSG_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		DUIMSG_HANDLER(WM_KEYDOWN, OnKeyDown)
		DUIMSG_HANDLER(WM_KILLFOCUS, OnKillFocus)
	END_DUIMSG_MAP()

	BEGIN_DUINOTIFY_MAP(TrayPopWnd)
	END_DUINOTIFY_MAP()

public:
	LRESULT OnInit() override;
	HWND CreateWithStyle(HWND pa_hwnd, DWORD style);
	HWND CreateWithDefaltStyle(HWND pa_hwnd);

public:
	LRESULT OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

public:
	void PopupWindow(PPOINT point, bool left_bottom = false);

private:
	HWND pa_hwnd_;
};

