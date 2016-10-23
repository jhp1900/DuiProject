#include "res_singleton.h"

ResSingleton * ResSingleton::instance_ = nullptr;

ResSingleton::ResSingleton()
{
	if(!xml_manager_.IsLoad())
		xml_manager_.LoadFile();
}

ResSingleton * ResSingleton::GetInstance()
{
	return instance_ ? instance_ : (instance_ = new ResSingleton());
}

void ResSingleton::DstroyInstance()
{
	if (instance_) {
		delete instance_;
		instance_ = nullptr;
	}
}

