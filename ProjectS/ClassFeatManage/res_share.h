#pragma once
#include "user_manage.h"

class ResShare
{
public:
  ResShare();
  ~ResShare() {}

  static ResShare *GetInstance();
  static void DestroyInstance();

public:
  UserManage * GetUserManager() const { return user_manager_; }

  void SetLogInfo(UserInfoStruct log_info) { log_info_ = log_info; }
  UserInfoStruct GetLogInfo() const { return log_info_; }

  void SetEditInfo(UserInfoStruct edit_info) { edit_info_ = edit_info; }
  UserInfoStruct GetEditInfo() const { return edit_info_; }

private:
  static ResShare *instance_;   // 单例模式的实例  

  UserManage *user_manager_;
  UserInfoStruct log_info_;
  UserInfoStruct edit_info_;
};

