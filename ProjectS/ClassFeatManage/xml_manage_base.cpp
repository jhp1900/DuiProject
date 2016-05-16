#include "xml_manage_base.h"
#include "pugixml\pugixml.cpp"

/**
 *  功能： 加载用户文件
 *  参数： file_path 文件路径； file_name 文件名
 *  返回值：  返回文件加载状态，加载成功返回 true， 否则返回 false
 */
bool XmlManageBase::LoadFile(CDuiString file_path, CDuiString file_name)
{
  path_and_name_ = file_path + file_name + _T(".xml");
  is_load_ = file_.load_file(path_and_name_.GetData());

  return is_load_;
}

/**
 *  功能：保存文件。当对文件进行增、删、改等动作后，需要调用本函数保存文件
 */
bool XmlManageBase::SaveFile()
{
  return file_.save_file((path_and_name_).GetData());
}

xml_node XmlManageBase::FindNode(char * node_name)
{
  NodeFd nef(node_name);
  return file_.find_node(nef);
}

xml_node XmlManageBase::GetNodeForAttr(
  CDuiString pa_name, 
  CDuiString node_name, 
  CDuiString attr_name, 
  CDuiString attr_value)
{
  char pa_node[MAX_PATH] = { 0 };
  char node[MAX_PATH] = { 0 };
  char attr[MAX_PATH] = { 0 };
  char value[MAX_PATH] = { 0 };

  WideToMulti(pa_name, pa_node);
  WideToMulti(node_name, node);
  WideToMulti(attr_name, attr);
  WideToMulti(attr_value, value);

  NodeFd nef(pa_node);
  xml_node root = file_.find_node(nef);

  return root.find_child_by_attribute(node, attr, value);
}

/**
 *  
 */
vector<xml_node> XmlManageBase::GetAllChild(CDuiString pa_node_name)
{
  char pa_name[MAX_PATH] = { 0 };
  WideToMulti(pa_node_name, pa_name);

  NodeFd nef(pa_name);
  xml_node pa_node = file_.find_node(nef);
  vector<xml_node> ret_node;

  for (auto iter = pa_node.first_child(); iter; iter = iter.next_sibling())
    ret_node.push_back(iter);

  return ret_node;
}

/**
 *  功能： 调用节点查找函数，找到所需要设置的节点，设置其属性值
 *  参数： str_node 节点名； attr_name 属性名； attr_value 属性值；
 */
void XmlManageBase::SetAttr(char *node_name, char *attr_name, char *attr_value)
{
  NodeFd nef(node_name);
  xml_node node = file_.find_node(nef);
  node.attribute(attr_name) = attr_value;
  SaveFile();
}

/**
 *  功能： 获取某个节点的 某个属性 的属性值
 *  参数： node_name 节点名； attr_name 属性名；
 *  返回值： CDuiString 所取节点属性值
 */
CDuiString XmlManageBase::GetAttr(char * node_name, char * attr_name)
{
  NodeFd nef(node_name);
  xml_node node = file_.find_node(nef);
  string str_value = node.attribute(attr_name).as_string();
  return MultiToWide(str_value);
}

/**
 *  功能： 为给定节点添加一个简单的子节点。至于简单的定义，请参见 NodeStruct 结构体说明
 *         如果节点结构体 node_info 中含有子节点信息，则会在递归中添加子节点信息，
 *         最终可以形成一个节点子树。
 *  参数： node_info 节点结构体，含有所需添加节点的信息； pa_node 所需天节点发父节点
 */
bool XmlManageBase::InsertSimpleNode(NodeStruct node_info, xml_node pa_node)
{
  xml_node node = pa_node.append_child(node_info.node_name.c_str());

  for (auto iter = node_info.attrs.begin();   // 遍历属性对向量，将所有属性添加到节点中
  iter != node_info.attrs.end();
    ++iter) {
    node.append_attribute((*iter).first.c_str()) = (*iter).second.c_str();
  }

  for (auto iter = node_info.child_nodes.begin(); // 遍历子节点向量，将所有子节点添加到本节点下
  iter != node_info.child_nodes.end();
    ++iter) {
    return InsertSimpleNode(*(*iter), node);
  }

  return SaveFile(); // 保存修改信息
}

/**
 *  功能： 检测某父节点下是否已有某子节点
 *  参数： pa_node_name 父节点名； node_name 子节点名
 *  返回值：  bool值， 返回 true 表示父子节点都已存在; 
 *           返回 false 表示父子节点自少有一个不存在;
 */
bool XmlManageBase::IsHave(char * pa_node_name, char * node_name)
{
  NodeFd nef(pa_node_name);
  xml_node pa_node = file_.find_node(nef);

  if (pa_node == nullptr)
    return false;

  for (auto iter = pa_node.first_child()
    ; iter
    ; iter = iter.next_sibling()) {
    if (strcmp(iter.name(), node_name) == 0)
      return true;
  }

  return false;
}

/* （工具函数） 宽字节 转 多字节*/
char * XmlManageBase::WideToMulti(CDuiString wide, char * multi)
{
  int wide_len = wide.GetLength();
  int multi_len = WideCharToMultiByte(CP_ACP, NULL, wide.GetData(), wide_len, NULL, 0, NULL, FALSE);
  ::WideCharToMultiByte(CP_ACP, NULL, wide.GetData(), -1, multi, multi_len, NULL, FALSE);
  return multi;
}

/* （工具函数） 多字节 转 宽字节*/
CDuiString XmlManageBase::MultiToWide(string multi)
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
