#include "add_admin_wnd.h"
#include "res_share.h"
#include "msg_header.h"

AddAdminWnd::AddAdminWnd(HWND pa_hwnd)
  : pa_hwnd_(pa_hwnd)
  , sex_(_T("女"))
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
    MessageBox(NULL, _T("ID号不能为空！"), _T("Error"), MB_OK);
    return;
  } else if (user_manage->IsHaveID(attr_str)) {
    MessageBox(NULL, _T("该ID号已经存在！"), _T("Warning"), MB_OK);
    return;
  }
  memset(attr_char, 0, sizeof(attr_char));    // 每次使用 attr_char 数组时，先把数组置 0
  user_manage->WideToMulti(attr_str, attr_char);
  node_info.attrs.push_back({ "id", attr_char });

  attr_str = m_PaintManager.FindControl(_T("user_name"))->GetText();
  if (attr_str == _T("")) {
    MessageBox(NULL, _T("用户名不能为空！"), _T("Error"), MB_OK);
    return;
  }
  memset(attr_char, 0, sizeof(attr_char));
  user_manage->WideToMulti(attr_str, attr_char);
  node_info.attrs.push_back({ "name", attr_char });

  memset(attr_char, 0, sizeof(attr_char));
  user_manage->WideToMulti(sex_, attr_char);
  node_info.attrs.push_back({ "sex", attr_char });

  node_info.attrs.push_back({ "type", "教师" });
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
    MessageBox(NULL, _T("添加成功"), _T("Message"), MB_OK);
    m_PaintManager.FindControl(_T("user_name"))->SetText(_T(""));
    m_PaintManager.FindControl(_T("ID"))->SetText(_T(""));
    ::PostMessage(pa_hwnd_, kAM_AddTeacherMsg, 0, 0);   // 发送消息到父窗口，以便进行列表的刷新
  } else {
    MessageBox(NULL, _T("添加失败!"), _T("Message"), MB_OK);
  }
}

