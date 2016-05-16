#pragma once
#include "window_impl_base.h"

class TeacherWin : public WindowImplBase
{
public:
  TeacherWin();
  ~TeacherWin();

  DECLARE_DUIWND_INFO(_T("TeacherWin"), CS_DBLCLKS, _T("teacher_wnd.xml"))

public:
  void Init(HWND pa_hwnd);

private:
  void ReloadList();    // 刷新列表内容

private:
  HWND pa_hwnd_;
  PDUI_LIST list_;
  PDUI_VERTLAYOUT list_elem_;
};

