#pragma once
#include "xml_manage_base.h"

typedef struct _UserInfoStruct {
  CDuiString id;
  CDuiString name;
  CDuiString sex;
  CDuiString pwd;
  CDuiString type;
  CDuiString college;
  CDuiString subject;
  CDuiString classes;
  CDuiString room;
  CDuiString tel;
}UserInfoStruct;

class UserManage : public XmlManageBase
{
public:
  UserManage() {}
  ~UserManage() {}

public:
  bool IsLoginOk(LPCTSTR user_name, LPCTSTR user_id, 
    LPCTSTR password, LPCTSTR user_type);
  vector<UserInfoStruct> GetSomeType(LPCTSTR some_type);
  bool InsertNode(NodeStruct node_info);
  bool IsHaveID(LPCTSTR id);
  bool DelNode(LPCTSTR id);
  UserInfoStruct GetOneNode(LPCTSTR id);
};

