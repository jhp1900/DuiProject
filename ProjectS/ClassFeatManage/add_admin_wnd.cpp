#include "add_admin_wnd.h"
#include "res_share.h"
#include "msg_header.h"

AddAdminWnd::AddAdminWnd(HWND pa_hwnd)
  : pa_hwnd_(pa_hwnd)
  , sex_(_T("Ů"))
{
}

bool AddAdminWnd::DoModal()
{
  if (!Create(pa_hwnd_, _T(""), WS_POPUP, WS_EX_TOOLWINDOW))
    return false;

  CenterWindow(pa_hwnd_);
  return ShowModal() != 0;
}

void AddAdminWnd::Notify(TNotifyUI & msg)
{
  if (msg.sType == _T("itemselect"))
    OnComboSelect(msg.pSender);
  else if (msg.sType == _T("selectchanged"))
    OnRadioChange(msg.pSender);

  __super::Notify(msg);
}

void AddAdminWnd::OnUserClick(const TNotifyUI & msg)
{
  if (msg.pSender->GetName() == _T("okbtn"))
    OnClickAddBtn(msg.pSender);
}

void AddAdminWnd::OnComboSelect(const CControlUI * pSender)
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

void AddAdminWnd::OnRadioChange(const CControlUI * pSender)
{
  sex_ = pSender->GetText();
}

void AddAdminWnd::OnClickAddBtn(const CControlUI * pSender)
{
  ResShare *share = ResShare::GetInstance();
  UserManage *user_manage = share->GetUserManager();

  NodeStruct node_info;
  node_info.node_name = "UserData";
  char attr_char[30] = { 0 };
  CDuiString attr_str = m_PaintManager.FindControl(_T("ID"))->GetText();
  if (attr_str == _T("")) {
    MessageBox(NULL, _T("ID�Ų���Ϊ�գ�"), _T("Error"), MB_OK);
    return;
  } else if (user_manage->IsHaveID(attr_str)) {
    MessageBox(NULL, _T("��ID���Ѿ����ڣ�"), _T("Warning"), MB_OK);
    return;
  }
  memset(attr_char, 0, sizeof(attr_char));    // ÿ��ʹ�� attr_char ����ʱ���Ȱ������� 0
  user_manage->WideToMulti(attr_str, attr_char);
  node_info.attrs.push_back({ "id", attr_char });

  attr_str = m_PaintManager.FindControl(_T("user_name"))->GetText();
  if (attr_str == _T("")) {
    MessageBox(NULL, _T("�û�������Ϊ�գ�"), _T("Error"), MB_OK);
    return;
  }
  memset(attr_char, 0, sizeof(attr_char));
  user_manage->WideToMulti(attr_str, attr_char);
  node_info.attrs.push_back({ "name", attr_char });

  memset(attr_char, 0, sizeof(attr_char));
  user_manage->WideToMulti(sex_, attr_char);
  node_info.attrs.push_back({ "sex", attr_char });

  node_info.attrs.push_back({ "type", "��ʦ" });
  node_info.attrs.push_back({ "pwd", "123456" });

  attr_str = m_PaintManager.FindControl(_T("college"))->GetText();
  memset(attr_char, 0, sizeof(attr_char));
  user_manage->WideToMulti(attr_str, attr_char);
  node_info.attrs.push_back({ "college", attr_char });

  attr_str = m_PaintManager.FindControl(_T("subject"))->GetText();
  memset(attr_char, 0, sizeof(attr_char));
  user_manage->WideToMulti(attr_str, attr_char);
  node_info.attrs.push_back({ "subject", attr_char });

  if (user_manage->InsertNode(node_info)) {
    MessageBox(NULL, _T("��ӳɹ�"), _T("Message"), MB_OK);
    m_PaintManager.FindControl(_T("user_name"))->SetText(_T(""));
    m_PaintManager.FindControl(_T("ID"))->SetText(_T(""));
    ::PostMessage(pa_hwnd_, kAM_AddTeacherMsg, 0, 0);   // ������Ϣ�������ڣ��Ա�����б��ˢ��
  } else {
    MessageBox(NULL, _T("���ʧ��!"), _T("Message"), MB_OK);
  }
}

