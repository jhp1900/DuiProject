#pragma once
#include "window_impl_base.h"
#include "msg_header.h"

class LoginOrLogon : public WindowImplBase
{
public:
  LoginOrLogon(HWND hwnd_parent);
	~LoginOrLogon() {}

	DECLARE_DUIWND_INFO(_T("LoginOrLogon"), CS_DBLCLKS, _T("log_wnd.xml"))

public:
	void Init();

private:    // ��д
  virtual void Notify(TNotifyUI& msg) override;		// DuiLib �ؼ���Ϣ·��
	virtual void OnUserClick(const TNotifyUI& msg) override;

private:    // �Զ���
  void OnLogin();     // ��½
	void OnGotoLogon();	// ȥע�����
	void OnCancel();		// ȡ��ע��
	void OnLogon();			// ע��
  void SetHead();     // ����ͷ��
  void SetDataPath(); // �������ݱ���·��
  void UserNameCheck(); // �û������
  void PasswordCheck(int flag); // ��¼������

  void KillFocus(CControlUI* pSender);
  void EnterKey(CControlUI* pSender);
  void LodinPreprocess();   // ��¼Ԥ����
  bool PathIsOk();

private:
	HWND hwnd_parent_;
	CVerticalLayoutUI *login_layout_;
	CVerticalLayoutUI *logon_layout_;
  CDuiString data_path_;    // �û�����·��
  CDuiString head_path_;    // �û�ͷ��·��
  bool user_name_is_ok_;    // �û����Ƿ���� true ��ʾ���� false ��ʾ������
  bool pwd_is_ok_;          // �����Ƿ���� true ��ʾ���� false ��ʾ������
};


