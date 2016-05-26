#include "advanced_wnd.h"
#include "manager.h"

AdvancedWnd::AdvancedWnd()
  : play_name_(_T(""))
{
}

AdvancedWnd::AdvancedWnd(LPCTSTR play_name)
{
  AdvancedWnd();
  play_name_ = play_name;
  Manager *manager = Manager::GetInstance();
  XmlManager *xml_manager = manager->GetXmlManager();
}

AdvancedWnd::~AdvancedWnd()
{
}

LRESULT AdvancedWnd::OnInit()
{
  PDUI_LIST ip_list = static_cast<PDUI_LIST>(m_PaintManager.FindControl(_T("ip_list")));
  PDUI_LISTTEXTELEM elem = new CListTextElementUI;
  elem->SetText(0, _T("10.18.3.63"));
  ip_list->Add(elem);

  return 0;
}

CDuiString AdvancedWnd::GetItemText(CControlUI * pList, int iItem, int iSubItem)
{
  return CDuiString();
}

BOOL AdvancedWnd::DoModal(HWND pa_hwnd)
{
  if (!Create(pa_hwnd, _T(""), WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_ACCEPTFILES))
    return false;
  CenterWindow(pa_hwnd);

  return ShowModal() != 0;
}