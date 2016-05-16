#include <shlobj.h>
#include "login_or_logon.h"
#include "config_file_manage.h"
#include "user_data_manage.h"
#include "manager.h"

LoginOrLogon::LoginOrLogon(HWND hwnd_parent)
  : hwnd_parent_(hwnd_parent)
  , user_name_is_ok_(false)
  , pwd_is_ok_(false)
{
  head_path_ = CPaintManagerUI::GetResourcePath() 
    + _T("res\\head_default.png");
}

/**
 *  功能： 类体初始化，创建并显示窗体，实现某些成员变量的赋值
 */
void LoginOrLogon::Init()
{
  /* 创建并显示窗体 */
	Create(hwnd_parent_, _T(""), UI_WNDSTYLE_CHILD & ~WS_VISIBLE, 0, 0, 0, 0, 0, (HMENU)(0));
	::SetWindowPos(*this, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);

  /* 从界面加载控件到程序 */
	login_layout_ = static_cast<CVerticalLayoutUI *>(m_PaintManager.FindControl(_T("login_layout")));
	logon_layout_ = static_cast<CVerticalLayoutUI *>(m_PaintManager.FindControl(_T("logon_layout")));
}

/**
 *  功能： 通知、消息 的分类路由功能
 */
void LoginOrLogon::Notify(TNotifyUI & msg)
{
  if (msg.sType == _T("textchanged"))   // 文本被改变
    ;
  else if (msg.sType == _T("killfocus"))  // 失去焦点
    KillFocus(msg.pSender);
  else if (msg.sType == _T("return")) // 敲回车
    EnterKey(msg.pSender);

  WindowImplBase::Notify(msg);
}

/* 点击事件的路由 */
void LoginOrLogon::OnUserClick(const TNotifyUI& msg)
{
  if (msg.pSender->GetName() == _T("login_btn"))      // 登录
    OnLogin();
  else if (msg.pSender->GetName() == _T("reset_btn")) // 重置
    ;
	else if (msg.pSender->GetName() == _T("goto_logon_btn"))  // 去注册界面
		OnGotoLogon();
	else if (msg.pSender->GetName() == _T("cancel_btn"))  // 取消注册
		OnCancel();
	else if (msg.pSender->GetName() == _T("logon_btn"))   // 注册
		OnLogon();
  else if (msg.pSender->GetName() == _T("head_btn"))    // 设置头像
    SetHead();
  else if (msg.pSender->GetName() == _T("data_path"))   // 设置数据保存路径
    SetDataPath();
}

/**
 *  功能： 响应登录按钮的点击，实现用户的登录
 *  实现： 更具用户名加载用户文件，再判断用户密码等信息是否正确，当这些都通过时，给予登录成功
 */
void LoginOrLogon::OnLogin()
{
  PDUI_EDIT user_name = static_cast<PDUI_EDIT>(m_PaintManager.FindControl(_T("user_name_login")));
  PDUI_EDIT pwd = static_cast<PDUI_EDIT>(m_PaintManager.FindControl(_T("password_login")));

  UserDataManage *user_data_manager = UserDataManage::GetInstance();
  if (!user_data_manager->LoadFile(data_path_, user_name->GetText())) {   // 如果文件加载失败，则给出提示并退出本函数
    MessageBox(NULL, _T("数据文件加载失败，请检查用户名是否正确！"), _T("Error"), MB_OK);
    return;
  }

  CDuiString xml_pwd = user_data_manager->GetAttr("Password", "value");
  CDuiString edit_pwd = pwd->GetText();

  if (xml_pwd == edit_pwd) {
    ::PostMessage(hwnd_parent_, kAM_LoginMsg, 0, 0);
    DataFileInfo login_user;
    login_user.path = data_path_;
    login_user.admin_name = user_name->GetText();
    login_user.hand_img = data_path_ + _T("res\\head.png");
    login_user.password = edit_pwd;
    Manager *manager = Manager::GetInstance();
    manager->SetUserInfo(login_user);
  } else {
    MessageBox(NULL, _T("对不起！密码验证失败！"), _T("Warning"), MB_OK);
  }
}

/* 去注册界面 */
void LoginOrLogon::OnGotoLogon()
{
	login_layout_->SetVisible(false);
	logon_layout_->SetVisible(true);
}

/* 取消注册 */
void LoginOrLogon::OnCancel()
{
	login_layout_->SetVisible(true);
	logon_layout_->SetVisible(false);
}

/**
 *  功能： 响应注册按钮点击， 实现用户注册
 *  实现： 从界面获取注册信息，判断注册信息合理的情况下，将用户信息写入新的用户文件中
 */
