#include "xml_manage_base.h"
#include "pugixml\pugixml.cpp"

/**
 *  ���ܣ� �����û��ļ�
 *  ������ file_path �ļ�·���� file_name �ļ���
 *  ����ֵ��  �����ļ�����״̬�����سɹ����� true�� ���򷵻� false
 */
bool XmlManageBase::LoadFile(CDuiString file_path, CDuiString file_name)
{
  path_and_name_ = file_path + file_name + _T(".xml");
  is_load_ = file_.load_file(path_and_name_.GetData());

  return is_load_;
}

/**
 *  ���ܣ������ļ��������ļ���������ɾ���ĵȶ�������Ҫ���ñ����������ļ�
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
 *  ���ܣ� ���ýڵ���Һ������ҵ�����Ҫ���õĽڵ㣬����������ֵ
 *  ������ str_node �ڵ����� attr_name �������� attr_value ����ֵ��
 */
void XmlManageBase::SetAttr(char *node_name, char *attr_name, char *attr_value)
{
  NodeFd nef(node_name);
  xml_node node = file_.find_node(nef);
  node.attribute(attr_name) = attr_value;
  SaveFile();
}

/**
 *  ���ܣ� ��ȡĳ���ڵ�� ĳ������ ������ֵ
 *  ������ node_name �ڵ����� attr_name ��������
 *  ����ֵ�� CDuiString ��ȡ�ڵ�����ֵ
 */
CDuiString XmlManageBase::GetAttr(char * node_name, char * attr_name)
{
  NodeFd nef(node_name);
  xml_node node = file_.find_node(nef);
  string str_value = node.attribute(attr_name).as_string();
  return MultiToWide(str_value);
}

/**
 *  ���ܣ� Ϊ�����ڵ����һ���򵥵��ӽڵ㡣���ڼ򵥵Ķ��壬��μ� NodeStruct �ṹ��˵��
 *         ����ڵ�ṹ�� node_info �к����ӽڵ���Ϣ������ڵݹ�������ӽڵ���Ϣ��
 *         ���տ����γ�һ���ڵ�������
 *  ������ node_info �ڵ�ṹ�壬����������ӽڵ����Ϣ�� pa_node ������ڵ㷢���ڵ�
 */
bool XmlManageBase::InsertSimpleNode(NodeStruct node_info, xml_node pa_node)
{
  xml_node node = pa_node.append_child(node_info.node_name.c_str());

  for (auto iter = node_info.attrs.begin();   // �������Զ�������������������ӵ��ڵ���
  iter != node_info.attrs.end();
    ++iter) {
    node.append_attribute((*iter).first.c_str()) = (*iter).second.c_str();
  }

  for (auto iter = node_info.child_nodes.begin(); // �����ӽڵ��������������ӽڵ���ӵ����ڵ���
  iter != node_info.child_nodes.end();
    ++iter) {
    return InsertSimpleNode(*(*iter), node);
  }

  return SaveFile(); // �����޸���Ϣ
}

/**
 *  ���ܣ� ���ĳ���ڵ����Ƿ�����ĳ�ӽڵ�
 *  ������ pa_node_name ���ڵ����� node_name �ӽڵ���
 *  ����ֵ��  boolֵ�� ���� true ��ʾ���ӽڵ㶼�Ѵ���; 
 *           ���� false ��ʾ���ӽڵ�������һ��������;
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

/* �����ߺ����� ���ֽ� ת ���ֽ�*/
char * XmlManageBase::WideToMulti(CDuiString wide, char * multi)
{
  int wide_len = wide.GetLength();
  int multi_len = WideCharToMultiByte(CP_ACP, NULL, wide.GetData(), wide_len, NULL, 0, NULL, FALSE);
  ::WideCharToMultiByte(CP_ACP, NULL, wide.GetData(), -1, multi, multi_len, NULL, FALSE);
  return multi;
}

/* �����ߺ����� ���ֽ� ת ���ֽ�*/
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
