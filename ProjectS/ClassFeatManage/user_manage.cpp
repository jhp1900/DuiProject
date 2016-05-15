#include "user_manage.h"

bool UserManage::IsLoginOk(LPCTSTR user_name, LPCTSTR user_id,
  LPCTSTR password, LPCTSTR user_type)
{
  CDuiString name = _T("");
  CDuiString id = _T("");
  CDuiString pwd = _T("");
  CDuiString type = _T("");
  root_node_ = file_.child("User");
  return true;

  for (auto node = root_node_.first_child(); node; node = node.next_sibling()) {
    name = MultiToWide(node.attribute("name").as_string());
    id = MultiToWide(node.attribute("id").as_string());
    pwd = MultiToWide(node.attribute("pwd").as_string());
    type = MultiToWide(node.attribute("type").as_string());
    if (name == user_name && id == user_id 
      && pwd == password && type == user_type)
      return true;
  }
  return false;
}

vector<UserInfoStruct> UserManage::GetSomeType(LPCTSTR some_type)
{
  vector<UserInfoStruct> ret;
  for (auto iter = root_node_.first_child(); iter; iter = iter.next_sibling()) {
    CDuiString type = MultiToWide(iter.attribute("type").as_string());
    if (type == some_type) {
      UserInfoStruct info;
      info.id = MultiToWide(iter.attribute("id").as_string());
      info.name = MultiToWide(iter.attribute("name").as_string());
      info.sex = MultiToWide(iter.attribute("sex").as_string());
      info.type = type;
      info.college = MultiToWide(iter.attribute("college").as_string());
      info.subject = MultiToWide(iter.attribute("subject").as_string());
      info.classes = MultiToWide(iter.attribute("classes").as_string());
      info.room = MultiToWide(iter.attribute("room").as_string());;
      info.tel = MultiToWide(iter.attribute("tel").as_string());
      ret.push_back(info);
    }
  }
  return ret;
}

bool UserManage::InsertNode(NodeStruct node_info)
{
  return InsertSimpleNode(node_info, root_node_);
}

bool UserManage::IsHaveID(LPCTSTR id)
{
  for (auto iter = root_node_.first_child(); iter; iter = iter.next_sibling())
    if (MultiToWide(iter.attribute("id").as_string()) == id)
      return true;

  return false;
}

bool UserManage::DelNode(LPCTSTR id)
{
  xml_node node = GetNodeForAttr(_T("User"), _T("UserData"), _T("id"), id);
  return root_node_.remove_child(node) && SaveFile();  // É¾³ý²¢±£´æ
}

UserInfoStruct UserManage::GetOneNode(LPCTSTR id)
{
  UserInfoStruct ret;
  xml_node node = GetNodeForAttr(_T("User"), _T("UserData"), _T("id"), id);
  ret.id = id;
  ret.name = MultiToWide(node.attribute("name").as_string());
  ret.sex = MultiToWide(node.attribute("sex").as_string());
  ret.type = MultiToWide(node.attribute("type").as_string());
  ret.pwd = MultiToWide(node.attribute("pwd").as_string());
  ret.college = MultiToWide(node.attribute("college").as_string());
  ret.subject = MultiToWide(node.attribute("subject").as_string());
  
  return ret;
}
