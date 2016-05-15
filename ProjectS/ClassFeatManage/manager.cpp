#include "manager.h"
#include "res_share.h"

Manager* Manager::instance_ = nullptr;

Manager::Manager() 
  : log_wnd_(nullptr)
  , admin_win_(nullptr)
  , teachter_win_(nullptr)
  , student_win_(nullptr)
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
  ::PostMessage(m_hWnd, kAM_InitOtherWndMsg, 0, 0);	// ����һ���Զ�����Ϣ����Ϣ������
  return 0;
}

LRESULT Manager::OnInitOtherWndMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
  ::ShowWindow(m_hWnd, SW_HIDE);
  log_wnd_ = new LogWnd(*this);
  log_wnd_->DoModal();

  return 0;
}

/**
 *  ���ܣ� ��Ӧ��½���巢���ĵ�½��Ϣ�����رյ�½���壬���������塣
 */
LRESULT Manager::OnLoginMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
  ::ShowWindow(m_hWnd, SW_SHOW);
  ResShare *share = ResShare::GetInstance();
  CDuiString user_type = share->GetLogInfo().type;

  /* ���ݵ�¼�����͵Ĳ�ͬ��������ͬ�Ľ��� */
  if (user_type == _T("ѧ��")) {
    student_win_ = new StudentWin;
    student_win_->Init(*this);
    student_win_->ShowWindow(true);
  } else if (user_type == _T("��ʦ")) {
    teachter_win_ = new TeacherWin;
    teachter_win_->Init(*this);
    teachter_win_->ShowWindow(true);
  } else if (user_type == _T("����Ա")) {
    admin_win_ = new AdminWin;
    admin_win_->Init(*this);
    admin_win_->ShowWindow(true);
  }

	return 0;
}

LRESULT Manager::OnLogCloseMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
  Close();
  return LRESULT();
}
