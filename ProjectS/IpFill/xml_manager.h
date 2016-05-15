#pragma once
#include "stdafx.h"
#include "pugixml\pugixml.hpp"

typedef struct _NodeStruct
{
  string node_name;   // �ڵ���
  vector<pair<string, string>> attrs;  // ���Զ�����, firstΪ�������� secondΪ����ֵ
} NodeStruct;

class XmlManager
{
public:
  XmlManager();
  ~XmlManager();

public:
  BOOL IsLoad() const { return is_load_; }
  BOOL LoadFile(CDuiString file_path, CDuiString file_name);
  void InsertNode(NodeStruct node_info);
  NodeStruct GetNodeInfo(LPCTSTR node_name);
  vector<NodeStruct> GetAllNode();
  vector<LPCTSTR> GetAllNodeName();

public:
  char * WideToMulti(CDuiString wide, char * multi);    // ���ַ�ת���ֽ�
  CDuiString MultiToWide(string multi);                 // ���ֽ�ת���ַ� 

private:
  pugi::xml_document file_;       // �����ļ�
  pugi::xml_node root_node_;      // �ļ����ڵ�
  CDuiString path_and_name_;      // ��·���ļ���
  BOOL is_load_;                  // �ļ�����״̬�� true ��ʾ���سɣ� false ��ʾ����ʧ�ܻ�û���ء�
};

