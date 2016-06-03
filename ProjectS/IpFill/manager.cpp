#include "manager.h"
#include "add_new_play_wnd.h"
#include "advanced_wnd.h"
#include <shellapi.h>
#include "res_singleton.h"
#include <NetCon.h>

Manager::Manager() 
{
  ResSingleton* res = ResSingleton::GetInstance();
  xml_manager_ = res->GetXmlManager();
  xml_manager_->LoadFile(CPaintManagerUI::GetInstancePath(), _T("config.xml"));
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
    OnClickAdvanced();
  } else if(msg.pSender->GetName() == _T("edit_play_btn")) {
    OnClickEditBtn();
  } else if (msg.pSender->GetName() == _T("update_play_btn")) {
    OnClickUpdateBtn();
  } else if (msg.pSender->GetName() == _T("del_play_btn")) {
    OnClickDelBtn();
  } else if (msg.pSender->GetName() == _T("add_play_btn")) {
    OnClickAddPlayBtn();
  } else if (msg.pSender->GetName() == _T("test_btn")) {      // 测试内容
    OnClickTestBtn();
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
  NETSTRUCT net_info;
  if (!GetPlayInfo(net_info)) {
    MessageBox(nullptr, _T("方案信息不合理哦，亲！"), _T("Message"), MB_OK);
    return;
  }

  AddNewPlayWnd new_play;
  new_play.DoModal(*this);
  CDuiString play_name = new_play.GetNewName();
  if (play_name == _T(""))
    return;

  net_info.play_name = play_name;
  xml_manager_->InsertNode(net_info);

  FlushPlayList();
}

void Manager::OnClickAdvanced()
{
  PDUI_COMBO play_list = static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("play_list")));
  AdvancedWnd adv_wnd(play_list->GetText());
  adv_wnd.DoModal(*this);
}

void Manager::OnClickDelBtn()
{
  PDUI_COMBO play_list = static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("play_list")));
  LPCTSTR play_name = play_list->GetText();
  if (0 == lstrcmp(play_name, _T("Auto"))) {
    MessageBox(nullptr, _T("自动获取方案不能删除哦，亲！"), _T("Error"), MB_OK);
    return;
  }
  if (xml_manager_->RemoveNode(xml_manager_->GetNode(play_name))) {
    MessageBox(nullptr, _T("方案删除成功!"), _T("Message"), MB_OK);
    FlushPlayList();
  } else {
    MessageBox(nullptr, _T("方案删除失败!"), _T("Message"), MB_OK);
  }
}

void Manager::OnClickEditBtn()
{
  for (auto iter : ip_ui_vector_) {
    iter->SetStateEdit(true);
  }
  m_PaintManager.FindControl(_T("edit_play_btn"))->SetVisible(false);
  m_PaintManager.FindControl(_T("update_play_btn"))->SetVisible(true);
}

void Manager::OnClickUpdateBtn()
{
  NETSTRUCT net_info;
  net_info.play_name = m_PaintManager.FindControl(_T("play_list"))->GetText();
  if (!GetPlayInfo(net_info)) {
    MessageBox(nullptr, _T("方案信息不合理哦，亲！"), _T("Message"), MB_OK);
    return;
  }

  if (xml_manager_->UpdateNode(net_info)) {
    MessageBox(nullptr, _T("方案修改成功！"), _T("Message"), MB_OK);
  } else {
    MessageBox(nullptr, _T("方案修改失败了哦，亲！"), _T("ERROR"), MB_OK);
    return;
  }

  for (auto iter : ip_ui_vector_) {
    iter->SetStateEdit(false);
  }
  m_PaintManager.FindControl(_T("update_play_btn"))->SetVisible(false);
  m_PaintManager.FindControl(_T("edit_play_btn"))->SetVisible(true);
}

BOOL Manager::GetPlayInfo(NETSTRUCT & net_info)
{
  for (auto iter : ip_ui_vector_) {
    if (!iter->IsReasonable())
      return FALSE;
  }

  for (int i = 0; i != 5; ++i) {
    net_info.SetVar(xml_manager_->net_attrs_[i], ip_ui_vector_[i]->GetText());
  }

  return TRUE;
}

