#pragma once

#include "stdafx.h"

enum MyMsg
{
	kAM_InitOtherWndMsg = WM_APP + 10,	// 创建初始化其他窗体
	kAM_InitCustomControlMsg,   // 初始化自定义控件
	kAM_TrayCallbackMsg,		// 托盘图标回掉信息
	kAM_TrayPopMsg,				// 发送托盘消息给主界面
};
