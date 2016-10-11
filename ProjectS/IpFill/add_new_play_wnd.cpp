#include "add_new_play_wnd.h"

AddNewPlayWnd::AddNewPlayWnd()
	: new_name_(_T(""))
{
}

AddNewPlayWnd::~AddNewPlayWnd()
{
}

BOOL AddNewPlayWnd::DoModal(HWND pa_hwnd)
{
	if (!Create(pa_hwnd, _T(""), WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_ACCEPTFILES))
		return false;
	CenterWindow(pa_hwnd);

	return ShowModal() != 0;
}

void AddNewPlayWnd::OnUserClick(const TNotifyUI & msg)
{
	if (msg.pSender->GetName() == _T("OK_btn")) {
		PDUI_EDIT name_edit = static_cast<PDUI_EDIT>(m_PaintManager.FindControl(_T("name_edit")));
		new_name_ = name_edit->GetText();
		if (new_name_ == _T(""))
			MessageBox(nullptr, _T("方案名不能为空哦，亲！"), _T("Message"), MB_OK);
		Close();
	} else if (msg.pSender->GetName() == _T("cancel_btn")) {
		Close();
	}
}