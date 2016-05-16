#pragma once
#include "stdafx.h"
#include "pugixml\pugixml.hpp"


using pugi::xml_document;
using pugi::xml_node;
using pugi::xml_attribute;

/**
 *  �ṹ������ NodeStruct �ڵ�ṹ��
 *  ���ܣ� ���ڹ���һ���ڵ㣬�ڵ㱾����һ���򵥵�ֻ�� �ڵ��������� ����Ľڵ�
 *        eg: <NodeName owner="" time="" type="" path="" />
 *        �����ṹ����԰���һ�� �ӽڵ�����������Ҳ���γ���һ�Žڵ�����
 */
typedef struct _NodeStruct
{
  string node_name;   // �ڵ���
  vector<pair<string, string> > attrs;  // ���Զ�����,firstΪ��������secondΪ����ֵ
  vector<struct _NodeStruct *> child_nodes;   // �ӽڵ�����
} NodeStruct;

/**
 *  ������ NodeFd
 *  ���ܣ� ���� find_node() ���Һ����Ĳ�������
 */
class NodeFd
{
public:
  NodeFd() {}
  NodeFd(string name) : node_name(name) {}

  void SetNodeName(string name) { node_name = name; }

  bool operator()(xml_node node) const
  {
    return (string(node.name()) == node_name);
  }

private:
  string node_name;
};

/**
 *  ������ XmlManageBase
 *  ���ܣ� XML �ļ��������ࡣ
 *  ˵���� ����ͨ���� pugixml ���߿��ʹ�ã���ʵ�ֱ������ XML �ļ��ĸ��ֲ���
 *         ������ҪΪ�� XML �ļ��Ľڵ㡢���Ե�  ����ɾ���ġ��� �Ȳ�����ʵ�֡�
 */
class XmlManageBase
{
public:
  XmlManageBase() {}
  ~XmlManageBase() {}

public:     // API ����
  bool LoadFile(CDuiString file_path, CDuiString file_name); // �����ļ�
  bool IsLoad() const { return is_load_; }    // �ļ������Ƿ�ɹ�
  xml_node FindNode(char *node_name);     // ��ȡĳ���ڵ�

  xml_node GetNodeForAttr(    // �������ԣ���ȡĳ���ڵ�
    CDuiString pa_node_name, 
    CDuiString node_name, 
    CDuiString attr_name, 
    CDuiString attr_value);

  vector<xml_node> GetAllChild(CDuiString pa_node_name);    // �õ�ĳ�ڵ�������ӽڵ�
  void SetAttr(char *node_name, char *attr_name, char *attr_value);// ����ĳ���ڵ��ĳ������
  CDuiString GetAttr(char *str_node, char *attr_name);            // ��ȡĳ���ڵ��ĳ������
  void InsertSimpleNode(NodeStruct node_info, xml_node pa_node);  // ���һ���򵥽ڵ�
  bool IsHave(char *pa_node_name, char *node_name);     // ���ĳ�����ڵ����Ƿ�ӵ��ĳ���ӽڵ�

public:     // ���ߺ�������
  char * WideToMulti(CDuiString wide, char * multi);    // ���ַ�ת���ֽ�
  CDuiString MultiToWide(string multi);                 // ���ֽ�ת���ַ� 

protected:
  xml_document file_;  // �����ļ�
  xml_node root_node_;      // �ļ����ڵ�
  CDuiString path_and_name_;  // ��·���ļ���
  bool is_load_;      // �ļ�����״̬�� true ����سɹ��� false �����ʧ�ܻ�û����
};

