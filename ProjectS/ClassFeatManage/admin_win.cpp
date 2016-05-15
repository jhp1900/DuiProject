#include "admin_win.h"
#include "res_share.h"
#include "edit_teacher_wnd.h"

AdminWin::AdminWin()
  : admin_menu_(nullptr)
  , teacher_list_(nullptr)
  , student_list_(nullptr)
  , college_list_(nullptr)
  , classes_list_(nullptr)
  , subject_list_(nullptr)
  , list_elem_(nullptr)
{
}

void AdminWin::Init(HWND pa_hwnd)
{
  pa_hwnd_ = pa_hwnd;
  Create(pa_hwnd_, _T(""), UI_WNDSTYLE_CHILD & ~WS_VISIBLE, 0, 0, 0, 0, 0, (HMENU)(0));
  ::SetWindowPos(*this, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
  teacher_list_ = static_cast<PDUI_LIST>(m_PaintManager.FindControl(_T("teacher_list")));
  student_list_ = static_cast<PDUI_LIST>(m_PaintManager.FindControl(_T("student_list")));
  college_list_ = static_cast<PDUI_LIST>(m_PaintManager.FindControl(_T("college_list")));
  classes_list_ = static_cast<PDUI_LIST>(m_PaintManager.FindControl(_T("classes_list")));
  subject_list_ = static_cast<PDUI_LIST>(m_PaintManager.FindControl(_T("subject_list")));
  ReloadTeacherList();
  ReloadStudentList();
  ReloadSubjectList();
}

void AdminWin::Notify(TNotifyUI & msg)
{
  if (msg.sType == TEXT("selectchanged"))			// �ı�ѡ��ؼ���״̬
    OnSelChanged(msg.pSender);
  else if (msg.sType == _T("headerclick"))    // �����ͷ
    OnHeaderItem(msg.pSender);

  __super::Notify(msg);
}

void AdminWin::OnUserClick(const TNotifyUI & msg)
{
  if (msg.pSender->GetName() == _T("edit_user"))
    OnEditBtn(msg.pSender);
  else if (msg.pSender->GetName() == _T("del_user"))
    OnDelBtn(msg.pSender);
}

LRESULT AdminWin::OnAddTeacherMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
  ReloadTeacherList();
  return LRESULT();
}

void AdminWin::OnSelChanged(const CControlUI * pSender)
{
  PDUI_TABLAYOUT tab = static_cast<PDUI_TABLAYOUT>(m_PaintManager.FindControl(_T("tab_layout")));
  CDuiString sender = pSender->GetName();
  if (sender == _T("opt_teacher"))
    tab->SelectItem(0);
  else if (sender == _T("opt_student"))
    tab->SelectItem(1);
  else if (sender == _T("opt_subject"))
    tab->SelectItem(2);
}

void AdminWin::OnHeaderItem(const CControlUI * pSender)
{
  if (pSender->GetName() == _T("control")) {
    RECT rec = pSender->GetPos();
    if (!admin_menu_) {
      admin_menu_ = new ListMenuWnd(*this, _T("admin_menu.xml"));
      admin_menu_->Init({ rec.left, rec.bottom });    // �Ա�ͷ�����½���Ϊ�˵������Ͻ�
    } else {
      admin_menu_->Reload({ rec.left, rec.bottom });
    }
  }
}

void AdminWin::ReloadTeacherList()
{
  UserManage *user_manager = ResShare::GetInstance()->GetUserManager();
  vector<UserInfoStruct> user_info_vec = user_manager->GetSomeType(_T("��ʦ"));
  CDialogBuilder builder;
  list_elem_ = static_cast<PDUI_VERTLAYOUT>(builder.Create(_T("teacher_list_elem.xml")));

  teacher_list_->RemoveAll();
  for (auto iter = user_info_vec.begin(); iter != user_info_vec.end(); ++iter) {
    list_elem_ = static_cast<PDUI_VERTLAYOUT>(builder.Create());
    list_elem_->FindSubControl(_T("elem_id"))->SetText(iter->id);
    list_elem_->FindSubControl(_T("elem_name"))->SetText(iter->name);
    list_elem_->FindSubControl(_T("elem_college"))->SetText(iter->college);
    list_elem_->FindSubControl(_T("elem_subject"))->SetText(iter->subject);
    teacher_list_->Add(list_elem_);
  }
}

