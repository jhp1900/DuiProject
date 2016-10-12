#pragma once

#include "stdafx.h"

enum MyMsg
{
	kAM_InitOtherWndMsg = WM_APP + 10,	// 创建初始化其他窗体
	kAM_InitCustomControlMsg,   // 初始化自定义控件
	kAM_TrayCallbackMsg,		// 托盘图标回掉信息
	kAM_TrayPopHomeMsg,			// 托盘消息 - 显示主页
	kAM_TrayPopExitMsg,			// 托盘消息 - 退出软件
	kAM_TrayPostPopMsg,			// 发送托盘消息给主界面
};
