#include "user_data_manage.h"

UserDataManage* UserDataManage::instance_ = nullptr;

UserDataManage * UserDataManage::GetInstance()
{
  return instance_ ? instance_ : (instance_ = new UserDataManage());
}

void UserDataManage::DestroyInstance()
{
  if (instance_) {
    delete instance_;
    instance_ = nullptr;
  }
}


/* ����С���������ܡ����ݲ��� */
void UserDataManage::Test()
{
  //xml_node node = GetNodeForAttr(_T("Memory"),_T("MemoryObject"), _T("owner"), _T("xjh"));
  //int i = 0;
}

/**
 *  ���ܣ� �����û��ļ�����д������û���Ϣ���������û�ע��ʱ��Ϣ�Ĵ洢
 *  ������ file_info ����һ�� �ļ�������Ϣ�ṹ�壬���ļ�������Ϣ�Ĵ洢
 *  ����ֵ��  bool, �ļ�������������Ϣд��ɹ����򷵻� true�� ���򷵻� false
 */
bool UserDataManage::NewFile(DataFileInfo file_info)
{
  ::CreateDirectory(file_info.path, NULL);  // �����û�Ŀ¼
  ::CreateDirectory(file_info.path + _T("res"), NULL);  // �����û���ԴĿ¼

  CDuiString user_file = file_info.path + file_info.admin_name + _T(".xml");
  CDuiString template_file = CPaintManagerUI::GetResourcePath();
  template_file += _T("template.xml");

  if (!CopyFile(template_file, user_file, true)) // ͨ������ģ���ļ������û��ļ�,���ʧ���򷵻�false
    return false;
  if (!CopyFile(file_info.hand_img, file_info.path + _T("res\\head.png"), true))
    return false;

  if (LoadFile(file_info.path, file_info.admin_name)) {
    char out_value[MAX_PATH] = { 0 };
    WideToMulti(file_info.admin_name, out_value);
    SetAttr("UserName", "value", out_value);

    WideToMulti(file_info.password, out_value);
    SetAttr("Password", "value", out_value);
  }

  return true;
}

/**
 *  ���ܣ� ���ĳ���ڵ���ӽڵ��У��Ƿ�ӵ��ĳ�����ԵĽڵ�
 *  ������ pa_node_name ���ڵ㣻 node_name ����ڵ���; attr_name ��������  attr_value ����ֵ��
 *  ����ֵ��  ����������Ľڵ㣬���� true�����򷵻� false;
 */
bool UserDataManage::HaveNodeForAttr(
  CDuiString pa_node_name,
  CDuiString node_name,
  CDuiString attr_name,
  CDuiString attr_value)
{
  xml_node ret_node;
  ret_node = GetNodeForAttr(pa_node_name, node_name, attr_name, attr_value);

  return ret_node != nullptr;
}

/**
 *  ���ܣ� ��Ӽ��俨�ڵ㡣���俨�ڵ����������Դ�ڵ㣬��Դ�ڵ��Ǳ�����������С�ڵ㡣
 *         ���ж��Ƿ������ͬ���Եļ��俨�ڵ㣬�������ڣ�������µģ�
 *         �����ڣ���ֻ�����Դ�ڵ㵽�Ѵ��ڵļ��俨�ڵ��¡�
 *  ������ node_info ���俨�����ṹ�壻 pa_node ���ڵ�
 */
void UserDataManage::AddMemoryCard(NodeStruct node_info, xml_node pa_node)
{
  LPCTSTR pa_node_name = MultiToWide(pa_node.name());
  LPCTSTR node_name = MultiToWide(node_info.node_name);
  LPCTSTR attr_name = MultiToWide(node_info.attrs.at(0).first);
  LPCTSTR attr_value = MultiToWide(node_info.attrs.at(0).second);

  xml_node have_node;
  have_node = GetNodeForAttr(pa_node_name, node_name, attr_name, attr_value);
  if (!have_node) {
    InsertSimpleNode(node_info, pa_node);
  } else {
    for (auto child = node_info.child_nodes.begin()
      ; child != node_info.child_nodes.end()
      ; ++child) {
      InsertSimpleNode(**child, have_node);
    }
  }
}
