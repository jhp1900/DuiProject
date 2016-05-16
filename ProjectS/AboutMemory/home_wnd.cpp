#include <direct.h>
#include "home_wnd.h"
#include "user_data_manage.h"
#include "add_object_wnd.h"
#include "list_menu_wnd.h"
#include "manager.h"

void HomeWnd::Init(HWND pa_hwnd)
{
  pa_hwnd_ = pa_hwnd;
	Create(pa_hwnd_, _T(""), UI_WNDSTYLE_CHILD & ~WS_VISIBLE, 0, 0, 0, 0, 0, (HMENU)(1));
	::SetWindowPos(*this, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
  list_ = static_cast<PDUI_LIST>(m_PaintManager.FindControl(_T("memory_object_list")));
  FullList();   // 填充列表信息
}

void HomeWnd::Notify(TNotifyUI & msg)
{
  if (msg.sType == _T("itemactivate"))    // 左键双击
    ;
  else if (msg.sType == _T("itemclick"))  // 点击列表框选项
    ;

  __super::Notify(msg);
}

void HomeWnd::OnUserClick(const TNotifyUI& msg)
{
  if (msg.pSender->GetName() == _T("btn_add"))
    CallAddObjectWnd();
  else if (msg.pSender->GetName() == _T("btn_edit"))
    ;
  else if (msg.pSender->GetName() == _T("btn_test")) {  // 测试调用
    UserDataManage *user_data_manager = UserDataManage::GetInstance();
    user_data_manager->Test();
  } else if (msg.pSender->GetName() == _T("elem_control"))
    OnClickElemMenu(msg);
}

LRESULT HomeWnd::OnAddObjectMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
  FullList();
  return LRESULT();
}

/**
 *  功能： 填充列表信息，及将所有的 MemoryObject 信息填充到列表框内
 *         需要说明的是，这里只用到了 MemoryObject 节点的 owner 属性值
 */
void HomeWnd::FullList()
{
  UserDataManage *user_data_manager = UserDataManage::GetInstance();
  vector<xml_node> node_structs = user_data_manager->GetAllChild(_T("Memory")); // 获取 Memory 节点下的所有子节点

  CDialogBuilder builder;
  list_elem_ = static_cast<PDUI_VERTLAYOUT>(builder.Create(_T("list_elem.xml")));

  list_->RemoveAll();
  for (auto iter = node_structs.begin()
    ; iter != node_structs.end(); ++iter) {   // 将所有子节点的 owner 属性值添加到列表中

    list_elem_ = static_cast<PDUI_VERTLAYOUT>(builder.Create());
    LPCTSTR attr_value;
    attr_value = user_data_manager->MultiToWide((*iter).attribute("owner").as_string());
    list_elem_->FindSubControl(_T("elem_owner"))->SetText(attr_value);
    attr_value = user_data_manager->MultiToWide((*iter).attribute("time").as_string());
    list_elem_->FindSubControl(_T("elem_time"))->SetText(attr_value);

    list_->Add(list_elem_);
  }
}

/**
 *  功能： 调出 记忆对象添加界面， 
 */
void HomeWnd::CallAddObjectWnd()
{
  AddObjectWnd add_object_wnd;
  add_object_wnd.DoModal(*this);
}

void HomeWnd::OnClickElemMenu(const TNotifyUI & msg)
{
  /* 记录下当前操作的记忆体对象 */
  PDUI_HORILAYOUT list_elem = nullptr;
  list_elem = static_cast<PDUI_HORILAYOUT>(msg.pSender->GetParent()->GetParent());
  Manager *manager = Manager::GetInstance();
  manager->SetOwner(list_elem->FindSubControl(_T("elem_owner"))->GetText());

  /* 显示列表菜单 */
  ListMenuWnd *menu = new ListMenuWnd(*this);
  menu->Init(msg.ptMouse);
  menu->ShowWindow(true);
}
