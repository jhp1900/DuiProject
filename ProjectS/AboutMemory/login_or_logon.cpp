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
 *  ���ܣ� �����ʼ������������ʾ���壬ʵ��ĳЩ��Ա�����ĸ�ֵ
 */
void LoginOrLogon::Init()
{
  /* ��������ʾ���� */
	Create(hwnd_parent_, _T(""), UI_WNDSTYLE_CHILD & ~WS_VISIBLE, 0, 0, 0, 0, 0, (HMENU)(0));
	::SetWindowPos(*this, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);

  /* �ӽ�����ؿؼ������� */
	login_layout_ = static_cast<CVerticalLayoutUI *>(m_PaintManager.FindControl(_T("login_layout")));
	logon_layout_ = static_cast<CVerticalLayoutUI *>(m_PaintManager.FindControl(_T("logon_layout")));
}

/**
 *  ���ܣ� ֪ͨ����Ϣ �ķ���·�ɹ���
 */
void LoginOrLogon::Notify(TNotifyUI & msg)
{
  if (msg.sType == _T("textchanged"))   // �ı����ı�
    ;
  else if (msg.sType == _T("killfocus"))  // ʧȥ����
    KillFocus(msg.pSender);
  else if (msg.sType == _T("return")) // �ûس�
    EnterKey(msg.pSender);

  WindowImplBase::Notify(msg);
}

/* ����¼���·�� */
void LoginOrLogon::OnUserClick(const TNotifyUI& msg)
{
  if (msg.pSender->GetName() == _T("login_btn"))      // ��¼
    OnLogin();
  else if (msg.pSender->GetName() == _T("reset_btn")) // ����
    ;
	else if (msg.pSender->GetName() == _T("goto_logon_btn"))  // ȥע�����
		OnGotoLogon();
	else if (msg.pSender->GetName() == _T("cancel_btn"))  // ȡ��ע��
		OnCancel();
	else if (msg.pSender->GetName() == _T("logon_btn"))   // ע��
		OnLogon();
  else if (msg.pSender->GetName() == _T("head_btn"))    // ����ͷ��
    SetHead();
  else if (msg.pSender->GetName() == _T("data_path"))   // �������ݱ���·��
    SetDataPath();
}

/**
 *  ���ܣ� ��Ӧ��¼��ť�ĵ����ʵ���û��ĵ�¼
 *  ʵ�֣� �����û��������û��ļ������ж��û��������Ϣ�Ƿ���ȷ������Щ��ͨ��ʱ�������¼�ɹ�
 */
void LoginOrLogon::OnLogin()
{
  PDUI_EDIT user_name = static_cast<PDUI_EDIT>(m_PaintManager.FindControl(_T("user_name_login")));
  PDUI_EDIT pwd = static_cast<PDUI_EDIT>(m_PaintManager.FindControl(_T("password_login")));

  UserDataManage *user_data_manager = UserDataManage::GetInstance();
  if (!user_data_manager->LoadFile(data_path_, user_name->GetText())) {   // ����ļ�����ʧ�ܣ��������ʾ���˳�������
    MessageBox(NULL, _T("�����ļ�����ʧ�ܣ������û����Ƿ���ȷ��"), _T("Error"), MB_OK);
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
    MessageBox(NULL, _T("�Բ���������֤ʧ�ܣ�"), _T("Warning"), MB_OK);
  }
}

/* ȥע����� */
void LoginOrLogon::OnGotoLogon()
{
	login_layout_->SetVisible(false);
	logon_layout_->SetVisible(true);
}

/* ȡ��ע�� */
void LoginOrLogon::OnCancel()
{
	login_layout_->SetVisible(true);
	logon_layout_->SetVisible(false);
}

/**
 *  ���ܣ� ��Ӧע�ᰴť����� ʵ���û�ע��
 *  ʵ�֣� �ӽ����ȡע����Ϣ���ж�ע����Ϣ���������£����û���Ϣд���µ��û��ļ���
 */
