#include "xml_manager.h"
#include "pugixml\pugixml.cpp"
#include <io.h>

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

  /* 如果文件不存在，则创建一个配置文件格式的文件; */
  if (_access(WideToMulti(path_and_name_, path), 0) == -1) {
    HANDLE file_handle = CreateFile(path_and_name_, GENERIC_WRITE | GENERIC_READ,
      0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    
    DWORD file_size = 0;
    CDuiString buffer = _T("<?xml version=\"1.0\"?><Info></Info>");
    ::WriteFile(file_handle, buffer, buffer.GetLength() * 2, &file_size, nullptr);

    CloseHandle(file_handle);   // 关闭文件
  }

  is_load_ = file_.load_file(path_and_name_.GetData());
  return is_load_;
}

void XmlManager::InsertNode(NetStruct net_info)
{
  if (!is_load_)
    return;

  pugi::xml_node pa_node = file_.child("Info").append_child("Play");
  char temp[MAX_PATH] = { 0 };
  ZeroMemory(temp, MAX_PATH);
  pa_node.append_attribute("name") = WideToMulti(net_info.play_name, temp);
  pugi::xml_node son_node;

  son_node = pa_node.append_child("IPAddress");
  ZeroMemory(temp, MAX_PATH);
  son_node.append_attribute("value") = WideToMulti(net_info.ip_address, temp);
  
  son_node = pa_node.append_child("Netmask");
  ZeroMemory(temp, MAX_PATH);
  son_node.append_attribute("value") = WideToMulti(net_info.netmask, temp);

  son_node = pa_node.append_child("Gateway");
  ZeroMemory(temp, MAX_PATH);
  son_node.append_attribute("value") = WideToMulti(net_info.gateway, temp);

  son_node = pa_node.append_child("FirstDNS");
  ZeroMemory(temp, MAX_PATH);
  son_node.append_attribute("value") = WideToMulti(net_info.firstDNS, temp);

  son_node = pa_node.append_child("SecondDNS");
  ZeroMemory(temp, MAX_PATH);
  son_node.append_attribute("value") = WideToMulti(net_info.secondDNS, temp);

  file_.save_file(path_and_name_.GetData());
}

NetStruct XmlManager::GetNodeInfo(LPCTSTR name)
{
  NetStruct net_info;
  pugi::xml_node node = file_.child("Info");
  char temp_char[MAX_PATH] = { 0 };
  for (auto pa_node : node) {
    string play_name = pa_node.attribute("name").as_string();
    if (0 == play_name.compare(WideToMulti(name, temp_char))) {   // 如果是我们需要的方案
      net_info.play_name = name;
      net_info.ip_address = pa_node.child("IPAddress").attribute("value").as_string();
      net_info.netmask = pa_node.child("Netmask").attribute("value").as_string();
      net_info.gateway = pa_node.child("Gateway").attribute("value").as_string();
      net_info.firstDNS = pa_node.child("FirstDNS").attribute("value").as_string();
      net_info.secondDNS = pa_node.child("SecondDNS").attribute("value").as_string();
    }
  }
  return net_info;
}

vector<NetStruct> XmlManager::GetAllNode()
{
  return vector<NetStruct>();
}

vector<CDuiString> XmlManager::GetAllNodeName()
{
  vector<CDuiString> ret;
  for (auto node : file_.child("Info")) {
    ret.push_back(MultiToWide(node.attribute("name").as_string()));
  }
  return ret;
}

/* （工具函数） 宽字节 转 多字节 */
char * XmlManager::WideToMulti(CDuiString wide, char * multi)
{
  int wide_len = wide.GetLength();
  int multi_len = WideCharToMultiByte(CP_ACP, NULL, wide.GetData(), wide_len, NULL, 0, NULL, FALSE);
  ::WideCharToMultiByte(CP_ACP, NULL, wide.GetData(), -1, multi, multi_len, NULL, FALSE);
  return multi;
}

/* （工具函数） 多字节 转 宽字节 */
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
