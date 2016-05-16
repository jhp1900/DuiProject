#include "xml_manager.h"
#include "pugixml\pugixml.cpp"
#include <io.h>

XmlManager::XmlManager() :
  path_and_name_(_T("")),
  is_load_(false)
{
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

void XmlManager::InsertNode(NodeStruct node_info)
{
  if (!is_load_)
    return;

  pugi::xml_node pa_node = file_.child("Info").append_child("Play");
  pa_node.append_attribute("name") = node_info.node_name.c_str();
  pugi::xml_node son_node;

  for (auto iter = node_info.attrs.begin(); iter != node_info.attrs.end(); ++iter) {
    son_node = pa_node.append_child("Attribute");
    son_node.append_attribute((*iter).first.c_str()) = (*iter).second.c_str();
  }

  file_.save_file(path_and_name_.GetData());
}

NodeStruct XmlManager::GetNodeInfo(LPCTSTR node_name)
{
  NodeStruct node_info;
  pugi::xml_node node = file_.child("Info");
  for (auto pa_node : node) {
    string play_name = pa_node.attribute("name").as_string();
    char *temp_char = new char();
    if (0 == play_name.compare(WideToMulti(node_name, temp_char))) {
      node_info.node_name = play_name;
      for (auto son_node : pa_node.children()) {
        pugi::xml_attribute attr = son_node.first_attribute();
        node_info.attrs.push_back({ attr.name(), attr.as_string() });
      }
    }
    delete temp_char;
  }
  return node_info;
}

vector<NodeStruct> XmlManager::GetAllNode()
{
  return vector<NodeStruct>();
}

vector<LPCTSTR> XmlManager::GetAllNodeName()
{
  vector<LPCTSTR> ret;
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