void LoginOrLogon::OnLogon()
{
  if (!(pwd_is_ok_ && user_name_is_ok_ && PathIsOk())) {   // ���ע�������������������ʾ��Ϣ���˳�������
    MessageBox(NULL, _T("ע����Ϣ������!�����²���!"), _T("Warning"), MB_OK);
    return;
  }

  /* ע�����ĸ��ؼ� */
  PDUI_EDIT user_name = static_cast<PDUI_EDIT>(m_PaintManager.FindControl(_T("user_name_logon")));
  PDUI_EDIT pwd = static_cast<PDUI_EDIT>(m_PaintManager.FindControl(_T("password_again")));
  PDUI_BUTTON head_btn = static_cast<PDUI_BUTTON>(m_PaintManager.FindControl(_T("head_btn")));

  UserDataManage *user_data_manager = UserDataManage::GetInstance();

  /* ����ע����Ϣ�����ļ���Ϣ */
  DataFileInfo file_info;
  file_info.admin_name = user_name->GetText(); 
  data_path_ += file_info.admin_name + _T("\\");
  file_info.path = data_path_; // ���û��������ļ�Ŀ¼����Ϊ�û�����Ŀ¼
  file_info.password = pwd->GetText();
  file_info.hand_img = head_path_;

  /* ����û��ļ������ɹ�����д�������Ϣ�������ļ���Ȼ�󷵻ص�½������е�½ */
  if (user_data_manager->NewFile(file_info)) { 
    /* �����Ϣ�������ļ� */
    char temp_char[MAX_PATH] = { 0 };
    ConfigFileManage *config_managet = ConfigFileManage::GetInstance();

    NodeStruct logon_info;
    config_managet->WideToMulti(file_info.admin_name, temp_char);
    logon_info.node_name = temp_char;
    config_managet->WideToMulti(file_info.path, temp_char);
    logon_info.attrs.push_back({ "data_path", temp_char });

    xml_node pa_node = config_managet->FindNode("UserData");
    config_managet->InsertSimpleNode(logon_info, pa_node);

    /* ���õ�¼������Ϣ */
    user_name = static_cast<PDUI_EDIT>(m_PaintManager.FindControl(_T("user_name_login")));
    user_name->SetText(file_info.admin_name);
    PDUI_LABEL head_label = static_cast<PDUI_LABEL>(m_PaintManager.FindControl(_T("head_label")));
    head_label->SetBkImage(head_path_);

    /* ���ص�¼���� */
    login_layout_->SetVisible(true);
    logon_layout_->SetVisible(false);
  } else {
    MessageBox(NULL, _T("�û���Ϣ����ʧ�ܣ�"), _T("Error"), MB_OK);
  }
}

/* �û�ͷ������ */
void LoginOrLogon::SetHead()
{
  TCHAR file_name[MAX_PATH] = { 0 };
  TCHAR folder_path[MAX_PATH] = { 0 };

  OPENFILENAME open_file_struct = { 0 };
  open_file_struct.lStructSize = sizeof(OPENFILENAME);
  open_file_struct.nMaxFile = MAX_PATH;	// ����
  open_file_struct.lpstrFilter = _T("*.png*.jpg\0\0");
  open_file_struct.lpstrFile = file_name;
  open_file_struct.nFilterIndex = 1;
  /* ����������ã����� OFN_ALLOWMULTISELECT ��֧�ֶ��ļ���ѡȡ������Ҫ OFN_EXPLORER ����ϣ���ȻĬ��Ϊ�ɵ����ģʽ */
  open_file_struct.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

  if (::GetOpenFileName(&open_file_struct)) {
    head_path_ = open_file_struct.lpstrFile;
    PDUI_BUTTON head_btn = nullptr;
    head_btn = static_cast<PDUI_BUTTON>(m_PaintManager.FindControl(_T("head_btn")));
    head_btn->SetBkImage(head_path_);
  }
}

