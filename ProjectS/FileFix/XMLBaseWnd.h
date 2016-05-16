#pragma once
#include "stdafx.h"

class XMLBaseWnd : public CWindowWnd, public INotifyUI
{
public:
	XMLBaseWnd(LPCTSTR xml_name);
	~XMLBaseWnd();

public:
	virtual LPCTSTR GetWindowClassName() const;		// 复写纯虚函数， 获取窗体类名
	virtual CDuiString GetSkinFile();				// 复写纯虚函数， 获取界面（.xml）文件名
	virtual CDuiString GetSkinFolder();				// 复写纯虚函数， 获取界面文件路径

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);		// 消息控制
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandle);		// 创建窗体函数
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);	// 一下三个函数是为了实现窗口标题栏的自绘;
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);		// 响应关闭消息
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);		// 响应销毁消息
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);	// 窗口的最大化、最小化、还原

	virtual void InitWindow();				// 初始化窗体
	virtual void Notify(TNotifyUI& msg);	// 路由控件消息的响应
	virtual BOOL OnClickSys(const TNotifyUI& msg);		// 系统控件消息响应 
	virtual BOOL OnClickUser(const TNotifyUI& msg);		// 用户控件消息响应

public:
	CPaintManagerUI *paint_manager_;	// 绘制管理者

private:
	CDuiString xml_name_;				// 存储 XML 文件名
};

