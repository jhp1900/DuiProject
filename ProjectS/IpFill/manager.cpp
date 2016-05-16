#include "manager.h"
#include "add_new_play_wnd.h"

Manager* Manager::instance_ = nullptr;

Manager::Manager() 
{
  xml_manager_.LoadFile(CPaintManagerUI::GetInstancePath(), _T("config.xml"));
}

Manager * Manager::GetInstance()
{
  return instance_ ? instance_ : (instance_ = new Manager());
}

void Manager::DestroyInstance()
{
  if (instance_) {
    delete instance_;
    instance_ = nullptr;
  }
}

LRESULT Manager::OnInit()
{
  /* 读取所有的方案名，并填入方案名下来列表中 */
  PDUI_COMBO play_list = static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("play_list")));
  PDUI_LISTLABELELEM list_elem;
  for (auto play_name : xml_manager_.GetAllNodeName()) {
    list_elem = new CListLabelElementUI;
    list_elem->SetText(play_name);
    play_list->Add(list_elem);
  }

  /* 初始化收录所有 ip地址 控件 */
  vector<LPCTSTR> all_ip_ui_str = {
    _T("ip_address"),
    _T("netmask"),
    _T("gateway"),
    _T("firstDNS"),
    _T("secondDNS")
  };
  IpControlUI * ip_ui_temp = nullptr;
  for (auto iter : all_ip_ui_str) {
    ip_ui_temp = static_cast<IpControlUI *>(m_PaintManager.FindControl(iter));
    ip_ui_vector_.push_back(ip_ui_temp);
  }

  return 0;
}

void Manager::OnUserClick(const TNotifyUI& msg)
{
  if (msg.pSender->GetName() == _T("start_play")) {

  } else if (msg.pSender->GetName() == _T("advanced_btn")) {

  } else if (msg.pSender->GetName() == _T("update_play_btn")) {

  } else if (msg.pSender->GetName() == _T("del_play_btn")) {

  } else if (msg.pSender->GetName() == _T("add_play_btn")) {
    OnClickAddPlayBtn();
  } else if (msg.pSender->GetName() == _T("test_btn")) {      // 测试内容

  }
}

LRESULT Manager::OnInitCustomControlMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
  return LRESULT();
}

LRESULT Manager::OnLogCloseMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
  Close();
  return LRESULT();
}

void Manager::OnClickAddPlayBtn()
{
  NodeStruct node_info;
  if (!GetPlayInfo(node_info))
    return;

  AddNewPlayWnd new_play;
  new_play.DoModal(*this);
  char temp[MAX_PATH] = { 0 };
  node_info.node_name = xml_manager_.WideToMulti(new_play.GetNewName(), temp);
  xml_manager_.InsertNode(node_info);
}

BOOL Manager::GetPlayInfo(NodeStruct & node_info)
{
  for (auto iter : ip_ui_vector_) {
    if (!iter->IsReasonable())
      return FALSE;
  }


  return TRUE;
}
