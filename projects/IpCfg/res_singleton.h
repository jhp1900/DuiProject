#pragma once
#include "xml_manager.h"

class ResSingleton
{
private:
	ResSingleton();

public:
	static ResSingleton * GetInstance();
	static void DstroyInstance();
	XmlManager & GetXmlMamager() { return xml_manager_; }

private:
	static ResSingleton * instance_;
	XmlManager xml_manager_;
};

