#pragma once
#include "stdafx.h"
#include "pugixml\pugixml.hpp"

typedef struct _NETSTRUCT
{
  CDuiString play_name;   // ������
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
  BOOL IsLoad() const { return is_load_; }
  BOOL LoadFile(CDuiString file_path, CDuiString file_name);
  void InsertNode(NETSTRUCT net_info);
  NETSTRUCT GetNodeInfo(LPCTSTR name);
  pugi::xml_node GetNode(LPCTSTR name);
  vector<NETSTRUCT> GetAllNode();
  vector<CDuiString> GetAllNodeName();
  BOOL RemoveNode(pugi::xml_node node);

public:
  char * WideToMulti(CDuiString wide, char * multi);    // ���ַ�ת���ֽ�
  CDuiString MultiToWide(string multi);                 // ���ֽ�ת���ַ� 

public:
  vector<LPCTSTR> net_attrs_;

private:
  pugi::xml_document file_;       // �����ļ�
  pugi::xml_node root_node_;      // �ļ����ڵ�
  CDuiString path_and_name_;      // ��·���ļ���
  BOOL is_load_;                  // �ļ�����״̬�� true ��ʾ���سɣ� false ��ʾ����ʧ�ܻ�û���ء�
};

