#include "XMLBaseWnd.h"

/**
功能：构造函数，初始化界面文件名、界面文件路径、绘制管理者
参数：LPCTSTR xml_name 界面文件名
*/
XMLBaseWnd::XMLBaseWnd(LPCTSTR xml_name) : xml_name_(xml_name)
{
	paint_manager_ = new CPaintManagerUI;
}

XMLBaseWnd::~XMLBaseWnd()
{
}

LPCTSTR XMLBaseWnd::GetWindowClassName() const
{
	return _T("XMLWnd");
}

CDuiString XMLBaseWnd::GetSkinFile()
{
	return xml_name_;
}

CDuiString XMLBaseWnd::GetSkinFolder()
{
	return _T("skin");
}

/**
功能：复写基类的 消息控制函数，为了接受到 WM_DROPFILES（文件拖放）消息.
*/
LRESULT XMLBaseWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg)
	{
	case WM_CREATE:			lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:		lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;	// 在这里，屏蔽 WM_NCACTIVATE、WM_NCCALCSIZE、WM_NCPAINT 三个消息
	case WM_NCCALCSIZE:		lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;	// 可以实现窗体标题栏的自绘
	case WM_NCPAINT:		lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:		lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;		// WM_NCHITTEST 消息, 用于确定鼠标事件发生在窗体的哪一部分
	case WM_CLOSE:			lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_SYSCOMMAND:		lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	default:			bHandled = FALSE; break;
	}
	if (bHandled)
		return lRes;

	if (bHandled)
		return lRes;

	if (paint_manager_->MessageHandler(uMsg, wParam, lParam, lRes))
		return lRes;

	return __super::HandleMessage(uMsg, wParam, lParam);
}

/**
功能：创建窗体，设置窗体风格，调用窗体初始化
*/
LRESULT XMLBaseWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandle)
{
	/* 设置窗口风格 */
	LONG style_value = ::GetWindowLong(*this, GWL_STYLE);
	style_value &= ~WS_CAPTION;		// 去掉 WS_CAPTION 属性，即去掉标题栏
	::SetWindowLong(*this, GWL_STYLE, style_value | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_THICKFRAME | WS_BORDER);

	paint_manager_->Init(m_hWnd);
	CDialogBuilder builder;
	CControlUI *pRoot = builder.Create(xml_name_.GetData(), (UINT)0, NULL, paint_manager_);
	ASSERT(pRoot && "Failed to parse XML");
	paint_manager_->AttachDialog(pRoot);
	paint_manager_->AddNotifier(this);

	InitWindow();

	return 0;
}

/* 用于屏蔽 WM_NCACTIVATE 消息，以实现自绘标题栏 */
LRESULT XMLBaseWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	if (::IsIconic(*this))
		bHandled = FALSE;

	return (wParam == 0) ? TRUE : FALSE;
}
/* 用于屏蔽 WM_NCCALCSIZE 消息，以实现自绘标题栏 */
LRESULT XMLBaseWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	return 0;
}
/* 用于屏蔽 WM_NCPAINT 消息，以实现自绘标题栏 */
LRESULT XMLBaseWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	return 0;
}

/**
功能及实现：使能够通过拖动改变窗体的位置，能够拉动边框来改变窗体的大小
这里是通过判断并返回鼠标所在点的值来实现的。
参数：	UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled
返回值：返回鼠标事件发生在窗体的位置
各种值含义，参考印象笔记 “OnNcHitTest 函数返回值”
*/
LRESULT XMLBaseWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	/* 获取鼠标事件的位置，并将该点转换为客户区坐标系上的点 */
	POINT pt;
	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT client_rect;
	::GetClientRect(*this, &client_rect);
	/* 实现窗口的拖大拖小 */
	if (!::IsZoomed(*this))		// IsZoomed 函数， 判断窗口是否最大化，是，返回TRUE；不是，返回FALSE。
	{
		RECT size_box = paint_manager_->GetSizeBox();	// GetSizeBox ,获取 XML 文件中window标签的 sizebox 属性值
		if (pt.y < client_rect.top + size_box.top)
		{
			if (pt.x < client_rect.left + size_box.left)
				return HTTOPLEFT;
			if (pt.x > client_rect.right - size_box.right)
				return HTTOPRIGHT;
			return HTTOP;
		}
		else if (pt.y > client_rect.bottom - size_box.bottom)
		{
			if (pt.x < client_rect.left + size_box.left)
				return HTBOTTOMLEFT;
			if (pt.x > client_rect.right - size_box.right)
				return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}
		if (pt.x < client_rect.left + size_box.left)
			return HTLEFT;
		if (pt.x > client_rect.right - size_box.right)
			return HTRIGHT;
	}

	/* 实现窗口拖动 */
	RECT caption_rect = paint_manager_->GetCaptionRect();	// GetCaptionRect 函数，获取 XML 文件中window标签的 caption 属性值，即，可拖动区域
	if (pt.x >= client_rect.left + caption_rect.left && pt.x < client_rect.right - caption_rect.right
		&& pt.y >= caption_rect.top && pt.y < caption_rect.bottom)
	{
		CControlUI *control = static_cast<CControlUI *>(paint_manager_->FindControl(pt));	// 获取 pt 点所在的空间指针
		if (control && _tcscmp(control->GetClass(), _T("ButtonUI")) != 0	// _tcscmp 用于比较两个字符串的，相对则为0； > 则为正； < 则为负
			&& _tcscmp(control->GetClass(), _T("OptionUI")) != 0
			&& _tcscmp(control->GetClass(), _T("TextUI")) != 0)
			return HTCAPTION;
	}

	return HTCLIENT;
}

