#pragma once
#include "stdafx.h"
#include "pugixml\pugixml.hpp"

typedef struct _NETSTRUCT
{
	CDuiString play_name;   // 方案名
	CDuiString net_name;    // 网络名称
	CDuiString ip_address;  // IP 地址
	CDuiString netmask;     // 子网掩码
	CDuiString gateway;     // 默认网关
	CDuiString firstDNS;    // 首选DNS
	CDuiString secondDNS;   // 备用DNS
	vector<pair<CDuiString, CDuiString>> more_ip_mask;  // 可添加的多个 IP 信息
	void SetVar(LPCTSTR name, LPCTSTR value);   // 通用 修改器
	CDuiString GetVar(LPCTSTR name);            // 通用 访问器
} NETSTRUCT;

class XmlManager
{
public:
	XmlManager();
	~XmlManager();

public:
	bool LoadFile(LPCTSTR path = nullptr);
	bool SetPath(LPCTSTR path);
	bool IsLoad() { return is_load_; }

public:
	bool SetPopAttr(LPCTSTR name, bool val);
	bool GetPopAttr(LPCTSTR name);

private:
	bool SaveFile();
	pugi::xml_node GetRootNode();
	pugi::xml_node GetNode(const char * name);


public:
	char * WideToMulti(CDuiString wide, char * multi);    // 宽字符转多字节
	CDuiString MultiToWide(string multi);                 // 多字节转宽字符 

private:
	pugi::xml_document file_;
	CDuiString path_;
	bool is_load_;
};

