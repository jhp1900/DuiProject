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
    EditMoreWnd more_wnd;
    more_wnd.DoModal(*this);
  } else if (msg.pSender->GetName() == _T("update_btn")) {

  } else if (msg.pSender->GetName() == _T("del_btn")) {

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
  ResSingleton* res = ResSingleton::GetInstance();
  XmlManager* xml_manager = res->GetXmlManager();
  NETSTRUCT net_info = xml_manager->GetNodeInfo(play_name_);

  PDUI_LIST ip_list = static_cast<PDUI_LIST>(m_PaintManager.FindControl(_T("ip_list")));
  PDUI_LISTTEXTELEM elem;
  for (auto iter : net_info.more_ip_mask) {
    elem = new CListTextElementUI;
    ip_list->Add(elem);
    elem->SetText(0, iter.first);
    elem->SetText(1, iter.second);
  }
}
