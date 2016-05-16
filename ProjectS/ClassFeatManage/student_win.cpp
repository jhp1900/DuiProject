#include "student_win.h"
#include "res_share.h"


StudentWin::StudentWin()
{
}


StudentWin::~StudentWin()
{
}

void StudentWin::Init(HWND pa_hwnd)
{
  pa_hwnd_ = pa_hwnd;
  Create(pa_hwnd_, _T(""), UI_WNDSTYLE_CHILD & ~WS_VISIBLE, 0, 0, 0, 0, 0, (HMENU)(0));
  ::SetWindowPos(*this, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);

  list_ = static_cast<PDUI_LIST>(m_PaintManager.FindControl(_T("student_list")));
  ReloadList();
}

void StudentWin::ReloadList()
{
  UserManage *user_manager = ResShare::GetInstance()->GetUserManager();
  vector<UserInfoStruct> user_info_vec = user_manager->GetSomeType(_T("ѧ��"));
  CDialogBuilder builder;
  list_elem_ = static_cast<PDUI_VERTLAYOUT>(builder.Create(_T("studt_GPA_list_elem.xml")));
  CDuiString cls[][6] = { 
    { _T("1"), _T("C����"), _T("88"), _T("78"), _T("83"), _T("80") }
    ,{ _T("2"), _T("�ߵ���ѧ"), _T("88"), _T("78"), _T("83"), _T("80") }
    ,{ _T("3"), _T("����"), _T("88"), _T("78"), _T("83"), _T("80") }
    ,{ _T("4"), _T("��·����"), _T("88"), _T("78"), _T("83"), _T("80") }
    ,{ _T("5"), _T("����"), _T("88"), _T("78"), _T("83"), _T("80") }
    ,{ _T("6"), _T("ģ��"), _T("88"), _T("78"), _T("83"), _T("80") }
    ,{ _T("7"), _T("��Ƶ"), _T("88"), _T("78"), _T("83"), _T("80") }
  };

  for (int i = 0; i < 7; ++i) {
    list_elem_ = static_cast<PDUI_VERTLAYOUT>(builder.Create());
    list_elem_->FindSubControl(_T("elem_id"))->SetText(cls[i][0]);
    list_elem_->FindSubControl(_T("elem_subject"))->SetText(cls[i][1]);
    list_elem_->FindSubControl(_T("elem_GPA_class"))->SetText(cls[i][2]);
    list_elem_->FindSubControl(_T("elem_GPA_job"))->SetText(cls[i][3]);
    list_elem_->FindSubControl(_T("elem_GPA_live"))->SetText(cls[i][4]);
    list_elem_->FindSubControl(_T("elem_GPA_Exams"))->SetText(cls[i][5]);
    list_->Add(list_elem_);
  }
}