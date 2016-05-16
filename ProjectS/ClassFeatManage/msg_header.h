#pragma once

#include "stdafx.h"

enum MyMsg
{
	kAM_InitOtherWndMsg = WM_APP + 10,	// 创建初始化其他窗体
	kAM_LoginMsg,	// 登录消息 
	kAM_LogCloseMsg,	// 退出消息
  kAM_AddTeacherMsg,    // 添加教师 消息
};