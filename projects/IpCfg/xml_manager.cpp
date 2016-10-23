#include "xml_manager.h"
#include "pugixml\pugixml.cpp"
#include <io.h>

void _NETSTRUCT::SetVar(LPCTSTR name, LPCTSTR value)
{
	if (name == _T("NetName"))
		net_name = value;
	else if (name == _T("IPAddress"))
		ip_address = value;
	else if (name == _T("Netmask"))
		netmask = value;
	else if (name == _T("Gateway"))
		gateway = value;
	else if (name == _T("FirstDNS"))
		firstDNS = value;
	else if (name == _T("SecondDNS"))
		secondDNS = value;
}

CDuiString _NETSTRUCT::GetVar(LPCTSTR name)
{
	if (name == _T("NetName"))
		return net_name;
	else if (name == _T("IPAddress"))
		return ip_address;
	else if (name == _T("Netmask"))
		return netmask;
	else if (name == _T("Gateway"))
		return gateway;
	else if (name == _T("FirstDNS"))
		return firstDNS;
	else if (name == _T("SecondDNS"))
		return secondDNS;
}

XmlManager::XmlManager() 
	: is_load_(false)
{
}

XmlManager::~XmlManager()
{
}

bool XmlManager::LoadFile(LPCTSTR path)
{
	if (path && wcslen(path) != 0)
		path_ = path;
	else
		path_ = _T("syscfg.xml");

	if (file_.load_file(path_)) {
		if (file_.child("Info"))
			return is_load_ = true;
		file_.reset();
	}
	return is_load_ = false;
}

bool XmlManager::SetPath(LPCTSTR path)
{
	if (path && wcslen(path) != 0) {
		path_ = path;
		return true;
	}
	return false;
}

bool XmlManager::SetPopAttr(LPCTSTR name, bool val)
{
	char temp[MAX_PATH] = { 0 };
	pugi::xml_node node = GetNode(WideToMulti(name, temp));

	pugi::xml_attribute attr = node.attribute("value");
	if (!attr) 
		attr = node.append_attribute("value");
	attr = val;
	
	return SaveFile();
}

bool XmlManager::GetPopAttr(LPCTSTR name)
{
	char temp[MAX_PATH] = { 0 };
	pugi::xml_node node = GetNode(WideToMulti(name, temp));

	pugi::xml_attribute attr = node.attribute("value");
	if (!attr) {
		attr = node.append_attribute("value");
		attr = false;
		SaveFile();
	}
	return attr.as_bool();
}

bool XmlManager::SaveFile()
{
	return file_.save_file(path_.GetData(), "  ", pugi::format_indent | pugi::format_write_bom, pugi::encoding_utf8);
}

pugi::xml_node XmlManager::GetRootNode()
{
	pugi::xml_node node = file_.child("Info");
	if (!node) {
		if (file_.first_child().type() != pugi::node_declaration) {
			file_.append_child(pugi::node_declaration).set_name("xml version=\"1.0\" encoding=\"utf-8\"");
		}
		node = file_.append_child("Info");
		node.append_attribute("version") = "1.0";
	}
	return node;
}

pugi::xml_node XmlManager::GetNode(const char * name)
{
	pugi::xml_node root = GetRootNode();
	pugi::xml_node node = root.child(name);
	if (!node)
		node = GetRootNode().append_child(name);
	return node;
}

/* （工具函数） 宽字节 转 多字节 */
char * XmlManager::WideToMulti(CDuiString wide, char * multi)
{
	ZeroMemory(multi, MAX_PATH);
	int wide_len = wide.GetLength();
	int multi_len = WideCharToMultiByte(CP_ACP, NULL, wide.GetData(), wide_len, NULL, 0, NULL, FALSE);
	::WideCharToMultiByte(CP_ACP, NULL, wide.GetData(), -1, multi, multi_len, NULL, FALSE);
	return multi;
}

/* （工具函数） 多字节 转 宽字节 */
CDuiString XmlManager::MultiToWide(string multi)
{
	int multi_len = multi.length();
	int wide_len = ::MultiByteToWideChar(CP_ACP, 0, multi.c_str(), -1, NULL, 0);
	wchar_t *wide_str;
	wide_str = new wchar_t[wide_len + 1];
	memset(wide_str, 0, (wide_len + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, multi.c_str(), -1, (LPWSTR)wide_str, wide_len);

	CDuiString ret = (wchar_t*)wide_str;
	delete wide_str;

	return ret;
}
