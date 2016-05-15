#pragma once
#include "xml_manage_base.h"

class ConfigFileManage : public XmlManageBase
{
public:
  ConfigFileManage() {}
  ~ConfigFileManage() {}

  static ConfigFileManage* GetInstance();
  static void DestroyInstance();

private:
  static ConfigFileManage *instance_;   // 单例模式的实例 
};

