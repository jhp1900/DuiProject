#pragma once
#include "stdafx.h"

class XMLBaseWnd : public CWindowWnd, public INotifyUI
{
public:
	XMLBaseWnd(LPCTSTR xml_name);
	~XMLBaseWnd();

public:
	virtual LPCTSTR GetWindowClassName() const;		// ��д���麯���� ��ȡ��������
	virtual CDuiString GetSkinFile();				// ��д���麯���� ��ȡ���棨.xml���ļ���
	virtual CDuiString GetSkinFolder();				// ��д���麯���� ��ȡ�����ļ�·��

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);		// ��Ϣ����
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandle);		// �������庯��
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);	// һ������������Ϊ��ʵ�ִ��ڱ��������Ի�;
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);		// ��Ӧ�ر���Ϣ
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);		// ��Ӧ������Ϣ
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);	// ���ڵ���󻯡���С������ԭ

	virtual void InitWindow();				// ��ʼ������
	virtual void Notify(TNotifyUI& msg);	// ·�ɿؼ���Ϣ����Ӧ
	virtual BOOL OnClickSys(const TNotifyUI& msg);		// ϵͳ�ؼ���Ϣ��Ӧ 
	virtual BOOL OnClickUser(const TNotifyUI& msg);		// �û��ؼ���Ϣ��Ӧ

public:
	CPaintManagerUI *paint_manager_;	// ���ƹ�����

private:
	CDuiString xml_name_;				// �洢 XML �ļ���
};

