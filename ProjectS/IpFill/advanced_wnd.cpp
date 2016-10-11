#include "advanced_wnd.h"
#include "manager.h"
#include "res_singleton.h"
#include "edit_more_wnd.h"

AdvancedWnd::AdvancedWnd()
	: play_name_(_T(""))
{
}

AdvancedWnd::AdvancedWnd(LPCTSTR play_name)
{
	AdvancedWnd();
	play_name_ = play_name;
}

AdvancedWnd::~AdvancedWnd()
{
}

LRESULT AdvancedWnd::OnInit()
{
	FlushList();

	return 0;
}

void AdvancedWnd::OnUserClick(const TNotifyUI & msg)
{
	if (msg.pSender->GetName() == _T("add_btn")) {
		OnClickAddBtn();
	} else if (msg.pSender->GetName() == _T("update_btn")) {
		OnClickUpdateBtn();
	} else if (msg.pSender->GetName() == _T("del_btn")) {
		OnClickDelBtn();
	}
}

BOOL AdvancedWnd::DoModal(HWND pa_hwnd)
{
	if (!Create(pa_hwnd, _T(""), WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_ACCEPTFILES))
		return false;
	CenterWindow(pa_hwnd);

	return ShowModal() != 0;
}

void AdvancedWnd::FlushList()
{
	XmlManager* xml_manager = ResSingleton::GetInstance()->GetXmlManager();
	NETSTRUCT net_info = xml_manager->GetNodeInfo(play_name_);

	PDUI_LIST ip_list = static_cast<PDUI_LIST>(m_PaintManager.FindControl(_T("ip_list")));
	ip_list->RemoveAll();
	PDUI_LISTTEXTELEM elem;

	for (auto iter : net_info.more_ip_mask) {
		elem = new CListTextElementUI;
		ip_list->Add(elem);
		elem->SetText(0, iter.first);
		elem->SetText(1, iter.second);
	}
}

void AdvancedWnd::OnClickAddBtn()
{
	EditMoreWnd more_wnd(play_name_);
	more_wnd.DoModal(*this);
	if (more_wnd.GetFlag())
		FlushList();
}

void AdvancedWnd::OnClickUpdateBtn()
{
	PDUI_LIST ip_list = static_cast<PDUI_LIST>(m_PaintManager.FindControl(_T("ip_list")));
	int index = ip_list->GetCurSel();
	if (index != -1) {
		EditMoreWnd more_wnd(play_name_, index);
		more_wnd.DoModal(*this);
		if (more_wnd.GetFlag())
			FlushList();
	}
}

void AdvancedWnd::OnClickDelBtn()
{
	PDUI_LIST ip_list = static_cast<PDUI_LIST>(m_PaintManager.FindControl(_T("ip_list")));
	int index = ip_list->GetCurSel();
	if (index != -1) {
		if (IDNO == MessageBox(nullptr, _T("确定删除该条信息？？"), _T("Warning"), MB_YESNO))
			return;

		XmlManager* xml_manager = ResSingleton::GetInstance()->GetXmlManager();
		pugi::xml_node pa_node = xml_manager->GetNode(play_name_).child("MoreIP");
		if (xml_manager->RemoveNode(pa_node, index)) {
			MessageBox(nullptr, _T("IP 删除成功!"), _T("Message"), MB_OK);
			FlushList();
		} else {
			MessageBox(nullptr, _T("IP 删除失败!"), _T("ERROR"), MB_OK);
		}
	}
}
