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
  ::PostMessage(m_hWnd, kAM_InitOtherWndMsg, 0, 0);	// 发送一个自定义消息到消息队列中
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
 *  功能： 响应登陆窗体发来的登陆消息，即关闭登陆窗体，开启主窗体。
 */
LRESULT Manager::OnLoginMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
  ::ShowWindow(m_hWnd, SW_SHOW);
  ResShare *share = ResShare::GetInstance();
  CDuiString user_type = share->GetLogInfo().type;

  /* 根据登录者类型的不同，创建不同的界面 */
  if (user_type == _T("学生")) {
    student_win_ = new StudentWin;
    student_win_->Init(*this);
    student_win_->ShowWindow(true);
  } else if (user_type == _T("教师")) {
    teachter_win_ = new TeacherWin;
    teachter_win_->Init(*this);
    teachter_win_->ShowWindow(true);
  } else if (user_type == _T("管理员")) {
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
