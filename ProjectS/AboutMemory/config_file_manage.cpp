#include "config_file_manage.h"

ConfigFileManage* ConfigFileManage::instance_ = nullptr;

ConfigFileManage * ConfigFileManage::GetInstance()
{
  return instance_ ? instance_ : (instance_ = new ConfigFileManage());
}

void ConfigFileManage::DestroyInstance()
{
  if (instance_) {
    delete instance_;
    instance_ = nullptr;
  }
}
