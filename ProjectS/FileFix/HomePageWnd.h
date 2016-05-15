#pragma once
#include "XMLBaseWnd.h"

class Manager : public XMLBaseWnd
{
public:
	Manager(LPCTSTR xml_path);
	~Manager();

private:
	virtual void InitWindow();							// ��ʼ������
	virtual BOOL OnClickUser(const TNotifyUI& msg);		// �û��ؼ���Ϣ��Ӧ
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);		// ��Ϣ����

private:
	void OnBtnClickAddFile();		// ��Ӧ����ļ���ť�����¼�
	void OnBtnClickAddFolder();		// ��Ӧ����ļ��а�ť�����¼�
	void OnBtnClickClear();			// ��Ӧ����б�ť����¼�
	void OnBtnClickFix();			// ��Ӧ�޸���ť�����¼�
	LRESULT OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam);	// �����ļ��Ϸ��¼�
	BOOL AddFileToList(LPCTSTR file_path_name);	// ���һ����·�����ļ������ļ��б���
	DWORD Exec(const TCHAR* name, const TCHAR* param, const TCHAR* dir, const TCHAR* relative_tool_dir);

private:
	CListUI *file_path_list_;
	CListTextElementUI *list_element_point_;
	vector<CDuiString> fail_names_;
};

