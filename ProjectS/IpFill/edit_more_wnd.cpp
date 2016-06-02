#include "edit_more_wnd.h"
#include "res_singleton.h"

EditMoreWnd::EditMoreWnd(LPCTSTR play_name)
  : play_name_(play_name)
  , index_(-1)
  , ipaddr_(_T(""))
  , netmask_(_T(""))
  , flag_(FALSE)
  , type_is_add_(TRUE)
{
}

EditMoreWnd::EditMoreWnd(LPCTSTR play_name, int index)
  : play_name_(play_name)
  , index_(index)
  , ipaddr_(_T(""))
  , netmask_(_T(""))
  , flag_(FALSE)
  , type_is_add_(FALSE)
{
  XmlManager *xml_manage = ResSingleton::GetInstance()->GetXmlManager();
  pugi::xml_node node = xml_manage->GetNode(play_name_).child("MoreIP");
  node = xml_manage->GetNode(node, index_);
  ipaddr_ = xml_manage->MultiToWide(node.attribute("ip").as_string());
  netmask_ = xml_manage->MultiToWide(node.attribute("netmask").as_string());
}

EditMoreWnd::~EditMoreWnd()
{
}

void EditMoreWnd::OnUserClick(const TNotifyUI & msg)
{
  if (msg.pSender->GetName() == _T("OK_btn")) {
    OnClickOKBtn();
  }
}

LRESULT EditMoreWnd::OnInit()
{
  IpControlUI *ipaddr = static_cast<IpControlUI *>(m_PaintManager.FindControl(_T("edit_ipaddr")));
  IpControlUI *netmask = static_cast<IpControlUI *>(m_PaintManager.FindControl(_T("edit_netmask")));

  ipaddr->SetText(ipaddr_);
  netmask->SetText(netmask_);

  return LRESULT();
}

BOOL EditMoreWnd::DoModal(HWND pa_hwnd)
{
  if (!Create(pa_hwnd, _T(""), WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_ACCEPTFILES))
    return false;
  CenterWindow(pa_hwnd);

  return ShowModal() != 0;
}

void EditMoreWnd::OnClickOKBtn()
{
  IpControlUI *ipaddr = static_cast<IpControlUI *>(m_PaintManager.FindControl(_T("edit_ipaddr")));
  IpControlUI *netmask = static_cast<IpControlUI *>(m_PaintManager.FindControl(_T("edit_netmask")));

  if (!ipaddr->IsReasonable() || !netmask->IsReasonable()) {
    MessageBox(nullptr, _T("信息不合理，请确认后重试！"), _T("ERROR"), MB_OK);
    return;
  }

  if (ipaddr_ == ipaddr->GetText() && netmask_ == netmask->GetText()) {
    MessageBox(nullptr, _T("信息未曾修过！"), _T("ERROR"), MB_OK);
    return;
  }

  XmlManager *xml_manage = ResSingleton::GetInstance()->GetXmlManager();
  pugi::xml_node pa_node = xml_manage->GetNode(play_name_).child("MoreIP");
  pair<string, LPCTSTR> param1 = { "ip", ipaddr->GetText() };
  pair<string, LPCTSTR> param2 = { "netmask", netmask->GetText() };
  BOOL is_have = xml_manage->IsHave(pa_node, param1, param2);

  if (is_have) {
    MessageBox(nullptr, _T("已存在相同的信息！"), _T("ERROR"), MB_OK);
    return;
  }
  
  if (type_is_add_) {
    xml_manage->InsertNode(pa_node, param1, param2);
    flag_ = TRUE;
    Close();
  } else {
    if (xml_manage->UpdateNode(pa_node, index_, param1, param2)) {
      MessageBox(nullptr, _T("信息修改成功！"), _T("Message"), MB_OK);
      flag_ = TRUE;
      Close();
    }
  }
}