void LoginOrLogon::OnLogon()
{
  if (!(pwd_is_ok_ && user_name_is_ok_ && PathIsOk())) {   // 如果注册条件不合理，则给出提示信息，退出本函数
    MessageBox(NULL, _T("注册信息不合理!请重新操作!"), _T("Warning"), MB_OK);
    return;
  }

  /* 注册界面的各控件 */
  PDUI_EDIT user_name = static_cast<PDUI_EDIT>(m_PaintManager.FindControl(_T("user_name_logon")));
  PDUI_EDIT pwd = static_cast<PDUI_EDIT>(m_PaintManager.FindControl(_T("password_again")));
  PDUI_BUTTON head_btn = static_cast<PDUI_BUTTON>(m_PaintManager.FindControl(_T("head_btn")));

  UserDataManage *user_data_manager = UserDataManage::GetInstance();

  /* 根据注册信息创建文件信息 */
  DataFileInfo file_info;
  file_info.admin_name = user_name->GetText(); 
  data_path_ += file_info.admin_name + _T("\\");
  file_info.path = data_path_; // 用用户名创建文件目录，作为用户数据目录
  file_info.password = pwd->GetText();
  file_info.hand_img = head_path_;

  /* 如果用户文件创建成功，则写入相关信息到配置文件，然后返回登陆界面进行登陆 */
  if (user_data_manager->NewFile(file_info)) { 
    /* 添加信息到配置文件 */
    char temp_char[MAX_PATH] = { 0 };
    ConfigFileManage *config_managet = ConfigFileManage::GetInstance();

    NodeStruct logon_info;
    config_managet->WideToMulti(file_info.admin_name, temp_char);
    logon_info.node_name = temp_char;
    config_managet->WideToMulti(file_info.path, temp_char);
    logon_info.attrs.push_back({ "data_path", temp_char });

    xml_node pa_node = config_managet->FindNode("UserData");
    config_managet->InsertSimpleNode(logon_info, pa_node);

    /* 设置登录界面信息 */
    user_name = static_cast<PDUI_EDIT>(m_PaintManager.FindControl(_T("user_name_login")));
    user_name->SetText(file_info.admin_name);
    PDUI_LABEL head_label = static_cast<PDUI_LABEL>(m_PaintManager.FindControl(_T("head_label")));
    head_label->SetBkImage(head_path_);

    /* 返回登录界面 */
    login_layout_->SetVisible(true);
    logon_layout_->SetVisible(false);
  } else {
    MessageBox(NULL, _T("用户信息创建失败！"), _T("Error"), MB_OK);
  }
}

/* 用户头像设置 */
void LoginOrLogon::SetHead()
{
  TCHAR file_name[MAX_PATH] = { 0 };
  TCHAR folder_path[MAX_PATH] = { 0 };

  OPENFILENAME open_file_struct = { 0 };
  open_file_struct.lStructSize = sizeof(OPENFILENAME);
  open_file_struct.nMaxFile = MAX_PATH;	// 必填
  open_file_struct.lpstrFilter = _T("*.png*.jpg\0\0");
  open_file_struct.lpstrFile = file_name;
  open_file_struct.nFilterIndex = 1;
  /* 下面风格的设置，填了 OFN_ALLOWMULTISELECT 可支持多文件的选取，但需要 OFN_EXPLORER 的配合，不然默认为旧的浏览模式 */
  open_file_struct.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

  if (::GetOpenFileName(&open_file_struct)) {
    head_path_ = open_file_struct.lpstrFile;
    PDUI_BUTTON head_btn = nullptr;
    head_btn = static_cast<PDUI_BUTTON>(m_PaintManager.FindControl(_T("head_btn")));
    head_btn->SetBkImage(head_path_);
  }
}

/* 用户数据路径设置 */
void LoginOrLogon::SetDataPath()
{
  TCHAR folder_path[MAX_PATH] = { 0 };
  BROWSEINFO bi;
  ZeroMemory(&bi, sizeof(BROWSEINFO));
  bi.hwndOwner = NULL;
  bi.pszDisplayName = folder_path;
  bi.lpszTitle = _T("从下面选文件夹目录:");
  bi.ulFlags = BIF_RETURNONLYFSDIRS;		// BIF_RETURNONLYFSDIRS 只用于选中目录， BIF_BROWSEINCLUDEFILES 可用于目录和文件
  LPITEMIDLIST idl = SHBrowseForFolder(&bi);	// 打开文件对话框
  SHGetPathFromIDList(idl, folder_path);	// 获取文件夹的路径 如：D:\Dow\360Yun\video
  data_path_ = folder_path;
  data_path_ += _T("\\");

  PDUI_BUTTON path_btn = nullptr;
  path_btn = static_cast<PDUI_BUTTON>(m_PaintManager.FindControl(_T("data_path")));
  path_btn->SetText(data_path_);
}

