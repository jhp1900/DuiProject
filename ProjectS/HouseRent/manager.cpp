#include "manager.h"

/**
	���ܣ�һЩ�ؼ��ĳ�ʼ����������������Դ��䵽��Ӧ������
 */
LRESULT Manager::OnInit()
{
	::PostMessage(m_hWnd, kAM_InitOtherWndMsg, 0, 0);	// ����һ���Զ�����Ϣ����Ϣ������
	return 0;
}

/**
	���ܣ��û���Ϣ·�ɡ�������Ϣ��Դ�Ŀؼ�����·����Ϣ��Ӧ
	������const TNotifyUI& msg ��Ϣ�ṹ��
 */
void Manager::OnUserClick(const TNotifyUI& msg)
{
  if (msg.pSender->GetName() == _T("btn_home"))
    home_wnd_->GotoHomePage();
}

/**
  ���ܣ���Ӧ �Զ�����Ϣ kAM_InitOtherWndMsg �������Լ���ʼ����Դ��ʾ����
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

/* ����ʹ�ã����ݲ��� */
void Manager::OnClickTest()
{
	MessageBox(*this, TEXT("���԰�ť��Ŀǰû�в�������"), 0, 0);
}

/* ����ʹ�ã����ݲ��� */
void Manager::OnClickAction()
{
}