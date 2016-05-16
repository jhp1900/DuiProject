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


/* 测试小函数，功能、内容不定 */
void UserDataManage::Test()
{
  //xml_node node = GetNodeForAttr(_T("Memory"),_T("MemoryObject"), _T("owner"), _T("xjh"));
  //int i = 0;
}

/**
 *  功能： 创建用户文件，并写入基本用户信息，可用于用户注册时信息的存储
 *  参数： file_info ，是一个 文件基本信息结构体，可文件基本信息的存储
 *  返回值：  bool, 文件创建、基本信息写入成功，则返回 true； 否则返回 false
 */
bool UserDataManage::NewFile(DataFileInfo file_info)
{
  ::CreateDirectory(file_info.path, NULL);  // 创建用户目录
  ::CreateDirectory(file_info.path + _T("res"), NULL);  // 创建用户资源目录

  CDuiString user_file = file_info.path + file_info.admin_name + _T(".xml");
  CDuiString template_file = CPaintManagerUI::GetResourcePath();
  template_file += _T("template.xml");

  if (!CopyFile(template_file, user_file, true)) // 通过复制模板文件创建用户文件,如果失败则返回false
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
 *  功能： 检测某个节点的子节点中，是否拥有某种属性的节点
 *  参数： pa_node_name 父节点； node_name 所查节点名; attr_name 属性名；  attr_value 属性值；
 *  返回值：  如果有这样的节点，返回 true；否则返回 false;
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
 *  功能： 添加记忆卡节点。记忆卡节点包含若干资源节点，资源节点是本记忆树的最小节点。
 *         先判断是否存在相同属性的记忆卡节点，若不存在，则添加新的；
 *         若存在，则只添加资源节点到已存在的记忆卡节点下。
 *  参数： node_info 记忆卡子树结构体； pa_node 父节点
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
