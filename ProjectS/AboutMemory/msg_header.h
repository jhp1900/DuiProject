#pragma once

#include "stdafx.h"

enum MyMsg
{
	kAM_InitOtherWndMsg = WM_APP + 10,	// 创建初始化其他窗体
	kAM_LoginMsg,	// 登录消息 
	kAM_LogonMsg,	// 注册消息
  kAM_AddObjectMsg,   // 添加记忆对象消息 
  kAM_AddCardMsg,     // 添加记忆卡片消息
};