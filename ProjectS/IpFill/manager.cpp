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
  /* ��ȡ���еķ������������뷽���������б��� */
  FlushPlayList();

  /* ��ʼ����¼���� ip��ַ �ؼ� */
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
  if (msg.pSender->GetName() == _T("start_play_btn")) {       // ��������
    StartPlay();
  } else if (msg.pSender->GetName() == _T("advanced_btn")) {

  } else if (msg.pSender->GetName() == _T("update_play_btn")) {

  } else if (msg.pSender->GetName() == _T("del_play_btn")) {

  } else if (msg.pSender->GetName() == _T("add_play_btn")) {
    OnClickAddPlayBtn();
  } else if (msg.pSender->GetName() == _T("test_btn")) {      // ��������
    
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
  NetStruct net_info;
  if (!GetPlayInfo(net_info)) {
    MessageBox(nullptr, _T("������Ϣ������Ŷ���ף�"), _T("Message"), MB_OK);
    return;
  }

  AddNewPlayWnd new_play;
  new_play.DoModal(*this);
  if (new_play.GetNewName() == _T("")) {
    MessageBox(nullptr, _T("����������Ϊ��Ŷ���ף�"), _T("Message"), MB_OK);
    return;
  }

  net_info.play_name = new_play.GetNewName();
  xml_manager_.InsertNode(net_info);

  FlushPlayList();
}

BOOL Manager::GetPlayInfo(NetStruct & net_info)
{
  for (auto iter : ip_ui_vector_) {
    if (!iter->IsReasonable())
      return FALSE;
  }

  net_info.ip_address = ip_ui_vector_[0]->GetText();
  net_info.netmask = ip_ui_vector_[1]->GetText();
  net_info.gateway = ip_ui_vector_[2]->GetText();
  net_info.firstDNS = ip_ui_vector_[3]->GetText();
  net_info.secondDNS = ip_ui_vector_[4]->GetText();

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

  NetStruct net_info = xml_manager_.GetNodeInfo(play_name);
  ip_ui_vector_[0]->SetText(net_info.ip_address);
  ip_ui_vector_[1]->SetText(net_info.netmask);
  ip_ui_vector_[2]->SetText(net_info.gateway);
  ip_ui_vector_[3]->SetText(net_info.firstDNS);
  ip_ui_vector_[4]->SetText(net_info.secondDNS);
}

void Manager::StartPlay()
{
  vector<LPCTSTR> command_lines;
  CDuiString set_ip_line = _T("/c netsh interface ip set address \"��̫��\"");
  CDuiString set_dns_line = _T("/c netsh interface ip set dnsservers \"��̫��\"");
  CDuiString add_ip_line = _T("/c netsh interface ip add address \"��̫��\"");
  CDuiString add_dns_line = _T("/c netsh interface ip add dnsservers \"��̫��\" ");
  PDUI_COMBO play_list = static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("play_list")));

  if (play_list->GetText() == _T("Auto")) {
    set_ip_line += _T(" dhcp");
    set_dns_line += _T(" dhcp");
    command_lines.push_back(set_ip_line);
    command_lines.push_back(set_dns_line);
  } else {
    NetStruct net_info = xml_manager_.GetNodeInfo(play_list->GetText());

    set_ip_line = set_ip_line 
      + _T(" static ") + net_info.ip_address 
      + _T(" ") + net_info.netmask 
      + _T(" ") + net_info.gateway;
    set_dns_line = set_dns_line + _T(" static ") + net_info.firstDNS;
    command_lines.push_back(set_ip_line);
    command_lines.push_back(set_dns_line);

    for (auto iter : net_info.more_ip_mask) {
      command_lines.push_back(add_ip_line + _T(" ") + iter.first + _T(" ") + iter.second);
    }

    if (net_info.secondDNS != _T("")) {
      add_dns_line += net_info.secondDNS;
      command_lines.push_back(add_dns_line);
      int a = 0;
    }
  }

  ExcuteCommand(command_lines);
}

void Manager::ExcuteCommand(LPCTSTR command_lien)
{
  SHELLEXECUTEINFO shell_info = { 0 };
  shell_info.cbSize = sizeof(SHELLEXECUTEINFO);
  shell_info.fMask = SEE_MASK_NOCLOSEPROCESS;
  shell_info.lpVerb = _T("runas");      // runas ����ʵ���Թ���ԱȨ�����г���
  shell_info.hwnd = nullptr;
  shell_info.lpFile = _T("cmd.exe");
  shell_info.lpDirectory = nullptr;
  shell_info.nShow = SW_HIDE;
  shell_info.lpParameters = command_lien;
  shell_info.hInstApp = nullptr;

  ShellExecuteEx(&shell_info);
  //WaitForSingleObject(shell_info.hProcess, INFINITE);
  DWORD exitcode;
  GetExitCodeProcess(shell_info.hProcess, &exitcode);
}

void Manager::ExcuteCommand(vector<LPCTSTR> command_lien_s)
{
  for (auto iter : command_lien_s)
    ExcuteCommand(iter);
}
