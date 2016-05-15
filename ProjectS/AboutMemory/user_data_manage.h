#pragma once
#include "xml_manage_base.h"

typedef struct _DataFileInfo  // �ļ�������Ϣ�ṹ��
{
  CDuiString path;        // ��·���ļ���
  CDuiString admin_name;  // �ļ����ˡ��û�����ȥ��׺�ļ���
  CDuiString password;    // ��½����
  CDuiString hand_img;    // ͷ��ͼƬ�ļ���
} DataFileInfo;

/**
 *  ������ UserDataManage
 *  ���ܣ� xml �ļ������ߣ��ṩ�� xml �ļ��Ĵ�����ɾ�������Լ����ļ����ݵ� ����ɾ���ġ��� ����
 */
class UserDataManage : public XmlManageBase
{
public:
  UserDataManage() {}
	~UserDataManage() {}

  static UserDataManage* GetInstance();
  static void DestroyInstance();

public:   // API ����
  bool NewFile(DataFileInfo file_info);   // �½��û��ļ�
  bool HaveNodeForAttr(
    CDuiString pa_node_name, 
    CDuiString node_name, 
    CDuiString attr_name, 
    CDuiString attr_value);
  void AddMemoryCard(NodeStruct node_info, xml_node pa_node);   // ��� ���俨 �ڵ�

  void Test();  // ����С����

private:
  static UserDataManage *instance_;   // ����ģʽ��ʵ��  
};
