#pragma once
#include "window_impl_base.h"

class AddObjectWnd : public WindowImplBase
{
public:
  AddObjectWnd() : pa_hwnd_(nullptr) {}
  ~AddObjectWnd() {}

  DECLARE_DUIWND_INFO(_T("AddObjectWnd"), CS_DBLCLKS, _T("add_memory_object_wnd.xml"))

public:
  bool DoModal(HWND pa_hwnd);

private:
//  virtual void Notify(TNotifyUI& msg) override;
  virtual void OnUserClick(const TNotifyUI& msg) override;

private:
  bool CheckOwner();      // �����������˹���Ϣ�Ƿ����
  void OnAddBtnClick();   // ��Ӧ��Ӱ�ť�ĵ��

private:
  HWND pa_hwnd_;
};

