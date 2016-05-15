#include "XMLBaseWnd.h"

/**
���ܣ����캯������ʼ�������ļ����������ļ�·�������ƹ�����
������LPCTSTR xml_name �����ļ���
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
���ܣ���д����� ��Ϣ���ƺ�����Ϊ�˽��ܵ� WM_DROPFILES���ļ��Ϸţ���Ϣ.
*/
LRESULT XMLBaseWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg)
	{
	case WM_CREATE:			lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:		lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;	// ��������� WM_NCACTIVATE��WM_NCCALCSIZE��WM_NCPAINT ������Ϣ
	case WM_NCCALCSIZE:		lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;	// ����ʵ�ִ�����������Ի�
	case WM_NCPAINT:		lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:		lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;		// WM_NCHITTEST ��Ϣ, ����ȷ������¼������ڴ������һ����
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
���ܣ��������壬���ô����񣬵��ô����ʼ��
*/
LRESULT XMLBaseWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandle)
{
	/* ���ô��ڷ�� */
	LONG style_value = ::GetWindowLong(*this, GWL_STYLE);
	style_value &= ~WS_CAPTION;		// ȥ�� WS_CAPTION ���ԣ���ȥ��������
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

/* �������� WM_NCACTIVATE ��Ϣ����ʵ���Ի������ */
LRESULT XMLBaseWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	if (::IsIconic(*this))
		bHandled = FALSE;

	return (wParam == 0) ? TRUE : FALSE;
}
/* �������� WM_NCCALCSIZE ��Ϣ����ʵ���Ի������ */
LRESULT XMLBaseWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	return 0;
}
/* �������� WM_NCPAINT ��Ϣ����ʵ���Ի������ */
LRESULT XMLBaseWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	return 0;
}

/**
���ܼ�ʵ�֣�ʹ�ܹ�ͨ���϶��ı䴰���λ�ã��ܹ������߿����ı䴰��Ĵ�С
������ͨ���жϲ�����������ڵ��ֵ��ʵ�ֵġ�
������	UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled
����ֵ����������¼������ڴ����λ��
����ֵ���壬�ο�ӡ��ʼ� ��OnNcHitTest ��������ֵ��
*/
LRESULT XMLBaseWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	/* ��ȡ����¼���λ�ã������õ�ת��Ϊ�ͻ�������ϵ�ϵĵ� */
	POINT pt;
	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT client_rect;
	::GetClientRect(*this, &client_rect);
	/* ʵ�ִ��ڵ��ϴ���С */
	if (!::IsZoomed(*this))		// IsZoomed ������ �жϴ����Ƿ���󻯣��ǣ�����TRUE�����ǣ�����FALSE��
	{
		RECT size_box = paint_manager_->GetSizeBox();	// GetSizeBox ,��ȡ XML �ļ���window��ǩ�� sizebox ����ֵ
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

	/* ʵ�ִ����϶� */
	RECT caption_rect = paint_manager_->GetCaptionRect();	// GetCaptionRect ��������ȡ XML �ļ���window��ǩ�� caption ����ֵ���������϶�����
	if (pt.x >= client_rect.left + caption_rect.left && pt.x < client_rect.right - caption_rect.right
		&& pt.y >= caption_rect.top && pt.y < caption_rect.bottom)
	{
		CControlUI *control = static_cast<CControlUI *>(paint_manager_->FindControl(pt));	// ��ȡ pt �����ڵĿռ�ָ��
		if (control && _tcscmp(control->GetClass(), _T("ButtonUI")) != 0	// _tcscmp ���ڱȽ������ַ����ģ������Ϊ0�� > ��Ϊ���� < ��Ϊ��
			&& _tcscmp(control->GetClass(), _T("OptionUI")) != 0
			&& _tcscmp(control->GetClass(), _T("TextUI")) != 0)
			return HTCAPTION;
	}

	return HTCLIENT;
}

/* �������� WM_CLOSET ��Ϣ */
LRESULT XMLBaseWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	bHandled = FALSE;
	return 0;
}

/**
���ܼ�ʵ�֣���Ӧ WM_DESTROY ��Ϣ��ͨ�� PostQuitMessage ���������˳���Ϣ��ʵ������
������UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled
����ֵ��LRESULT��Ĭ�Ϸ��� 0;
*/
LRESULT XMLBaseWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	::PostQuitMessage(0L);
	bHandled = FALSE;
	return 0;
}

/**
���ܼ�ʵ�֣���Ӧ����Ĺر���Ϣ����󻯡���ԭ��Ϣ��
ͨ���жϴ��嵱ǰ�Ƿ������״̬��ȷ������ʾʲô��ť
������ UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled
����ֵ������ǹرմ��壬�򷵻� 0�����򣬷��� HandleMessage �ķ���ֵ��
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
	if (::IsZoomed(*this) != is_zoomed)		// ������״̬�����仯�� ָ  ���->�����  ��  �����->��� ��
	{
		if (!is_zoomed)		// ���� !is_zoomed ��״̬���Ǳ仯ǰ��״̬���磬!is_zoomed Ϊ�棬��˵���Ǵ��� �����->��� ת��ʱ��״̬
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

/* �����ʼ�������� �ṩ�������ิд */
void XMLBaseWnd::InitWindow()
{
}

/**
���ܣ��ؼ���Ϣ��·�ɣ����������Ϣ����·���˵���¼������˫���¼����Ҽ��˵��¼��Լ�DuiLibĬ���¼���
������TNotifyUI& msg �¼���Ϣ�ṹ��
*/
void XMLBaseWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == TEXT("click"))		// ��������ؼ�
	{
		if (OnClickSys(msg))
			return;
		else if (OnClickUser(msg))
			return;
	}

	//__super::Notify(msg);
}

/* ���¼��������ṩ�������ิд�� */
BOOL XMLBaseWnd::OnClickSys(const TNotifyUI& msg)	// ��Ӧϵͳ�ؼ��ĵ��
{
	CDuiString btn_name = msg.pSender->GetName();

	if (btn_name == TEXT("minbtn"))
	{
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);		// ������С����Ϣ
		return TRUE;
	}
	else if (btn_name == TEXT("maxbtn"))
	{
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);		// ���������Ϣ
		return TRUE;
	}
	else if (btn_name == TEXT("restorebtn"))
	{
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);		// ���ʹ��ڻ�ԭ��Ϣ
		return TRUE;
	}
	else if (btn_name == TEXT("closebtn"))
	{
		PostQuitMessage(0L);		// �����˳���Ϣ
		return TRUE;
	}

	return FALSE;
}

BOOL XMLBaseWnd::OnClickUser(const TNotifyUI& msg)	// ��Ӧ�û��ؼ��ĵ��
{
	return FALSE;
}
