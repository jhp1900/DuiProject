#pragma once
#include "window_impl_base.h"
#include "native_control_ui.h"
#include "user_data_manage.h"

class ResDropOCX;

class AddCardWnd : public WindowImplBase
{
public:
  AddCardWnd() 
    : pa_hwnd_(nullptr)
    , res_layout_(nullptr)
    , res_elem_(nullptr)
    , bkimage_(_T(""))
  {}
    ~AddCardWnd();

  DECLARE_DUIWND_INFO(_T("AddCardWnd"), CS_DBLCLKS, _T("add_memory_card_wnd.xml"))

  BEGIN_DUICONTROL_CREATE(AddCardWnd)
    DUICONTROL_CREATE(_T("Panel"), NativeControlUI)
    END_DUICONTROL_CREATE()

  BEGIN_DUIMSG_MAP(AddCardWnd)
    DUIMSG_HANDLER(WM_DROPFILES, OnDropFiles)
  END_DUIMSG_MAP()

public:
  bool DoModal(HWND pa_hwnd);

private:
  virtual void Notify(TNotifyUI& msg) override;		// DuiLib 控件消息路由
  virtual void OnUserClick(const TNotifyUI& msg) override;
  LRESULT OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);	// 处理文件拖放事件

private:
  void OnClickAddBtn();
  void OnComboSelect(const TNotifyUI & msg);     // 响应 Combobox 的选择改变
  bool FileTypeIsOk(TCHAR file_name[MAX_PATH], TCHAR ex_name[20], CDuiString res_type);
  void SetResElem(LPCTSTR file_name);
  void OnClickDelBtn(CControlUI* pSender);
  LPCTSTR GetSysTime();
  void NodeStructInit(NodeStruct &node);

private:
  HWND pa_hwnd_;
  PDUI_TILELAYOUT res_layout_;
  PDUI_CONTAINER res_elem_;
  CDuiString bkimage_;
};

