#include "teacher_win.h"
#include "res_share.h"

TeacherWin::TeacherWin()
{
}

TeacherWin::~TeacherWin()
{
}

void TeacherWin::Init(HWND pa_hwnd)
{
  pa_hwnd_ = pa_hwnd;
  Create(pa_hwnd_, _T(""), UI_WNDSTYLE_CHILD & ~WS_VISIBLE, 0, 0, 0, 0, 0, (HMENU)(0));
  ::SetWindowPos(*this, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);

  list_ = static_cast<PDUI_LIST>(m_PaintManager.FindControl(_T("student_list")));
  ReloadList();
}

void TeacherWin::ReloadList()
{
  CDialogBuilder builder;
  list_elem_ = static_cast<PDUI_VERTLAYOUT>(builder.Create(_T("tcher_GPA_list_elem.xml")));
  CDuiString cls[][6] = {
    { _T("2012101205"), _T("赵强"), _T("88"), _T("78"), _T("83"), _T("80") }
    ,{ _T("2012101206"), _T("周建"), _T("88"), _T("78"), _T("83"), _T("80") }
    ,{ _T("2012101208"), _T("陈永林"), _T("88"), _T("78"), _T("83"), _T("80") }
    ,{ _T("2012101220"), _T("吴晓霞"), _T("88"), _T("78"), _T("83"), _T("80") }
    ,{ _T("2012101232"), _T("彭明亮"), _T("88"), _T("78"), _T("83"), _T("80") }
  };

  for (int i = 0; i < 5; ++i) {
    list_elem_ = static_cast<PDUI_VERTLAYOUT>(builder.Create());
    list_elem_->FindSubControl(_T("elem_id"))->SetText(cls[i][0]);
    list_elem_->FindSubControl(_T("elem_name"))->SetText(cls[i][1]);
    list_elem_->FindSubControl(_T("elem_GPA_class"))->SetText(cls[i][2]);
    list_elem_->FindSubControl(_T("elem_GPA_job"))->SetText(cls[i][3]);
    list_elem_->FindSubControl(_T("elem_GPA_live"))->SetText(cls[i][4]);
    list_elem_->FindSubControl(_T("elem_GPA_Exams"))->SetText(cls[i][5]);
    list_->Add(list_elem_);
  }
}