/* �û�����·������ */
void LoginOrLogon::SetDataPath()
{
  TCHAR folder_path[MAX_PATH] = { 0 };
  BROWSEINFO bi;
  ZeroMemory(&bi, sizeof(BROWSEINFO));
  bi.hwndOwner = NULL;
  bi.pszDisplayName = folder_path;
  bi.lpszTitle = _T("������ѡ�ļ���Ŀ¼:");
  bi.ulFlags = BIF_RETURNONLYFSDIRS;		// BIF_RETURNONLYFSDIRS ֻ����ѡ��Ŀ¼�� BIF_BROWSEINCLUDEFILES ������Ŀ¼���ļ�
  LPITEMIDLIST idl = SHBrowseForFolder(&bi);	// ���ļ��Ի���
  SHGetPathFromIDList(idl, folder_path);	// ��ȡ�ļ��е�·�� �磺D:\Dow\360Yun\video
  data_path_ = folder_path;
  data_path_ += _T("\\");

  PDUI_BUTTON path_btn = nullptr;
  path_btn = static_cast<PDUI_BUTTON>(m_PaintManager.FindControl(_T("data_path")));
  path_btn->SetText(data_path_);
}

/* ����û����Ƿ���� */
void LoginOrLogon::UserNameCheck()
{
  ConfigFileManage *config_manager = ConfigFileManage::GetInstance();
  PDUI_EDIT edit_user_name = static_cast<PDUI_EDIT>(
    m_PaintManager.FindControl(_T("user_name_logon")));

  if (edit_user_name->GetText().IsEmpty()) {
    MessageBox(NULL, _T("�û�������Ϊ�գ�����������!"), _T("Error"), MB_OK);
    user_name_is_ok_ = false;
    return;
  }

  char logon_name[MAX_PATH] = { 0 };
  config_manager->WideToMulti(edit_user_name->GetText(), logon_name);

  if (config_manager->IsHave("UserData", logon_name)) {
    MessageBox(NULL, _T("�Բ��𣬸��û����ѱ�ռ�ã�"), _T("Warning"), MB_OK);
    user_name_is_ok_ = false;
  } else {
    user_name_is_ok_ = true;
  }
}

/* ����¼���������Ƿ���� */
void LoginOrLogon::PasswordCheck(int flag)
{
  PDUI_EDIT pwd_logon = static_cast<PDUI_EDIT>(m_PaintManager.FindControl(_T("password_logon")));
  PDUI_EDIT pwd_again = static_cast<PDUI_EDIT>(m_PaintManager.FindControl(_T("password_again")));

  if (flag == 0 && pwd_again->GetText().IsEmpty())
    return;

  if (pwd_logon->GetText() == pwd_again->GetText()) {
    pwd_is_ok_ = true;
  } else {
    MessageBox(NULL, _T("�����������벻һ�£�����������!"), _T("Warning"), MB_OK);
    pwd_is_ok_ = false;
  }
}

/* ʧȥ�����¼���Ӧ */
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

/* �ûس����¼���Ӧ */
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
 *  ���ܣ� ��¼Ԥ���������û��ļ�·��������û�ͷ��
 */
void LoginOrLogon::LodinPreprocess()
{
  PDUI_EDIT user_name = static_cast<PDUI_EDIT>(m_PaintManager.FindControl(_T("user_name_login")));
  PDUI_LABEL head_label = static_cast<PDUI_LABEL>(m_PaintManager.FindControl(_T("head_label")));
  if (user_name->GetText().IsEmpty()) {   // ��������Ϊ�գ��򷵻�
    head_label->SetBkImage(CPaintManagerUI::GetResourcePath()
      + _T("res\\head.png"));   // ����δ֪ͷ��
    return;
  }

  ConfigFileManage *config_manager = ConfigFileManage::GetInstance();
  char node_name[MAX_PATH] = { 0 };
  config_manager->WideToMulti(user_name->GetText(), node_name);
  data_path_ = config_manager->GetAttr(node_name, "data_path");

  if (data_path_ == _T(""))   // ���û�������ȷ������·��Ϊ��
    head_label->SetBkImage(CPaintManagerUI::GetResourcePath() 
      + _T("res\\head.png"));   // ����δ֪ͷ��
  else
    head_label->SetBkImage(data_path_ + _T("res\\head.png")); // �����û�ͷ��ͼƬ
}

bool LoginOrLogon::PathIsOk()
{
  PDUI_BUTTON path_btn = nullptr;
  path_btn = static_cast<PDUI_BUTTON>(m_PaintManager.FindControl(_T("data_path")));
  
  return path_btn->GetText() != _T("");
}
