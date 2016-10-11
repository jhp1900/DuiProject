#include "manager.h"
#include "add_new_play_wnd.h"
#include "advanced_wnd.h"
#include <shellapi.h>
#include "res_singleton.h"
#include <NetCon.h>
#include "resource.h"

Manager::Manager()
{
	ResSingleton* res = ResSingleton::GetInstance();
	xml_manager_ = res->GetXmlManager();
	xml_manager_->LoadFile(CPaintManagerUI::GetInstancePath(), _T("config.xml"));
}

void Manager::Notify(TNotifyUI & msg)
{
	if (msg.sType == _T("itemselect"))
		if (msg.pSender->GetName() == _T("play_list"))
			OnSelectPlay(msg);

	__super::Notify(msg);
}

LRESULT Manager::OnInit()
{
	::SendMessage(*this, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon((HINSTANCE)GetWindowLongPtr(m_hWnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDI_ICON1)));
	AddTray();

	FlushPlayList();    // 读取所有的方案名，并填入方案名下来列表中

	EnumNetName();      // 枚举所有网卡的网络名称

	/* 初始化收录所有 ip地址 控件 */
	vector<LPCTSTR> all_ip_ui_str = {
	  _T("ip_address"),
	  _T("netmask"),
	  _T("gateway"),
	  _T("firstDNS"),
	  _T("secondDNS")
	};
	IpControlUI * ip_ui_temp = nullptr;
	for (auto iter : all_ip_ui_str) {
		ip_ui_temp = static_cast<IpControlUI *>(m_PaintManager.FindControl(iter));
		ip_ui_vector_.push_back(ip_ui_temp);
	}

	return 0;
}

void Manager::OnUserClick(const TNotifyUI& msg)
{
	if (msg.pSender->GetName() == _T("start_play_btn")) {       // 启动方案
		StartPlay();
	} else if (msg.pSender->GetName() == _T("advanced_btn")) {
		OnClickAdvanced();
	} else if (msg.pSender->GetName() == _T("edit_play_btn")) {
		OnClickEditBtn();
	} else if (msg.pSender->GetName() == _T("update_play_btn")) {
		OnClickUpdateBtn();
	} else if (msg.pSender->GetName() == _T("del_play_btn")) {
		OnClickDelBtn();
	} else if (msg.pSender->GetName() == _T("add_play_btn")) {
		OnClickAddPlayBtn();
	} else if (msg.pSender->GetName() == _T("test_btn")) {      // 测试内容
		OnClickTestBtn();
	}
}

LRESULT Manager::OnTray(UINT uMsg, WPARAM wparam, LPARAM lparam, BOOL & bHandled)
{
	if (wparam != 1)
		return 0;
	switch (lparam)
	{
		case WM_LBUTTONUP:
			break;
		case WM_RBUTTONUP:
			LPPOINT lpoint = new tagPOINT;
			::GetCursorPos(lpoint);
			tray_pop_wnd_.PopupWindow(lpoint);
			break;
	}

	return LRESULT();
}

void Manager::OnClickAddPlayBtn()
{
	NETSTRUCT net_info;
	if (!GetPlayInfo(net_info)) {
		MessageBox(nullptr, _T("方案信息不合理哦，亲！"), _T("Message"), MB_OK);
		return;
	}

	net_info.net_name = m_PaintManager.FindControl(_T("net_name_list"))->GetText();

	AddNewPlayWnd new_play;
	new_play.DoModal(*this);
	CDuiString play_name = new_play.GetNewName();
	if (play_name == _T(""))
		return;

	net_info.play_name = play_name;
	xml_manager_->InsertNode(net_info);

	FlushPlayList();
	SetControlEnabled(false);
}

void Manager::OnClickAdvanced()
{
	PDUI_COMBO play_list = static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("play_list")));
	AdvancedWnd adv_wnd(play_list->GetText());
	adv_wnd.DoModal(*this);
}

void Manager::OnClickDelBtn()
{
	PDUI_COMBO play_list = static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("play_list")));
	LPCTSTR play_name = play_list->GetText();
	if (0 == lstrcmp(play_name, _T("Auto"))) {
		MessageBox(nullptr, _T("【Auto】方案不能删除哦，亲！"), _T("Error"), MB_OK);
		return;
	}
	if (xml_manager_->RemoveNode(xml_manager_->GetNode(play_name))) {
		MessageBox(nullptr, _T("方案删除成功!"), _T("Message"), MB_OK);
		FlushPlayList();
	} else {
		MessageBox(nullptr, _T("方案删除失败!"), _T("Message"), MB_OK);
	}
}

void Manager::OnClickEditBtn()
{
	SetControlEnabled(true);
}