/* 检测用户名是否合理 */
void LoginOrLogon::UserNameCheck()
{
  ConfigFileManage *config_manager = ConfigFileManage::GetInstance();
  PDUI_EDIT edit_user_name = static_cast<PDUI_EDIT>(
    m_PaintManager.FindControl(_T("user_name_logon")));

  if (edit_user_name->GetText().IsEmpty()) {
    MessageBox(NULL, _T("用户名不能为空，请重新输入!"), _T("Error"), MB_OK);
    user_name_is_ok_ = false;
    return;
  }

  char logon_name[MAX_PATH] = { 0 };
  config_manager->WideToMulti(edit_user_name->GetText(), logon_name);

  if (config_manager->IsHave("UserData", logon_name)) {
    MessageBox(NULL, _T("对不起，该用户名已被占用！"), _T("Warning"), MB_OK);
    user_name_is_ok_ = false;
  } else {
    user_name_is_ok_ = true;
  }
}

/* 检测登录密码设置是否合理 */
void LoginOrLogon::PasswordCheck(int flag)
{
  PDUI_EDIT pwd_logon = static_cast<PDUI_EDIT>(m_PaintManager.FindControl(_T("password_logon")));
  PDUI_EDIT pwd_again = static_cast<PDUI_EDIT>(m_PaintManager.FindControl(_T("password_again")));

  if (flag == 0 && pwd_again->GetText().IsEmpty())
    return;

  if (pwd_logon->GetText() == pwd_again->GetText()) {
    pwd_is_ok_ = true;
  } else {
    MessageBox(NULL, _T("两次密码输入不一致，请重新输入!"), _T("Warning"), MB_OK);
    pwd_is_ok_ = false;
  }
}

/* 失去焦点事件响应 */
void LoginOrLogon::KillFocus(CControlUI * pSender)
{
  if(pSender->GetName() == _T("user_name_login"))
    LodinPreprocess();
  else if (pSender->GetName() == _T("user_name_logon"))
    UserNameCheck();
  else if (pSender->GetName() == _T("password_logon"))
    PasswordCheck(0);
  else if (pSender->GetName() == _T("password_again"))
    PasswordCheck(1);
}

/* 敲回车键事件响应 */
void LoginOrLogon::EnterKey(CControlUI * pSender)
{
  if (pSender->GetName() == _T("password_login"))
    OnLogin();
  else if (pSender->GetName() == _T("user_name_logon"))
    OnLogon();
  else if (pSender->GetName() == _T("password_logon"))
    OnLogon();
  else if (pSender->GetName() == _T("password_again"))
    OnLogon();
}

/**
 *  功能： 登录预处理，加载用户文件路径，添加用户头像
 */
void LoginOrLogon::LodinPreprocess()
{
  PDUI_EDIT user_name = static_cast<PDUI_EDIT>(m_PaintManager.FindControl(_T("user_name_login")));
  PDUI_LABEL head_label = static_cast<PDUI_LABEL>(m_PaintManager.FindControl(_T("head_label")));
  if (user_name->GetText().IsEmpty()) {   // 如果输入框为空，则返回
    head_label->SetBkImage(CPaintManagerUI::GetResourcePath()
      + _T("res\\head.png"));   // 设置未知头像
    return;
  }

  ConfigFileManage *config_manager = ConfigFileManage::GetInstance();
  char node_name[MAX_PATH] = { 0 };
  config_manager->WideToMulti(user_name->GetText(), node_name);
  data_path_ = config_manager->GetAttr(node_name, "data_path");

  if (data_path_ == _T(""))   // 若用户名不正确，返回路径为空
    head_label->SetBkImage(CPaintManagerUI::GetResourcePath() 
      + _T("res\\head.png"));   // 设置未知头像
  else
    head_label->SetBkImage(data_path_ + _T("res\\head.png")); // 设置用户头像图片
}

bool LoginOrLogon::PathIsOk()
{
  PDUI_BUTTON path_btn = nullptr;
  path_btn = static_cast<PDUI_BUTTON>(m_PaintManager.FindControl(_T("data_path")));
  
  return path_btn->GetText() != _T("");
}
