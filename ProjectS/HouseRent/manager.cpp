#include "manager.h"

/**
	功能：一些控件的初始化、将搜索到的资源填充到对应的区域
 */
LRESULT Manager::OnInit()
{
	::PostMessage(m_hWnd, kAM_InitOtherWndMsg, 0, 0);	// 发送一个自定义消息到消息队列中
	return 0;
}

/**
	功能：用户消息路由。根据消息来源的控件名来路由消息响应
	参数：const TNotifyUI& msg 消息结构体
 */
void Manager::OnUserClick(const TNotifyUI& msg)
{
  if (msg.pSender->GetName() == _T("btn_home"))
    home_wnd_->GotoHomePage();
}

/**
  功能：响应 自定义消息 kAM_InitOtherWndMsg ，创建以及初始化资源显示窗体
 */
LRESULT Manager::OnInitOtherWndMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
  log_wnd_.reset(new LoginOrLogon(*this));
  log_wnd_->Init();
  log_wnd_->ShowWindow(true);

	return LRESULT();
}

LRESULT Manager::OnLoginMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
  CContainerUI *log_container = static_cast<CContainerUI *>(m_PaintManager.FindControl(_T("log_container")));
  CContainerUI *home_container = static_cast<CContainerUI *>(m_PaintManager.FindControl(_T("home_container")));

  log_container->SetVisible(false);
  home_container->SetVisible(true);

  home_wnd_.reset(new HomeWnd());
  home_wnd_->Init(*this);
  home_wnd_->ShowWindow(true);

  log_wnd_->Close();
  log_wnd_.release();

  return LRESULT();
}

/* 测试使用，内容不定 */
void Manager::OnClickTest()
{
	MessageBox(*this, TEXT("测试按钮，目前没有测试内容"), 0, 0);
}

/* 测试使用，内容不定 */
void Manager::OnClickAction()
{
}