#pragma once
#include "stdafx.h"
#include "window_impl_base.h"
#include "msg_header.h"

class HomeWnd : public WindowImplBase
{
public:
	HomeWnd() : pa_hwnd_(nullptr) {}
	~HomeWnd() {}

	DECLARE_DUIWND_INFO(_T("HomeWnd"), CS_DBLCLKS, _T("home_wnd.xml"))

  BEGIN_DUIMSG_MAP(HomeWnd)
    DUIMSG_HANDLER(kAM_AddObjectMsg, OnAddObjectMsg)
  END_DUIMSG_MAP()

public:
	void Init(HWND pa_hwnd);

private:
  virtual void Notify(TNotifyUI& msg) override;
  virtual void OnUserClick(const TNotifyUI& msg) override;

private:
  LRESULT OnAddObjectMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

  void FullList();    // 填充列表信息
  void CallAddObjectWnd();    // 调用添加记忆对象的界面
  void OnClickElemMenu(const TNotifyUI &msg);

private:
  HWND pa_hwnd_;
  PDUI_LIST list_;
  PDUI_VERTLAYOUT list_elem_;
};

