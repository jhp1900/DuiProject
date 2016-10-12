#include "xml_manager.h"
#include "..\..\third_party\pugixml\pugixml.cpp"

XmlManager::XmlManager()
{
}


XmlManager::~XmlManager()
{
}

bool XmlManager::LoadFile(const WCHAR * path /*=nullptr*/)
{
	//CDuiString file_paht;
	//if (path && wcslen(path) != 0)
	//	file_paht = path;
	//else
	//	file_paht = _T("syscfg.xml");

	//if (doc_.load_file(file_paht)) {
	//	if (doc_.child("Info"))
	//		return true;
	//	doc_.reset();
	//}

	return false;
}
