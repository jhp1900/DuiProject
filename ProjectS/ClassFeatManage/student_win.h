#pragma once
#include "window_impl_base.h"

class StudentWin : public WindowImplBase
{
public:
  StudentWin();
  ~StudentWin();

  DECLARE_DUIWND_INFO(_T("StudentWin"), CS_DBLCLKS, _T("student_wnd.xml"))

public:
  void Init(HWND pa_hwnd);

private:
  void ReloadList();    // 刷新列表内容

private:
  HWND pa_hwnd_;
  PDUI_LIST list_;
  PDUI_VERTLAYOUT list_elem_;
};

