#include "add_object_wnd.h"
#include "user_data_manage.h"
#include "msg_header.h"

bool AddObjectWnd::DoModal(HWND pa_hwnd)
{
  pa_hwnd_ = pa_hwnd;
  if (!Create(pa_hwnd_, _T(""), WS_POPUP, WS_EX_TOOLWINDOW))
    return false;

  CenterWindow(pa_hwnd_);
  return ShowModal() != 0;
}

//void AddObjectWnd::Notify(TNotifyUI & msg)
//{
//  if(msg.sType == _T("killfocus"))
//
//  __super::Notify(msg);
//}

void AddObjectWnd::OnUserClick(const TNotifyUI & msg)
{
  if (msg.pSender->GetName() == _T("add_btn"))
    OnAddBtnClick();
  else if (msg.pSender->GetName() == _T("cancel_btn"))
    Close();
}

/**
 *  功能： 检测添加的主人公是否合理（及是否已经有相同的主人公？）
 *  返回值：  true 表示主人公信息合理； 反之不合理;
 */
bool AddObjectWnd::CheckOwner()
{
  PDUI_EDIT owner_edit = static_cast<PDUI_EDIT>(m_PaintManager.FindControl(_T("owner")));

  if (owner_edit->GetText() == _T("")) {
    MessageBox(NULL, _T("主人公不能为空!"), _T("Error"), MB_OK);
    return false;
  }
  
  UserDataManage *user_manager = UserDataManage::GetInstance();
  if (user_manager->HaveNodeForAttr(
    _T("Memory"),
    _T("MemoryObject"), 
    _T("owner"), 
    owner_edit->GetText())) {

    MessageBox(NULL, _T("改主人公已经存在，请重新输入!"), _T("Warrning"), MB_OK);
    return false;
  }

  return true;
}

void AddObjectWnd::OnAddBtnClick()
{
  if (!CheckOwner())
    return;

  PDUI_EDIT owner_edit = static_cast<PDUI_EDIT>(m_PaintManager.FindControl(_T("owner")));
  PDUI_COMBO combo_type = static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("type")));
  PDUI_DATETIME time_data = static_cast<PDUI_DATETIME>(m_PaintManager.FindControl(_T("time")));
  UserDataManage *user_manager = UserDataManage::GetInstance();

  CDuiString owner_str = owner_edit->GetText();

  char owner[MAX_PATH] = { 0 };
  char type[MAX_PATH] = { 0 };
  char time[MAX_PATH] = { 0 };
  user_manager->WideToMulti(owner_str, owner);
  user_manager->WideToMulti(combo_type->GetText(), type);
  user_manager->WideToMulti(time_data->GetText(), time);

  NodeStruct node_struct;
  node_struct.node_name = "MemoryObject";
  node_struct.attrs.push_back({ "owner", owner });
  node_struct.attrs.push_back({ "type", type });
  node_struct.attrs.push_back({ "time", time });
  
  xml_node pa_node = user_manager->FindNode("Memory");
  user_manager->InsertSimpleNode(node_struct, pa_node);

  ::PostMessage(pa_hwnd_, kAM_AddObjectMsg, 0, 0);
  Close();
}
