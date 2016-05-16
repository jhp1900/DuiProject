#include "edit_teacher_wnd.h"
#include "res_share.h"

EditTeacherWnd::EditTeacherWnd(HWND pa_hwnd, ShowType show_type)
  : pa_hwnd_(pa_hwnd)
  , show_type_(show_type)
  , sex_(_T("Ů"))
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
  if (sex_ == _T("��")) {
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
  if (sel_college == _T("ͨ�Ź���")) {
    sub_vec = { _T("�ߵ���ѧ"),_T("����"), _T("��ѧ����"), _T("C����")
      , _T("��·����"), _T("����"), _T("ģ��"), _T("��Ƶ"), _T("���������") };
  } else if (sel_college == _T("���ӹ���")) {
    sub_vec = { _T("�ߵ���ѧ"),_T("����"), _T("��ѧ����"), _T("C����")
      , _T("��·ԭ��"), _T("��������"), _T("�Զ���ԭ��"), _T("PLC"), _T("��е��ͼ") };
  } else if (sel_college == _T("�����")) {
    sub_vec = { _T("�ߵ���ѧ"),_T("����"), _T("��ѧ����"), _T("C����")
      , _T("��·ԭ��"), _T("���������"), _T("����ϵͳ��ԭ��"), _T("���������ӿڼ���") };
  } else if (sel_college == _T("����")) {
    sub_vec = { _T("������˰��"),_T("����ѧ"), _T("���ѧ"), _T("����ѧ")
      , _T("ͳ��ѧ"), _T("���ѧ"), _T("���÷�"), _T("����ó��") };
  } else if (sel_college == _T("����")) {
    sub_vec = { _T("�ߵ���ѧ"),_T("����"), _T("��ѧ����"), _T("C����")
      , _T("��·����"), _T("����"), _T("ģ��"), _T("��Ƶ"), _T("���������") };
  } else if (sel_college == _T("����")) {
    sub_vec = { _T("�ߵ���ѧ"),_T("����"), _T("��ѧ����"), _T("C����")
      , _T("��·����"), _T("����"), _T("ģ��"), _T("��Ƶ"), _T("���������") };
  } else if (sel_college == _T("�赸")) {
    sub_vec = { _T("�ߵ���ѧ"),_T("����"), _T("��ѧ����"), _T("C����")
      , _T("��·����"), _T("����"), _T("ģ��"), _T("��Ƶ"), _T("���������") };
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
