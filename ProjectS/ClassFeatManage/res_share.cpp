#include "res_share.h"

ResShare * ResShare::instance_ = nullptr;

ResShare::ResShare()
{
  user_manager_ = new UserManage;
}

ResShare * ResShare::GetInstance()
{
  return instance_ ? instance_ : (instance_ = new ResShare);
}

void ResShare::DestroyInstance()
{
  if (instance_) {
    delete instance_;
    instance_ = nullptr;
  }
}

