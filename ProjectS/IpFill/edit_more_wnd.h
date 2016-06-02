#pragma once
#include "window_impl_base.h"
#include "ip_control_ui.h"

class EditMoreWnd : public WindowImplBase
{
public:
  EditMoreWnd(LPCTSTR play_name);
  EditMoreWnd(LPCTSTR play_name, int index);
  ~EditMoreWnd();

  DECLARE_DUIWND_INFO(_T("EditMoreWnd"), CS_DBLCLKS, _T("edit_more_wnd.xml"))

  BEGIN_DUICONTROL_CREATE(EditMoreWnd)
    DUICONTROL_CREATE_FROM_XML(_T("IpControl"), _T("ip_control.xml"))
    DUICONTROL_CREATE(_T("IpControlLayout"), IpControlUI)
    END_DUICONTROL_CREATE()

private:
  virtual void OnUserClick(const TNotifyUI& msg) override;
  virtual LRESULT OnInit() override;

public:
  BOOL DoModal(HWND pa_hwnd);
  BOOL GetFlag() { return flag_; }

private:
  void OnClickOKBtn();

private:
  CDuiString play_name_;    // ��ǰ������
  int index_;
  CDuiString ipaddr_;       // ip��ַ��
  CDuiString netmask_;      // ��������
  BOOL flag_;               // ��ʶλ��������IP���޸���IP��Ϣ��Ϊ TRUE�� ��֮Ϊ FALSE��
  BOOL type_is_add_;        // ���ͱ�ʶ��ΪTRUE��ʾΪ������ͣ� ��֮Ϊ�༭���ͣ�
};
