#pragma once
#include "window_impl_base.h"
#include "ip_control_ui.h"
#include "msg_head.h"
#include "xml_manager.h"

class Manager : public WindowImplBase
{
public:
  Manager();
	~Manager() {}

  BEGIN_DUICONTROL_CREATE(Manager)
    DUICONTROL_CREATE_FROM_XML(_T("IpControl"), _T("ip_control.xml"))
    DUICONTROL_CREATE(_T("IpControlLayout"), IpControlUI)
    END_DUICONTROL_CREATE()

  BEGIN_DUIMSG_MAP(Manager)
    DUIMSG_HANDLER(kAM_InitCustomControlMsg, OnInitCustomControlMsg)
  END_DUIMSG_MAP()

  static Manager* GetInstance();
  static void DestroyInstance();

  virtual CDuiString GetSkinFolder() override { return _T("skin"); }
  virtual CDuiString GetSkinFile() override { return _T("manager.xml"); }
  virtual LPCTSTR GetWindowClassName(void) const override { return _T("Manager"); }
  virtual void Notify(TNotifyUI& msg) override;
  
private:
	virtual LRESULT OnInit() override;
  virtual void OnUserClick(const TNotifyUI& msg) override;

private:
  LRESULT OnInitCustomControlMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
  LRESULT OnLogCloseMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
  void OnClickAddPlayBtn();
  void OnClickAdvanced();
  BOOL GetPlayInfo(NETSTRUCT &node_info);
  void FlushPlayList();
  void OnSelectPlay(TNotifyUI &msg);
  void StartPlay();
  BOOL ExcuteCommand(LPCTSTR command_lien);               // 执行一个命令
  BOOL ExcuteCommand(vector<CDuiString> command_lien_s);     // 执行一组命令集
  void OnClickTestBtn();

public:
  XmlManager* GetXmlManager() { return &xml_manager_; }

private:
  static Manager *instance_;    // 单例模式的实例
  XmlManager xml_manager_;
  vector<IpControlUI *> ip_ui_vector_;
};