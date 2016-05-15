#include "log_wnd.h"
#include "res_share.h"


LogWnd::LogWnd(HWND pa_hwnd) 
  : pa_hwnd_(pa_hwnd)
  , radio_type_(_T("学生"))
{
}

bool LogWnd::DoModal()
{
  if (!Create(pa_hwnd_, _T(""), WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_ACCEPTFILES))
    return false;

  CenterWindow(pa_hwnd_);
  ShowInit();

  return ShowModal() != 0;
}

void LogWnd::Notify(TNotifyUI & msg)
{
  if (msg.sType == _T("click"))
    OnClick(msg.pSender);
  else if (msg.sType == _T("selectchanged"))
    OnRadioChange(msg.pSender);

  __super::Notify(msg);
}

void LogWnd::ShowInit()
{
  UserManage *user_manager = ResShare::GetInstance()->GetUserManager();

}

void LogWnd::OnClick(CControlUI * pSender)
{
  if (pSender->GetName() == _T("close_btn"))
    ::PostMessage(pa_hwnd_, kAM_LogCloseMsg, 0, 0);
  else if (pSender->GetName() == _T("cancel"))
    ::PostMessage(pa_hwnd_, kAM_LogCloseMsg, 0, 0);
  else if (pSender->GetName() == _T("login"))
    OnClickLoginBtn();
}

void LogWnd::OnRadioChange(CControlUI * pSender)
{
  radio_type_ = pSender->GetText();
  int a = 0;
}

void LogWnd::OnClickLoginBtn()
{
  CDuiString user_name = m_PaintManager.FindControl(_T("user_name"))->GetText();
  CDuiString user_id = m_PaintManager.FindControl(_T("ID"))->GetText();
  CDuiString password = m_PaintManager.FindControl(_T("password"))->GetText();

  if (user_name == _T("") || user_id == _T("") || password == _T("")) {
    MessageBox(nullptr, _T("登录信息有误！"), _T("Error"), MB_OK);
  } else {
    ResShare *res_share = ResShare::GetInstance();
    UserManage *user_manager = res_share->GetUserManager();
    CDuiString path = CPaintManagerUI::GetResourcePath();
    path += _T("..\\Data\\");

    if (user_manager->LoadFile(path, _T("user_data_file"))) {
      if (user_manager->IsLoginOk(user_name, user_id, password, radio_type_)) {
        UserInfoStruct log_info;
        log_info.id = user_id;
        log_info.name = user_name;
        log_info.pwd = password;
        log_info.type = radio_type_;
        res_share->SetLogInfo(log_info);    // 记录下登录者的信息

        ::PostMessage(pa_hwnd_, kAM_LoginMsg, 0, 0);
        Close();
      } else {
        MessageBox(nullptr, _T("登录信息有误！"), _T("Error"), MB_OK);
      }

    } else {
      MessageBox(nullptr, _T("文件加载失败！"), _T("Error"), MB_OK);
    }
  }
}

