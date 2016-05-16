#pragma once
#include "xml_manage_base.h"

typedef struct _DataFileInfo  // 文件基本信息结构体
{
  CDuiString path;        // 带路径文件名
  CDuiString admin_name;  // 文件主人、用户名、去后缀文件名
  CDuiString password;    // 登陆密码
  CDuiString hand_img;    // 头像图片文件名
} DataFileInfo;

/**
 *  类名： UserDataManage
 *  功能： xml 文件管理者，提供对 xml 文件的创建、删除操作以及对文件内容的 增、删、改、查 操作
 */
class UserDataManage : public XmlManageBase
{
public:
  UserDataManage() {}
	~UserDataManage() {}

  static UserDataManage* GetInstance();
  static void DestroyInstance();

public:   // API 部分
  bool NewFile(DataFileInfo file_info);   // 新建用户文件
  bool HaveNodeForAttr(
    CDuiString pa_node_name, 
    CDuiString node_name, 
    CDuiString attr_name, 
    CDuiString attr_value);
  void AddMemoryCard(NodeStruct node_info, xml_node pa_node);   // 添加 记忆卡 节点

  void Test();  // 测试小函数

private:
  static UserDataManage *instance_;   // 单例模式的实例  
};
