#include "xml_manager.h"
#include "pugixml\pugixml.cpp"
#include <io.h>

void _NETSTRUCT::SetVar(LPCTSTR name, LPCTSTR value)
{
  if (name == _T("IPAddress"))
    ip_address = value;
  else if (name == _T("Netmask"))
    netmask = value;
  else if (name == _T("Gateway"))
    gateway = value;
  else if (name == _T("FirstDNS"))
    firstDNS = value;
  else if (name == _T("SecondDNS"))
    secondDNS = value;
}

CDuiString _NETSTRUCT::GetVar(LPCTSTR name)
{
  if (name == _T("IPAddress"))
    return ip_address;
  else if (name == _T("Netmask"))
    return netmask;
  else if (name == _T("Gateway"))
    return gateway;
  else if (name == _T("FirstDNS"))
    return firstDNS;
  else if (name == _T("SecondDNS"))
    return secondDNS;
}

XmlManager::XmlManager() :
  path_and_name_(_T("")),
  is_load_(false)
{
  vector<LPCTSTR> temp_str = {
    _T("IPAddress"),
    _T("Netmask"),
    _T("Gateway"),
    _T("FirstDNS"),
    _T("SecondDNS")
  };
  for (auto iter : temp_str)
    net_attrs_.push_back(iter);
}

XmlManager::~XmlManager()
{
}

BOOL XmlManager::LoadFile(CDuiString file_path, CDuiString file_name)
{
  path_and_name_ = file_path + file_name;
  char path[MAX_PATH] = { 0 };

  /* ����ļ������ڣ��򴴽�һ�������ļ���ʽ���ļ�; */
  if (_access(WideToMulti(path_and_name_, path), 0) == -1) {
    HANDLE file_handle = CreateFile(path_and_name_, GENERIC_WRITE | GENERIC_READ,
      0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    
    DWORD file_size = 0;
    CDuiString buffer = _T("<?xml version=\"1.0\"?><Info></Info>");
    ::WriteFile(file_handle, buffer, buffer.GetLength() * 2, &file_size, nullptr);

    CloseHandle(file_handle);   // �ر��ļ�
  }

  is_load_ = file_.load_file(path_and_name_.GetData());
  return is_load_;
}

void XmlManager::InsertNode(NETSTRUCT net_info)
{
  if (!is_load_)
    return;

  pugi::xml_node pa_node = file_.child("Info").append_child("Play");
  char temp[MAX_PATH] = { 0 };
  pa_node.append_attribute("name") = WideToMulti(net_info.play_name, temp);
  pugi::xml_node son_node;

  for (auto iter : net_attrs_) {
    son_node = pa_node.append_child(WideToMulti(iter, temp));
    son_node.append_attribute("value") = WideToMulti(net_info.GetVar(iter), temp);
  }

  son_node = pa_node.append_child("MoreIP");
  pugi::xml_node grandson_node;
  for (auto iter : net_info.more_ip_mask) {
    grandson_node = son_node.append_child("AddIPChild");
    grandson_node.append_attribute("ip") = WideToMulti(iter.first, temp);
    grandson_node.append_attribute("netmask") = WideToMulti(iter.second, temp);
  }

  file_.save_file(path_and_name_.GetData());
}

NETSTRUCT XmlManager::GetNodeInfo(LPCTSTR name)
{
  NETSTRUCT net_info;
  pugi::xml_node node = file_.child("Info");
  char temp_char[MAX_PATH] = { 0 };
  for (auto pa_node : node) {
    string play_name = pa_node.attribute("name").as_string();
    if (0 == play_name.compare(WideToMulti(name, temp_char))) {   // �����������Ҫ�ķ���
      net_info.play_name = name;
      char temp[MAX_PATH] = { 0 };
      pugi::xml_node son_node;
      for (auto iter : net_attrs_) {
        son_node = pa_node.child(WideToMulti(iter, temp));
        net_info.SetVar(iter, MultiToWide(son_node.attribute("value").as_string()));
      }
      
      pair<CDuiString, CDuiString> mo_ip_pair;
      for (auto iter : pa_node.child("MoreIP").children()) {
        mo_ip_pair.first = iter.attribute("ip").as_string();
        mo_ip_pair.second = iter.attribute("netmask").as_string();
        net_info.more_ip_mask.push_back(mo_ip_pair);
      }
    }
  }

  return net_info;
}

pugi::xml_node XmlManager::GetNode(LPCTSTR name)
{
  pugi::xml_node root_node = file_.child("Info");
  char temp_char[MAX_PATH] = { 0 };
  for (auto node : root_node) {
    string play_name = node.attribute("name").as_string();
    if (0 == play_name.compare(WideToMulti(name, temp_char)))    // �����������Ҫ�ķ���
      return node;
  }

  return pugi::xml_node();
}

vector<NETSTRUCT> XmlManager::GetAllNode()
{
  return vector<NETSTRUCT>();
}

vector<CDuiString> XmlManager::GetAllNodeName()
{
  vector<CDuiString> ret;
  for (auto node : file_.child("Info")) {
    ret.push_back(MultiToWide(node.attribute("name").as_string()));
  }
  return ret;
}

BOOL XmlManager::RemoveNode(pugi::xml_node node)
{
  pugi::xml_node pa_node = node.parent();
  BOOL ret = pa_node.remove_child(node);

  file_.save_file(path_and_name_.GetData());
  return ret;
}

/* �����ߺ����� ���ֽ� ת ���ֽ� */
char * XmlManager::WideToMulti(CDuiString wide, char * multi)
{
  ZeroMemory(multi, MAX_PATH);
  int wide_len = wide.GetLength();
  int multi_len = WideCharToMultiByte(CP_ACP, NULL, wide.GetData(), wide_len, NULL, 0, NULL, FALSE);
  ::WideCharToMultiByte(CP_ACP, NULL, wide.GetData(), -1, multi, multi_len, NULL, FALSE);
  return multi;
}

/* �����ߺ����� ���ֽ� ת ���ֽ� */
CDuiString XmlManager::MultiToWide(string multi)
{
  int multi_len = multi.length();
  int wide_len = ::MultiByteToWideChar(CP_ACP, 0, multi.c_str(), -1, NULL, 0);
  wchar_t *wide_str;
  wide_str = new wchar_t[wide_len + 1];
  memset(wide_str, 0, (wide_len + 1) * sizeof(wchar_t));
  ::MultiByteToWideChar(CP_ACP, 0, multi.c_str(), -1, (LPWSTR)wide_str, wide_len);

  CDuiString ret = (wchar_t*)wide_str;
  delete wide_str;

  return ret;
}
