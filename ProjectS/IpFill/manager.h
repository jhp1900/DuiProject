#pragma once
#include "window_impl_base.h"
#include "ip_control_ui.h"
#include "msg_head.h"
#include "xml_manager.h"
#include "tray_pop_wnd.h"

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
		DUIMSG_HANDLER(kAM_TrayCallbackMsg, OnTray)
	END_DUIMSG_MAP()

	virtual CDuiString GetSkinFolder() override { return _T("skin"); }
	virtual CDuiString GetSkinFile() override { return _T("manager.xml"); }
	virtual LPCTSTR GetWindowClassName(void) const override { return _T("Manager"); }
	virtual void Notify(TNotifyUI& msg) override;

private:
	virtual LRESULT OnInit() override;
	virtual void OnUserClick(const TNotifyUI& msg) override;

private:
	LRESULT OnTray(UINT uMsg, WPARAM wparam, LPARAM lparam, BOOL& bHandled);

private:
	void OnClickAddPlayBtn();
	void OnClickAdvanced();
	void OnClickDelBtn();
	void OnClickEditBtn();
	void OnClickUpdateBtn();
	BOOL GetPlayInfo(NETSTRUCT &node_info);
	void FlushPlayList();
	void EnumNetName();
	void SetNetName(LPCTSTR net_name);
	void OnSelectPlay(TNotifyUI &msg);
	void StartPlay();
	BOOL ExcuteCommand(LPCTSTR command_lien);               // 执行一个命令
	BOOL ExcuteCommand(vector<CDuiString> command_lien_s);     // 执行一组命令集
	void SetControlEnabled(BOOL enable);
	CDuiString MakeComLine(vector<CDuiString> prarm);
	LPCTSTR GetNetName();
	void OnClickTestBtn();
	void AddTray();

private:
	XmlManager *xml_manager_;
	vector<IpControlUI *> ip_ui_vector_;
	TrayPopWnd tray_pop_wnd_;
};