void Manager::FlushPlayList()
{
  PDUI_COMBO play_list = static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("play_list")));
  for (int i = 1; i < play_list->GetCount(); )
    play_list->RemoveAt(1);

  PDUI_LISTLABELELEM list_elem;
  for (auto play_name : xml_manager_->GetAllNodeName()) {
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

  NETSTRUCT net_info = xml_manager_->GetNodeInfo(play_name);
  for (int i = 0; i != 5; ++i) {
    ip_ui_vector_[i]->SetText(net_info.GetVar(xml_manager_->net_attrs_[i]));
  }
}

void Manager::StartPlay()
{
  vector<CDuiString> command_lines;
  CDuiString set_ip_line = _T("/c netsh interface ip set address \"以太网\" ");
  CDuiString set_dns_line = _T("/c netsh interface ip set dnsservers \"以太网\" ");
  CDuiString add_ip_line = _T("/c netsh interface ip add address \"以太网\" ");
  CDuiString add_dns_line = _T("/c netsh interface ip add dnsservers \"以太网\" ");
  PDUI_COMBO play_list = static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("play_list")));

  if (play_list->GetText() == _T("Auto")) {
    set_ip_line += _T("dhcp");
    set_dns_line += _T("dhcp");
    command_lines.push_back(set_ip_line);
    command_lines.push_back(set_dns_line);
  } else {
    NETSTRUCT net_info = xml_manager_->GetNodeInfo(play_list->GetText());

    set_ip_line = set_ip_line 
      + _T("static ") + net_info.ip_address 
      + _T(" ") + net_info.netmask 
      + _T(" ") + net_info.gateway;
    command_lines.push_back(set_ip_line);

    set_dns_line = set_dns_line + _T("static ") + net_info.firstDNS;
    command_lines.push_back(set_dns_line);

    for (auto iter : net_info.more_ip_mask) {
      command_lines.push_back(add_ip_line + iter.first + _T(" ") + iter.second);
    }

    if (net_info.secondDNS != _T("")) {
      add_dns_line += net_info.secondDNS;
      command_lines.push_back(add_dns_line);
      int a = 0;
    }
  }

  if (ExcuteCommand(command_lines))
    MessageBox(nullptr, _T("方案启用成功!"), _T("Message"), MB_OK);
  else
    MessageBox(nullptr, _T("方案启用失败！请查证！"), _T("Message"), MB_OK);
}

BOOL Manager::ExcuteCommand(LPCTSTR command_lien)
{
  SHELLEXECUTEINFO shell_info = { 0 };
  shell_info.cbSize = sizeof(SHELLEXECUTEINFO);
  shell_info.fMask = SEE_MASK_NOCLOSEPROCESS;
  shell_info.lpVerb = _T("runas");      // runas 可以实现以管理员权限运行程序
  shell_info.hwnd = nullptr;
  shell_info.lpFile = _T("cmd.exe");
  shell_info.lpDirectory = nullptr;
  shell_info.nShow = SW_HIDE;
  shell_info.lpParameters = command_lien;
  shell_info.hInstApp = nullptr;

  DWORD exitcode;
  ShellExecuteEx(&shell_info);
  return GetExitCodeProcess(shell_info.hProcess, &exitcode);
}

BOOL Manager::ExcuteCommand(vector<CDuiString> command_lien_s)
{
  for (auto iter : command_lien_s)
    if (!ExcuteCommand(iter))
      return FALSE;

  return TRUE;
}

void Manager::OnClickTestBtn()
{
  INetConnectionManager *net_manager = nullptr;
  INetConnection *net_conn = nullptr;
  IEnumNetConnection *net_enum = nullptr;
  ULONG celt_fetched;

  setlocale(LC_CTYPE, "");
  CoInitialize(NULL);
  CoCreateInstance(CLSID_ConnectionManager, NULL, CLSCTX_SERVER, IID_INetConnectionManager, (void**)&net_manager);
  if (net_manager == nullptr) {
    MessageBox(nullptr, _T("网络检测失败！"), _T("ERROR"), MB_OK);
    return;
  }
  net_manager->EnumConnections(NCME_DEFAULT, &net_enum);
  while (net_enum->Next(1, &net_conn, &celt_fetched) == S_OK)
  {

  }

  NETCON_PROPERTIES net_proper;
  
}
