#pragma once
#include "xml_manager.h"

class ResSingleton
{
private:
	ResSingleton();

public:
	static ResSingleton* GetInstance();
	static void DestroyInstance();

	XmlManager* GetXmlManager();

private:
	static ResSingleton* instance_;
	XmlManager xml_manager_;
};

