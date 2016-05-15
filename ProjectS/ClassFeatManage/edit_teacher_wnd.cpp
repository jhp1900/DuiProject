#include "edit_teacher_wnd.h"
#include "res_share.h"

EditTeacherWnd::EditTeacherWnd(HWND pa_hwnd, ShowType show_type)
  : pa_hwnd_(pa_hwnd)
  , show_type_(show_type)
  , sex_(_T("女"))
{
}

bool EditTeacherWnd::DoModal()
{
  if (!Create(pa_hwnd_, _T(""), WS_POPUP, WS_EX_TOOLWINDOW))
    return false;

  CenterWindow(pa_hwnd_);
  InitShow();

  return ShowModal() != 0;
}

void EditTeacherWnd::Notify(TNotifyUI & msg)
{
  if (msg.sType == _T("itemselect"))
    OnComboSelect(msg.pSender);
  else if (msg.sType == _T("selectchanged"))
    OnRadioChange(msg.pSender);

  __super::Notify(msg);
}

void EditTeacherWnd::OnUserClick(const TNotifyUI & msg)
{
}

void EditTeacherWnd::InitShow()
{
  if (show_type_ == ShowType::self_edit) {
    m_PaintManager.FindControl(_T("pwd_area"))->SetVisible(true);
  }

  UserInfoStruct edit_user = ResShare::GetInstance()->GetEditInfo();

  PDUI_EDIT id_edit = static_cast<PDUI_EDIT>(m_PaintManager.FindControl(_T("ID")));
  id_edit->SetText(edit_user.id);
  id_edit->SetReadOnly(true);
  m_PaintManager.FindControl(_T("user_name"))->SetText(edit_user.name);

  sex_ = edit_user.sex;
  if (sex_ == _T("男")) {
    PDUI_RADIOBOX radio_sex = static_cast<PDUI_RADIOBOX>(m_PaintManager.FindControl(_T("sex_man")));
    radio_sex->Selected(true);
  }

  //PDUI_COMBO combo = static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("college")));
  m_PaintManager.FindControl(_T("college"))->SetText(edit_user.college);
  m_PaintManager.FindControl(_T("subject"))->SetText(edit_user.subject);
}

void EditTeacherWnd::OnComboSelect(const CControlUI * pSender)
{
  if (pSender->GetName() != _T("college"))
    return;

  CDuiString sel_college = pSender->GetText();
  vector<LPCTSTR> sub_vec;
  if (sel_college == _T("通信工程")) {
    sub_vec = { _T("高等数学"),_T("复变"), _T("大学物理"), _T("C语言")
      , _T("电路分析"), _T("数电"), _T("模电"), _T("高频"), _T("计算机网络") };
  } else if (sel_college == _T("电子工程")) {
    sub_vec = { _T("高等数学"),_T("复变"), _T("大学物理"), _T("C语言")
      , _T("电路原理"), _T("电力电子"), _T("自动化原理"), _T("PLC"), _T("机械制图") };
  } else if (sel_college == _T("计算机")) {
    sub_vec = { _T("高等数学"),_T("复变"), _T("大学物理"), _T("C语言")
      , _T("电路原理"), _T("计算机网络"), _T("操作系统及原理"), _T("汇编语言与接口技术") };
  } else if (sel_college == _T("经管")) {
    sub_vec = { _T("财政与税收"),_T("管理学"), _T("社会学"), _T("市政学")
      , _T("统计学"), _T("会计学"), _T("经济法"), _T("国际贸易") };
  } else if (sel_college == _T("人文")) {
    sub_vec = { _T("高等数学"),_T("复变"), _T("大学物理"), _T("C语言")
      , _T("电路分析"), _T("数电"), _T("模电"), _T("高频"), _T("计算机网络") };
  } else if (sel_college == _T("艺术")) {
    sub_vec = { _T("高等数学"),_T("复变"), _T("大学物理"), _T("C语言")
      , _T("电路分析"), _T("数电"), _T("模电"), _T("高频"), _T("计算机网络") };
  } else if (sel_college == _T("舞蹈")) {
    sub_vec = { _T("高等数学"),_T("复变"), _T("大学物理"), _T("C语言")
      , _T("电路分析"), _T("数电"), _T("模电"), _T("高频"), _T("计算机网络") };
  }

  PDUI_COMBO combo_sub = static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("subject")));
  combo_sub->RemoveAll();
  CListLabelElementUI *elem = nullptr;
  for (auto iter = sub_vec.begin(); iter != sub_vec.end(); ++iter) {
    elem = new CListLabelElementUI;
    elem->SetText(*iter);
    combo_sub->Add(elem);
  }
  combo_sub->SelectItem(0);
}

void EditTeacherWnd::OnRadioChange(const CControlUI * pSender)
{
  sex_ = pSender->GetText();
}
