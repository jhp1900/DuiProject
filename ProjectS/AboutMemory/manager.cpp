#include "manager.h"
#include "config_file_manage.h"

Manager* Manager::instance_ = nullptr;

Manager::Manager() 
  : log_wnd_(nullptr)
  , home_wnd_(nullptr)
  , owner_(_T(""))
{}

Manager * Manager::GetInstance()
{
  return instance_ ? instance_ : (instance_ = new Manager());
}

void Manager::DestroyInstance()
{
  if (instance_) {
    delete instance_;
    instance_ = nullptr;
  }
}

LRESULT Manager::OnInit()
{
  ConfigFileManage *config_manager = ConfigFileManage::GetInstance();
  LPCTSTR config_file_path = CPaintManagerUI::GetResourcePath();

  if (!config_manager->LoadFile(config_file_path, _T("config_data"))) {   // ���������ļ�
    MessageBox(NULL, _T("�����ļ�����ʧ�ܣ����������ļ��Ƿ���ȷ��"), _T("Error"), MB_OK);
    Close();
    return 0;
  }

  ::PostMessage(m_hWnd, kAM_InitOtherWndMsg, 0, 0);	// ����һ���Զ�����Ϣ����Ϣ������
  return 0;
}

LRESULT Manager::OnInitOtherWndMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	log_wnd_ = new LoginOrLogon(*this);
	log_wnd_->Init();
	log_wnd_->ShowWindow(true);
  return 0;
}

/**
 *  ���ܣ� ��Ӧ��½���巢���ĵ�½��Ϣ�����رյ�½���壬���������塣
 */
LRESULT Manager::OnLoginMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	CContainerUI *log_container = static_cast<CContainerUI *>(m_PaintManager.FindControl(_T("log_container")));
	CContainerUI *home_container = static_cast<CContainerUI *>(m_PaintManager.FindControl(_T("home_container")));
  PDUI_BUTTON head = static_cast<PDUI_BUTTON>(m_PaintManager.FindControl(_T("head")));

  head->SetBkImage(user_info_.hand_img);

	log_container->SetVisible(false);
	home_container->SetVisible(true);

	home_wnd_ = new HomeWnd;
	home_wnd_->Init(*this);
	home_wnd_->ShowWindow(true);

	log_wnd_->Close();
	return 0;
}