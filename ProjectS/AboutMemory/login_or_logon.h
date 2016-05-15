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

private:    // 覆写
  virtual void Notify(TNotifyUI& msg) override;		// DuiLib 控件消息路由
	virtual void OnUserClick(const TNotifyUI& msg) override;

private:    // 自定义
  void OnLogin();     // 登陆
	void OnGotoLogon();	// 去注册界面
	void OnCancel();		// 取消注册
	void OnLogon();			// 注册
  void SetHead();     // 设置头像
  void SetDataPath(); // 设置数据保存路径
  void UserNameCheck(); // 用户名检测
  void PasswordCheck(int flag); // 登录密码检测

  void KillFocus(CControlUI* pSender);
  void EnterKey(CControlUI* pSender);
  void LodinPreprocess();   // 登录预处理
  bool PathIsOk();

private:
	HWND hwnd_parent_;
	CVerticalLayoutUI *login_layout_;
	CVerticalLayoutUI *logon_layout_;
  CDuiString data_path_;    // 用户数据路径
  CDuiString head_path_;    // 用户头像路径
  bool user_name_is_ok_;    // 用户名是否合理， true 表示合理， false 表示不合理
  bool pwd_is_ok_;          // 密码是否合理， true 表示合理， false 表示不合理
};


