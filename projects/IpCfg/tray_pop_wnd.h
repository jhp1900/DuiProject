#pragma once

#include "window_impl_base.h"
#include "msg_head.h"

enum TrayPopMsg
{
	kAM_PopClickHomeMsg,
	kAM_PopClickExitMsg,
	kAM_PopSelectHotkayMsg,
	kAM_PopSelectMinMsg,
};

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
		DUINOTIFY_HANDLER(_T("pop_home"), DUINOTIFY_CLICK, OnClickBtn)
		DUINOTIFY_HANDLER(_T("pop_exit"), DUINOTIFY_CLICK, OnClickBtn)
		DUINOTIFY_TYPE_HANDLER(DUINOTIFY_SELECTCHANGED, OnSelectChanged)
	END_DUINOTIFY_MAP()

public:
	LRESULT OnInit() override;
	HWND CreateWithStyle(HWND pa_hwnd, DWORD style);
	HWND CreateWithDefaltStyle(HWND pa_hwnd);

private:
	LRESULT OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

private:
	void OnClickBtn(TNotifyUI &msg, bool &handled);
	void OnSelectChanged(TNotifyUI &msg, bool &handled);

public:
	void PopupWindow(PPOINT point, bool left_bottom = false);

private:
	HWND pa_hwnd_;
};