/* 用于屏蔽 WM_CLOSET 消息 */
LRESULT XMLBaseWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	bHandled = FALSE;
	return 0;
}

/**
功能及实现：响应 WM_DESTROY 消息，通过 PostQuitMessage 函数发送退出消息来实现销毁
参数：UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled
返回值：LRESULT，默认返回 0;
*/
LRESULT XMLBaseWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	::PostQuitMessage(0L);
	bHandled = FALSE;
	return 0;
}

/**
功能及实现：响应窗体的关闭消息、最大化、还原消息；
通过判断窗体当前是否处于最大化状态来确定该显示什么按钮
参数： UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled
返回值：如果是关闭窗体，则返回 0；否则，返回 HandleMessage 的返回值。
*/
LRESULT XMLBaseWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	if (wParam == SC_CLOSE) {
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}

	BOOL is_zoomed = ::IsZoomed(*this);
	LRESULT res = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(*this) != is_zoomed)		// 当窗体状态发生变化（ 指  最大化->非最大化  或  非最大化->最大化 ）
	{
		if (!is_zoomed)		// 这里 !is_zoomed 的状态，是变化前的状态；如，!is_zoomed 为真，则说明是处于 非最大化->最大化 转变时的状态
		{
			CControlUI *control = static_cast<CControlUI *>(paint_manager_->FindControl(_T("maxBtn")));
			if (control)
				control->SetVisible(false);
			control = static_cast<CControlUI *>(paint_manager_->FindControl(_T("restoreBtn")));
			if (control)
				control->SetVisible(true);
		}
		else
		{
			CControlUI *control = static_cast<CControlUI *>(paint_manager_->FindControl(_T("maxBtn")));
			if (control)
				control->SetVisible(true);
			control = static_cast<CControlUI *>(paint_manager_->FindControl(_T("restoreBtn")));
			if (control)
				control->SetVisible(false);
		}
	}

	return res;
}

/* 窗体初始化函数， 提供给派生类复写 */
void XMLBaseWnd::InitWindow()
{
}

/**
功能：控件消息的路由，这里更具消息类型路由了点击事件、左键双击事件、右键菜单事件以及DuiLib默认事件。
参数：TNotifyUI& msg 事件消息结构体
*/
void XMLBaseWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == TEXT("click"))		// 左键单击控件
	{
		if (OnClickSys(msg))
			return;
		else if (OnClickUser(msg))
			return;
	}

	//__super::Notify(msg);
}

/* 以下几个函数提供给派生类复写。 */
BOOL XMLBaseWnd::OnClickSys(const TNotifyUI& msg)	// 响应系统控件的点击
{
	CDuiString btn_name = msg.pSender->GetName();

	if (btn_name == TEXT("minbtn"))
	{
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);		// 发送最小化消息
		return TRUE;
	}
	else if (btn_name == TEXT("maxbtn"))
	{
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);		// 发送最大化消息
		return TRUE;
	}
	else if (btn_name == TEXT("restorebtn"))
	{
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);		// 发送窗口还原消息
		return TRUE;
	}
	else if (btn_name == TEXT("closebtn"))
	{
		PostQuitMessage(0L);		// 发送退出消息
		return TRUE;
	}

	return FALSE;
}

BOOL XMLBaseWnd::OnClickUser(const TNotifyUI& msg)	// 响应用户控件的点击
{
	return FALSE;
}
