#include "manager.h"
#include "add_new_play_wnd.h"
#include <shellapi.h>

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

void Manager::Notify(TNotifyUI & msg)
{
  if (msg.sType == _T("itemselect"))
    if (msg.pSender->GetName() == _T("play_list"))
      OnSelectPlay(msg);

  __super::Notify(msg);
}

LRESULT Manager::OnInit()
{
  /* 读取所有的方案名，并填入方案名下来列表中 */
  FlushPlayList();

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
  if (msg.pSender->GetName() == _T("start_play_btn")) {       // 启动方案
    StartPlay();
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
  if (!GetPlayInfo(node_info)) {
    MessageBox(nullptr, _T("方案信息不合理哦，亲！"), _T("Message"), MB_OK);
    return;
  }

  AddNewPlayWnd new_play;
  new_play.DoModal(*this);
  CDuiString node_name = new_play.GetNewName();
  if (node_name == _T("")) {
    MessageBox(nullptr, _T("方案名不能为空哦，亲！"), _T("Message"), MB_OK);
    return;
  }

  char temp[MAX_PATH] = { 0 };
  node_info.node_name = xml_manager_.WideToMulti(node_name, temp);
  xml_manager_.InsertNode(node_info);

  FlushPlayList();
}

BOOL Manager::GetPlayInfo(NodeStruct & node_info)
{
  for (auto iter : ip_ui_vector_) {
    if (!iter->IsReasonable())
      return FALSE;
  }

  vector<string> attr_name = {
    "ip_address",
    "netmask",
    "gateway",
    "firstDNS",
    "secondDNS"
  };
  
  for (int i = 0; i != 5; ++i) {
    char temp[MAX_PATH] = { 0 };
    string attr_value = xml_manager_.WideToMulti(ip_ui_vector_[i]->GetText(), temp);
    node_info.attrs.push_back({ attr_name[i], attr_value });
  }

  return TRUE;
}

void Manager::FlushPlayList()
{
  PDUI_COMBO play_list = static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("play_list")));
  for (int i = 1; i < play_list->GetCount(); )
    play_list->RemoveAt(1);

  PDUI_LISTLABELELEM list_elem;
  for (auto play_name : xml_manager_.GetAllNodeName()) {
    list_elem = new CListLabelElementUI;
    list_elem->SetText(play_name);
    play_list->Add(list_elem);
  }
}

void Manager::OnSelectPlay(TNotifyUI & msg)
{
  CDuiString play_name = msg.pSender->GetText();
  if (play_name == _T("Auto")) {
    for (auto iter : ip_ui_vector_) {
      iter->SetText(_T("..."));
    }
    return;
  }

  NodeStruct node_info = xml_manager_.GetNodeInfo(play_name);
  for (int i = 0; i != 5; ++i) {
    LPCTSTR ip_info = xml_manager_.MultiToWide(node_info.attrs[i].second);
    ip_ui_vector_[i]->SetText(ip_info);
  }
}

void Manager::StartPlay()
{
  PDUI_COMBO play_list = static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("play_list")));
  NodeStruct node_info = xml_manager_.GetNodeInfo(play_list->GetText());

  CDuiString set_ip_line = _T("/c netsh interface ip set address \"以太网\" static");
  CDuiString set_dns_line = _T("/c netsh interface ip set dnsservers \"以太网\" static");

  for (int i = 0; i != 3; ++i) {
    set_ip_line += _T(" ");
    set_ip_line += xml_manager_.MultiToWide(node_info.attrs[i].second);
  }
    
  ExcuteCommand(set_ip_line);
}

void Manager::ExcuteCommand(LPCTSTR command_lien)
{
  SHELLEXECUTEINFO shell_info = { 0 };
  shell_info.cbSize = sizeof(SHELLEXECUTEINFO);
  shell_info.fMask = SEE_MASK_NOCLOSEPROCESS;
  shell_info.lpVerb = nullptr;
  shell_info.hwnd = nullptr;
  shell_info.lpFile = _T("cmd.exe");
  shell_info.lpDirectory = nullptr;
  shell_info.nShow = SW_HIDE;
  shell_info.lpParameters = command_lien;
  shell_info.hInstApp = nullptr;

  ShellExecuteEx(&shell_info);
  WaitForSingleObject(shell_info.hProcess, INFINITE);
}