void Manager::OnClickUpdateBtn()
{
	NETSTRUCT net_info;
	net_info.play_name = m_PaintManager.FindControl(_T("play_list"))->GetText();
	if (net_info.play_name == _T("Auto")) {
		MessageBox(nullptr, _T("【Auto】方案不能保存，请选择【存为新方案】"), _T("ERROR"), MB_OK);
		return;
	}
	net_info.net_name = m_PaintManager.FindControl(_T("net_name_list"))->GetText();
	if (!GetPlayInfo(net_info)) {
		MessageBox(nullptr, _T("方案信息不合理哦，亲！"), _T("Message"), MB_OK);
		return;
	}

	if (xml_manager_->UpdateNode(net_info)) {
		MessageBox(nullptr, _T("方案修改成功！"), _T("Message"), MB_OK);
	} else {
		MessageBox(nullptr, _T("方案修改失败了哦，亲！"), _T("ERROR"), MB_OK);
		return;
	}
	SetControlEnabled(false);
}

BOOL Manager::GetPlayInfo(NETSTRUCT & net_info)
{
	for (auto iter : ip_ui_vector_) {
		if (!iter->IsReasonable())
			return FALSE;
	}

	for (int i = 0; i != 5; ++i) {
		net_info.SetVar(xml_manager_->net_attrs_[i], ip_ui_vector_[i]->GetText());
	}

	return TRUE;
}

void Manager::FlushPlayList()
{
	PDUI_COMBO play_list = static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("play_list")));
	for (int i = 1; i < play_list->GetCount(); )
		play_list->RemoveAt(1);

	PDUI_LISTLABELELEM list_elem;
	for (auto play_name : xml_manager_->GetAllNodeName()) {
		list_elem = new CListLabelElementUI;
		list_elem->SetText(play_name);
		play_list->Add(list_elem);
	}
}

void Manager::EnumNetName()
{
	INetConnectionManager *net_manager = nullptr;
	INetConnection *net_conn = nullptr;
	IEnumNetConnection *net_enum = nullptr;
	ULONG celt_fetched;

	setlocale(LC_CTYPE, "");
	CoInitialize(NULL);
	CoCreateInstance(CLSID_ConnectionManager, NULL, CLSCTX_SERVER, IID_INetConnectionManager, (void**)&net_manager);
	if (net_manager == nullptr) {
		MessageBox(nullptr, _T("网络检测失败！"), _T("ERROR"), MB_OK);
		return;
	}

	net_manager->EnumConnections(NCME_DEFAULT, &net_enum);
	NETCON_PROPERTIES *net_proper;
	PDUI_COMBO net_name_list = static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("net_name_list")));
	PDUI_LISTLABELELEM list_elem;
	while (net_enum->Next(1, &net_conn, &celt_fetched) == S_OK)
	{
		net_conn->GetProperties(&net_proper);
		list_elem = new CListLabelElementUI;
		list_elem->SetText(net_proper->pszwName);
		net_name_list->Add(list_elem);
	}
	net_name_list->SelectItem(0);
}

void Manager::SetNetName(LPCTSTR net_name)
{
	PDUI_COMBO net_name_list = static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("net_name_list")));
	int count = net_name_list->GetCount();
	for (int i = 0; i < count; ++i) {
		if (net_name_list->GetItemAt(i)->GetText() == net_name) {
			net_name_list->SelectItem(i);
			return;
		}
	}

	//  MessageBox(nullptr, _T(""), _T(""), MB_OK);
}

void Manager::OnSelectPlay(TNotifyUI & msg)
{
	SetControlEnabled(false);
	CDuiString play_name = msg.pSender->GetText();
	if (play_name == _T("Auto")) {
		for (auto iter : ip_ui_vector_) {
			iter->SetText(_T("..."));
		}
		m_PaintManager.FindControl(_T("net_name_list"))->SetEnabled(true);  // Auto 状态下，网络名称列表可用
		return;
	}

	NETSTRUCT net_info = xml_manager_->GetNodeInfo(play_name);
	for (int i = 0; i != 5; ++i) {
		ip_ui_vector_[i]->SetText(net_info.GetVar(xml_manager_->net_attrs_[i]));
	}
	SetNetName(net_info.net_name);
}

