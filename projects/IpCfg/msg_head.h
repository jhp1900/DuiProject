#pragma once

#include "stdafx.h"

enum MyMsg
{
	kAM_InitOtherWndMsg = WM_APP + 10,	// ������ʼ����������
	kAM_InitCustomControlMsg,   // ��ʼ���Զ���ؼ�
	kAM_TrayCallbackMsg,		// ����ͼ��ص���Ϣ
	kAM_TrayPopMsg,				// ����������Ϣ��������
};
