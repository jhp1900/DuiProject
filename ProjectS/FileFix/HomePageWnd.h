#pragma once
#include "XMLBaseWnd.h"

class Manager : public XMLBaseWnd
{
public:
	Manager(LPCTSTR xml_path);
	~Manager();

private:
	virtual void InitWindow();							// 初始化窗体
	virtual BOOL OnClickUser(const TNotifyUI& msg);		// 用户控件消息响应
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);		// 消息控制

private:
	void OnBtnClickAddFile();		// 响应添加文件按钮单击事件
	void OnBtnClickAddFolder();		// 响应添加文件夹按钮单击事件
	void OnBtnClickClear();			// 响应清除列表按钮点击事件
	void OnBtnClickFix();			// 响应修复按钮单击事件
	LRESULT OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam);	// 处理文件拖放事件
	BOOL AddFileToList(LPCTSTR file_path_name);	// 添加一个带路径的文件名到文件列表中
	DWORD Exec(const TCHAR* name, const TCHAR* param, const TCHAR* dir, const TCHAR* relative_tool_dir);

private:
	CListUI *file_path_list_;
	CListTextElementUI *list_element_point_;
	vector<CDuiString> fail_names_;
};

