#pragma once
#include "stdafx.h"
#include "pugixml\pugixml.hpp"

typedef struct _NETSTRUCT
{
	CDuiString play_name;   // ������
	CDuiString net_name;    // ��������
	CDuiString ip_address;  // IP ��ַ
	CDuiString netmask;     // ��������
	CDuiString gateway;     // Ĭ������
	CDuiString firstDNS;    // ��ѡDNS
	CDuiString secondDNS;   // ����DNS
	vector<pair<CDuiString, CDuiString>> more_ip_mask;  // ����ӵĶ�� IP ��Ϣ
	void SetVar(LPCTSTR name, LPCTSTR value);   // ͨ�� �޸���
	CDuiString GetVar(LPCTSTR name);            // ͨ�� ������
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
	char * WideToMulti(CDuiString wide, char * multi);    // ���ַ�ת���ֽ�
	CDuiString MultiToWide(string multi);                 // ���ֽ�ת���ַ� 

private:
	pugi::xml_document file_;
	CDuiString path_;
	bool is_load_;
};

