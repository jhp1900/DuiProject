#pragma once
#include "stdafx.h"
#include "window_impl_base.h"

class HomeWnd : public WindowImplBase
{
public:
	HomeWnd() {}
	~HomeWnd() {}

	DECLARE_DUIWND_INFO(_T("HomeWnd"), CS_DBLCLKS, _T("home_wnd.xml"))

private:
  virtual LRESULT OnInit() override;
  virtual void Notify(TNotifyUI& msg);				// ·�ɿؼ���Ϣ����Ӧ
  virtual void OnUserClick(const TNotifyUI& msg) override;		// �û��ؼ���Ϣ��Ӧ

private:
  void OnSelectChange(const CControlUI *pSender);

public:
	void Init(HWND hwnd_parent);
  void GotoHomePage();

private:
  CTabLayoutUI *tab_layout_;
};

