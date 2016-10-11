#pragma once

#include "stdafx.h"

enum MyMsg
{
	kAM_InitOtherWndMsg = WM_APP + 10,	// 创建初始化其他窗体
	kAM_InitCustomControlMsg,   // 初始化自定义控件
};