void Manager::StartPlay()
{
	vector<CDuiString> command_lines;
	PDUI_COMBO play_list = static_cast<PDUI_COMBO>(m_PaintManager.FindControl(_T("play_list")));
	vector<CDuiString> prarm;

	if (play_list->GetText() == _T("Auto")) {
		/* 自动获取 IP 地址 */
		prarm.clear();
		prarm.push_back(_T("set address "));  // 设置类型
		prarm.push_back(GetNetName());
		prarm.push_back(_T("dhcp"));
		command_lines.push_back(MakeComLine(prarm));

		/* 自动获取 DNS */
		prarm.clear();
		prarm.push_back(_T("set dnsservers "));  // 设置类型
		prarm.push_back(GetNetName());
		prarm.push_back(_T("dhcp"));
		command_lines.push_back(MakeComLine(prarm));
	} else {
		NETSTRUCT net_info = xml_manager_->GetNodeInfo(play_list->GetText());

		/* 设置 IP 信息 */
		prarm.clear();
		prarm.push_back(_T("set address "));
		prarm.push_back(GetNetName());
		prarm.push_back(_T("static "));
		prarm.push_back(net_info.ip_address + _T(" "));
		prarm.push_back(net_info.netmask + _T(" "));
		prarm.push_back(net_info.gateway);
		command_lines.push_back(MakeComLine(prarm));

		/* 设置 DNS 信息 */
		prarm.clear();
		prarm.push_back(_T("set dnsservers "));
		prarm.push_back(GetNetName());
		prarm.push_back(_T("static "));
		prarm.push_back(net_info.firstDNS);
		command_lines.push_back(MakeComLine(prarm));

		/* 拥有多个 IP 的情况 */
		for (auto iter : net_info.more_ip_mask) {
			prarm.clear();
			prarm.push_back(_T("add address "));
			prarm.push_back(GetNetName());
			prarm.push_back(iter.first + _T(" "));
			prarm.push_back(iter.second);
			command_lines.push_back(MakeComLine(prarm));
		}

		/* 拥有 备用DNS 的情况 */
		if (net_info.secondDNS != _T("")) {
			prarm.clear();
			prarm.push_back(_T("add dnsservers "));
			prarm.push_back(GetNetName());
			prarm.push_back(net_info.secondDNS);
			command_lines.push_back(MakeComLine(prarm));
		}
	}

	if (ExcuteCommand(command_lines))
		MessageBox(nullptr, _T("方案启用成功!"), _T("Message"), MB_OK);
	else
		MessageBox(nullptr, _T("方案启用失败！请查证！"), _T("Message"), MB_OK);
}

BOOL Manager::ExcuteCommand(LPCTSTR command_lien)
{
	SHELLEXECUTEINFO shell_info = { 0 };
	shell_info.cbSize = sizeof(SHELLEXECUTEINFO);
	shell_info.fMask = SEE_MASK_NOCLOSEPROCESS;
	shell_info.lpVerb = _T("runas");      // runas 可以实现以管理员权限运行程序
	shell_info.hwnd = nullptr;
	shell_info.lpFile = _T("cmd.exe");
	shell_info.lpDirectory = nullptr;
	shell_info.nShow = SW_HIDE;
	shell_info.lpParameters = command_lien;
	shell_info.hInstApp = nullptr;

	DWORD exitcode;
	ShellExecuteEx(&shell_info);
	return GetExitCodeProcess(shell_info.hProcess, &exitcode);
}

BOOL Manager::ExcuteCommand(vector<CDuiString> command_lien_s)
{
	for (auto iter : command_lien_s)
		if (!ExcuteCommand(iter))
			return FALSE;

	return TRUE;
}

void Manager::SetControlEnabled(BOOL enable)
{
	for (auto iter : ip_ui_vector_) {
		iter->SetStateEdit(enable);
	}
	m_PaintManager.FindControl(_T("net_name_list"))->SetEnabled(enable);
	m_PaintManager.FindControl(_T("update_play_btn"))->SetVisible(enable);  // 可编辑状态时，编辑按钮隐藏
	m_PaintManager.FindControl(_T("edit_play_btn"))->SetVisible(!enable);
}

CDuiString Manager::MakeComLine(vector<CDuiString> prarm)
{
	CDuiString ret = _T("/c netsh interface ip ");
	for (auto iter : prarm) {
		ret += iter;
	}
	return ret;
}

LPCTSTR Manager::GetNetName()
{
	CDuiString ret = _T("\"");
	PDUI_CONTROL play_list = m_PaintManager.FindControl(_T("play_list"));
	if (play_list->GetText() == _T("Auto")) {
		ret += m_PaintManager.FindControl(_T("net_name_list"))->GetText() + _T("\" ");
	} else {
		ret += xml_manager_->GetNodeInfo(play_list->GetText()).net_name + _T("\" ");
	}
	return ret;
}

void Manager::OnClickTestBtn()
{
}

void Manager::AddTray()
{
	tray_pop_wnd_.CreateWithDefaltStyle(m_hWnd);

	NOTIFYICONDATA tray_data;

	tray_data.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	tray_data.hWnd = this->m_hWnd;
	tray_data.uID = 1;
	tray_data.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	tray_data.uCallbackMessage = kAM_TrayCallbackMsg;
	tray_data.hIcon = LoadIcon((HINSTANCE)GetWindowLongPtr(m_hWnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDI_ICON1));

	wcscpy_s(tray_data.szTip, L"成都天狐威视IVGA");
	Shell_NotifyIcon(NIM_ADD, &tray_data);
}