void AdminWin::ReloadStudentList()
{
  UserManage *user_manager = ResShare::GetInstance()->GetUserManager();
  vector<UserInfoStruct> user_info_vec = user_manager->GetSomeType(_T("ѧ��"));
  CDialogBuilder builder;
  list_elem_ = static_cast<PDUI_VERTLAYOUT>(builder.Create(_T("student_list_elem.xml")));

  student_list_->RemoveAll();
  for (auto iter = user_info_vec.begin(); iter != user_info_vec.end(); ++iter) {
    list_elem_ = static_cast<PDUI_VERTLAYOUT>(builder.Create());
    list_elem_->FindSubControl(_T("elem_id"))->SetText(iter->id);
    list_elem_->FindSubControl(_T("elem_name"))->SetText(iter->name);
    list_elem_->FindSubControl(_T("elem_sex"))->SetText(iter->sex);
    list_elem_->FindSubControl(_T("elem_classes"))->SetText(iter->classes);
    list_elem_->FindSubControl(_T("elem_room"))->SetText(iter->room);
    list_elem_->FindSubControl(_T("elem_tel"))->SetText(iter->tel);
    student_list_->Add(list_elem_);
  }
}

void AdminWin::ReloadSubjectList()
{
  UserManage *user_manager = ResShare::GetInstance()->GetUserManager();
  vector<UserInfoStruct> user_info_vec = user_manager->GetSomeType(_T("ѧ��"));
  CDialogBuilder builder;
  list_elem_ = static_cast<PDUI_VERTLAYOUT>(builder.Create(_T("classes_list_elem.xml")));
  LPCTSTR cls[] = {_T("2012-�ƶ�-1"), _T("2012-�ƶ�-2"), _T("2012-���-1"), _T("2012-���-2"), _T("2012-���"), _T("2012-���") };
  for (int i = 0; i < 6; ++i) {
    list_elem_ = static_cast<PDUI_VERTLAYOUT>(builder.Create());
    list_elem_->FindSubControl(_T("elem_cls_name"))->SetText(cls[i]);
    classes_list_->Add(list_elem_);
  }

  LPCTSTR sub[] = { _T("�ߵ���ѧ"), _T("����"), _T("��ѧ����"), _T("C����"), _T("��·����"), _T("����"), _T("����"), _T("ģ��"), _T("��Ƶ"), _T("���������") };
  for (int i = 0; i < 10; ++i) {
    list_elem_ = static_cast<PDUI_VERTLAYOUT>(builder.Create());
    list_elem_->FindSubControl(_T("elem_cls_name"))->SetText(sub[i]);
    subject_list_->Add(list_elem_);
  }
}

void AdminWin::OnEditBtn(const CControlUI * pSender)
{
  PDUI_CONTAINER elem = static_cast<PDUI_CONTAINER>(pSender->GetParent()->GetParent()->GetParent());
  LPCTSTR id = elem->FindSubControl(_T("elem_id"))->GetText();
  UserManage *user_manager = ResShare::GetInstance()->GetUserManager();
  ResShare::GetInstance()->SetEditInfo(user_manager->GetOneNode(id));

  EditTeacherWnd edit_tcher_(*this, ShowType::admin_edit);
  edit_tcher_.DoModal();
}

void AdminWin::OnDelBtn(const CControlUI * pSender)
{
  int ret = MessageBox(NULL, _T("ȷ��ɾ��������Ϣ"), _T("Message"), MB_YESNO);
  if (ret == IDNO)
    return;
  PDUI_CONTAINER elem = static_cast<PDUI_CONTAINER>(pSender->GetParent()->GetParent()->GetParent());
  LPCTSTR id = elem->FindSubControl(_T("elem_id"))->GetText();
  UserManage *user_manager = ResShare::GetInstance()->GetUserManager();
  user_manager->DelNode(id);
  ReloadTeacherList();
}

