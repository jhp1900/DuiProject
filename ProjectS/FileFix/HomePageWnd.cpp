#include "HomePageWnd.h"
#include <commdlg.h>
#include <shlobj.h>
#include <shlwapi.h>
#include "TraversingResource.h"
#include <shellapi.h>
#include "file_system.h"
#include <atlstr.h>
#include <atlwin.h>

Manager::Manager(LPCTSTR xml_path) : XMLBaseWnd(xml_path)
{
}

Manager::~Manager()
{
}

/**
���ܣ���ʼ����Ա������ͨ�� DragAcceptFiles() �����ô�������ļ����Ϸ�
*/
void Manager::InitWindow()
{
	DragAcceptFiles(m_hWnd, TRUE);
	file_path_list_ = static_cast<CListUI *>(paint_manager_->FindControl(_T("files_list")));
}

/**
���ܣ��û���Ϣ·�ɡ�������Ϣ��Դ�Ŀؼ�����·����Ϣ��Ӧ
������const TNotifyUI& msg ��Ϣ�ṹ��
*/
BOOL Manager::OnClickUser(const TNotifyUI& msg)
{
	if (msg.pSender->GetName() == TEXT("add_file"))				//	����ļ�·��
	{
		OnBtnClickAddFile();
		return TRUE;
	}
	else if (msg.pSender->GetName() == TEXT("add_folder"))		//	����ļ���·��
	{
		OnBtnClickAddFolder();
		return TRUE;
	}
	else if (msg.pSender->GetName() == TEXT("clear"))			//	����б�
	{
		OnBtnClickClear();
		return TRUE;
	}
	else if (msg.pSender->GetName() == TEXT("fix"))				//	�޸��ļ�
	{
		OnBtnClickFix();
		return TRUE;
	}

	return FALSE;
}

/**
���ܣ���д����� ��Ϣ���ƺ�����Ϊ�˽��ܵ� WM_DROPFILES���ļ��Ϸţ���Ϣ.
*/
LRESULT Manager::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg)
	{
	case WM_DROPFILES:	lRes = OnDropFiles(uMsg, wParam, lParam); break;
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
  ���ܣ���ѡ���ļ��Ի��򣬽�ѡ���ļ���·����Ϣ��ӵ��б����, ֧�ֶ���ļ���ѡ�С�
  ʵ�֣�ͨ������ GetOpenFileName() ���������洢�������Ϣ�� OPENFILENAME �ṹ����Ϊ�������룬ͬʱ���ļ���ϢҲ�洢�ڸýṹ�崫����
        �ļ�·�������Ƶ����Դ洢�ڽṹ��� lpstrFile ��Ա�У����ַ��ĸ�ʽ�ֱ�Ϊ��
          Explorer���D:\Dow\360Yun\videoNULL111.aviNULL222.mp4NULLNULL (�����ʹ�� NULL ��Ϊ�ָ���� ĩβ�����и� NULL)
          �ɷ���Կո���Ϊ�ָ��ʹ�ö��ļ���
 */
void Manager::OnBtnClickAddFile()
{
	TCHAR open_files_name[80 * MAX_PATH] = { 0 };	// �洢������ѡ�ļ���·��
	TCHAR files_name[MAX_PATH] = { 0 };		// ��ȡ�����ĵ����ļ���·���ļ���
	TCHAR folder_path[MAX_PATH] = { 0 };	// �ļ���·��
	OPENFILENAME open_file_struct = { 0 };
	open_file_struct.lStructSize = sizeof(OPENFILENAME);
	open_file_struct.nMaxFile = MAX_PATH;	// ����
	open_file_struct.lpstrFilter = _T("MP4�ļ�(*.mp4)\0*.mp4\0\0");
	open_file_struct.lpstrFile = open_files_name;
	open_file_struct.nFilterIndex = 1;
	/* ����������ã����� OFN_ALLOWMULTISELECT ��֧�ֶ��ļ���ѡȡ������Ҫ OFN_EXPLORER ����ϣ���ȻĬ��Ϊ�ɵ����ģʽ */
	open_file_struct.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

	if (::GetOpenFileName(&open_file_struct))
	{
		lstrcpyn(folder_path, open_files_name, open_file_struct.nFileOffset);	// ��ȡ·��
		folder_path[open_file_struct.nFileOffset] = '\0';	// ��ӽ�����־
		int name_len = lstrlen(folder_path);
		if (folder_path[name_len - 1] != '\\')
			lstrcat(folder_path, _T("\\"));
		TCHAR *p = open_files_name + open_file_struct.nFileOffset;

		while (*p)
		{
			ZeroMemory(files_name, sizeof(files_name));
			lstrcat(files_name, folder_path);
			lstrcat(files_name, p);
			p += lstrlen(p) + 1;	// ʹָ����һ���ļ�

			AddFileToList(files_name);
		}
	}

	CProgressUI *progress = static_cast<CProgressUI *>(paint_manager_->FindControl(_T("progress")));
	progress->SetValue(0);
}

/**
  ���ܣ����һ���ļ����µ�ƥ���ļ����б��С�
  ʵ�֣�ʹ�� BROWSEINFO ��������������������Ϣ�� ͨ�� SHBrowseForFolder() ������ SHGetPathFromIDList() ������ȡ�ļ�������·����
        ��ͨ�� TraversingResource ��ʵ���ļ��ı�����������ͬʱɸѡ����Ҫ���ļ����ͣ���ӵ��б���
 */
void Manager::OnBtnClickAddFolder()
{
	TCHAR folder_path[MAX_PATH] = { 0 };		// �洢�ļ���·�� 
	TCHAR path_all_files[MAX_PATH] = { 0 };		// �洢�ļ����� *.* �ļ��������ļ��ı���
	TCHAR file_path_name[MAX_PATH] = { 0 };		// �洢ɸѡ���Ĵ�·�����ļ���
	BOOL is_match = FALSE;		// ��׼�Ƿ��ҵ�ƥ����ļ�
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = folder_path;
	bi.lpszTitle = _T("������ѡ�ļ���Ŀ¼:");
	bi.ulFlags = BIF_RETURNONLYFSDIRS;		// BIF_RETURNONLYFSDIRS ֻ����ѡ��Ŀ¼�� BIF_BROWSEINCLUDEFILES ������Ŀ¼���ļ�
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);	// ���ļ��Ի���

	if (NULL == idl)
		return;
	SHGetPathFromIDList(idl, folder_path);	// ��ȡ�ļ��е�·�� �磺D:\Dow\360Yun\video
	lstrcpy(path_all_files, folder_path);	// ����
	lstrcat(path_all_files, _T("\\*.*"));	// ����ɸѡͨ�����������ʽ�磺D:\Dow\360Yun\video\*.*
	lstrcat(folder_path, _T("\\"));			// ���ļ���·���� '\' ��β�������Ժ������ļ�����������ʽ�磺D:\Dow\360Yun\video\

	TraversingResource search_file;
	for (search_file.FirstFile(path_all_files); search_file.IsOK(); search_file.NextFile())
	{
		TCHAR ex_name[20] = { 0 };
		lstrcpy(ex_name, ::PathFindExtension(search_file.GetCurFile().cFileName));

		/* ��׺��Ϊ .avi �� .mp4 (���ִ�Сд) ʱ�������б� */
		if (lstrcmp(ex_name, _T(".")) && lstrcmp(ex_name, _T("..")) && !StrCmpI(ex_name, _T(".mp4")))
		{
			lstrcpy(file_path_name, folder_path);
			lstrcat(file_path_name, search_file.GetCurFile().cFileName);
			is_match = AddFileToList(file_path_name);
		}
	}
	if (!is_match)		// 
		MessageBox(NULL, _T("���ļ�����û���� .mp4 ����ƥ����ļ�"), _T("��Ϣ��ʾ"), MB_OK);

	CProgressUI *progress = static_cast<CProgressUI *>(paint_manager_->FindControl(_T("progress")));
	progress->SetValue(0);
}

/**
���ܣ���Ӧ����б�ť�ĵ���������Ƿ�����޸�ʧ�ܵ��ļ������Ƿ�������ʾ
*/
void Manager::OnBtnClickClear()
{
	CProgressUI *progress = static_cast<CProgressUI *>(paint_manager_->FindControl(_T("progress")));
	CLabelUI *prompt = static_cast<CLabelUI *>(paint_manager_->FindControl(_T("prompt")));
	progress->SetValue(0);
	prompt->SetText(_T("���޽����޸�����"));
	if (!fail_names_.empty())
	{
		switch (MessageBox(NULL, _T("�������޸�ʧ�ܵ��ļ����Ƿ������б��У�"), _T("��Ϣ��ʾ"), MB_YESNO))
		{
		case IDYES:	// �û�ѡ������������������б�������޸�ʧ�ܵ��ļ����б���
			file_path_list_->RemoveAll();
			for (vector<CDuiString>::iterator iter = fail_names_.begin(); iter != fail_names_.end(); ++iter)
			{
				list_element_point_ = new CListTextElementUI;
				file_path_list_->Add(list_element_point_);
				list_element_point_->SetText(0, iter->GetData());
			}
			break;
		case IDNO:		// �û�ѡ�񲻱�����ֱ����յ�ǰ�б�
			file_path_list_->RemoveAll();
			fail_names_.clear();
			break;
		default:
			break;
		}
	}
	else
		file_path_list_->RemoveAll();
}

/**
  ���ܣ���Ӧ�޸���ť�ĵ�����������޸��ɹ����������ڵ���ʾ��ѡ��
  ʵ�֣������ļ��б����� Exec() ���������ļ��޸�����û�ɹ����⽫�ļ������浽 vector ���� fail_name_ �С�
       ͨ���жϽ�������ֵ���ļ������Ƿ�һ�����ж��Ƿ����Ե��ļ������޸��ɹ���
       ����Ҫ�ٴ��޸�ʧ���ļ�������յ�ǰ�б��������ʧ���ļ����б��У�Ȼ������Լ�������һ�ε��ļ��޸�
 */
void Manager::OnBtnClickFix()
{
	CProgressUI *progress = static_cast<CProgressUI *>(paint_manager_->FindControl(_T("progress")));
	CLabelUI *prompt = static_cast<CLabelUI *>(paint_manager_->FindControl(_T("prompt")));
	TCHAR param[MAX_PATH + 10] = { 0 };	// �洢ָ�����
	TCHAR file_name[MAX_PATH] = { 0 };	// ���·�����ļ�����������δ�ܳɹ��޸����ļ�ʱ����ͨ���˱������ļ�����ӵ� vector ��
	TCHAR prompt_tchar[MAX_PATH + 10] = { 0 };	// �洢
	BOOL is_fail = TRUE;
	int file_count = file_path_list_->GetCount();
	if (0 == file_count)
	{
		MessageBox(NULL, _T("û������޸��ļ�"), _T("��Ϣ����"), MB_OK);
		return;
	}
	progress->SetMaxValue(file_count);	// ���ý����������ֵ
	progress->SetValue(0);				// ���ý������ĵ�ǰֵ
	fail_names_.clear();		// ��մ洢�޸�ʧ���ļ��� vector 
	for (int i = 0, j = 0; i < file_count; ++i)
	{
		lstrcpy(param, _T("\""));	// ����ڲ��� param ǰ����� " ��Ϊ�˱�֤�пո��ִ������Ҳ������ִ��
		lstrcpy(file_name, static_cast<CListTextElementUI *>(file_path_list_->GetItemAt(i))->GetText(0));
		lstrcat(param, file_name);
		lstrcat(param, _T("\""));
		lstrcpy(prompt_tchar, _T("�����޸��ļ�:"));
		lstrcat(prompt_tchar, file_name);
		prompt->SetText(prompt_tchar);
		if (-1 == Exec(_T("repair"), param, NULL, 0))
		{
			fail_names_.push_back(file_name);	// ��δ�ܳɹ��޸����ļ������浽 vector ��
			is_fail = FALSE;		// �������ļ��޸����ɹ���������ͽ��˱�׼��Ϊ FALSE 
			continue;
		}
		progress->SetValue(++j);	// ���ý�������ֵ
	}

	if (is_fail)		// ���ļ��޸����ɹ�
	{
		prompt->SetText(_T("�ļ��޸���ɣ�"));
		MessageBox(NULL, _T("�ļ��޸���ɣ�"), _T("��Ϣ��ʾ"), MB_OK);
	}
	else
	{
		if (IDRETRY == MessageBox(NULL, _T("���ļ��޸����ɹ����Ƿ����ԣ�"), _T("��Ϣ��ʾ"), MB_RETRYCANCEL))
		{	/* ��ѡ������ʱ�� ����������ļ��б������޸�ʧ�ܵ��ļ��ٴ���ӵ��б��У� Ȼ������Լ�ʵ���ٴ��޸� */
			file_path_list_->RemoveAll();
			for (vector<CDuiString>::iterator iter = fail_names_.begin(); iter != fail_names_.end(); ++iter)
			{
				list_element_point_ = new CListTextElementUI;
				file_path_list_->Add(list_element_point_);
				list_element_point_->SetText(0, iter->GetData());
			}
			OnBtnClickFix();	// �����޸����ݹ����
		}
		else
			prompt->SetText(_T("���޽����޸�����"));
	}
}

/**
  ���ܣ���Ӧ�ļ����Ϸţ������Ϸŵ�������涨��ʽ���ļ���ӵ��б����
  ʵ�֣����� DragQueryFile() ���������ļ�������� iFile ��Ϊ 0xFFFFFFFF ʱ���������سɹ��Ϸ��ļ��ĸ�����
       �ٴε��ã���ȡ���ļ��������ͨ�� DragFinish() �ͷ��� wParam ת�������Ľṹ����
  ������UINT uMsg, WPARAM wParam, LPARAM lParam
  ����ֵ��LRESULT
*/
LRESULT Manager::OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDROP hDrop = (HDROP)wParam;
	int nFileNum = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0); // ��ק�ļ�����
	TCHAR fine_name[MAX_PATH] = { 0 };
	TCHAR ex_name[20] = { 0 };
	for (int i = 0; i < nFileNum; i++)
	{
		DragQueryFile(hDrop, i, fine_name, MAX_PATH);//�����ҷ���ļ���
		lstrcpy(ex_name, ::PathFindExtension(fine_name));	// �õ��ļ���׺��
		if (lstrcmp(ex_name, _T(".")) && lstrcmp(ex_name, _T("..")) && !StrCmpI(ex_name, _T(".mp4")))
			AddFileToList(fine_name);
		else
		{
			lstrcat(fine_name, _T("\n���� .mp4 ����ƥ����ļ�\n������ѡ��"));
			MessageBox(NULL, fine_name, _T("��Ϣ��ʾ"), MB_OK);
		}
	}
	DragFinish(hDrop);      //�ͷ�hDrop

	return 0;
}

/**
���ܣ�����ļ���Ϣ���б��С������б����б����Ѿ���ͬ��Ϣ������ӣ�������ӣ�
������LPCTSTR file_path_name����Ҫ��ӵ��ļ���Ϣ����һ����·�����ļ����ַ�����
����ֵ��BOOL�� ��ӳɹ����� TURE�� ���Ѵ�����ͬ����Ϣ��������Ӳ��ɹ������� FALSE��
*/
BOOL Manager::AddFileToList(LPCTSTR file_path_name)
{
	for (int i = 0; i < file_path_list_->GetCount(); ++i)
	{
		if (lstrcmp(static_cast<CListTextElementUI *>(file_path_list_->GetItemAt(i))->GetText(0), file_path_name) == 0)
		{
			TCHAR hint[MAX_PATH + 20] = { 0 };
			lstrcpy(hint, file_path_name);
			lstrcat(hint, _T("\n���ļ��Ѿ����б��б������ڣ�\n������ѡ��"));
			MessageBox(NULL, hint, _T("��Ϣ��ʾ"), MB_OK);
			return FALSE;
		}
	}
	list_element_point_ = new CListTextElementUI;
	if (file_path_list_->Add(list_element_point_))
	{
		list_element_point_->SetText(0, file_path_name);
		return TRUE;
	}
	return FALSE;
}

/**
  ���ܣ�	���� ShellExecuteEx() ������ͨ�� SHELLEXECUTEINFO �ṹ�崫�ݲ����������� CMD �����в�ִ��������
  ������	const TCHAR* name �� ��Ҫ�򿪵�Ӧ�ó�����
        const TCHAR* param �� �����в���
        const TCHAR* dir �� Ӧ�ó���·���������Ϊ NULL ���ʾ��ǰĿ¼
        const TCHAR* relative_tool_dir
  ����ֵ��DWORD �� ��ʾ���� CMD ���˳�����
*/
DWORD Manager::Exec(const TCHAR* name, const TCHAR* param, const TCHAR* dir, const TCHAR* relative_tool_dir)
{
	rainbow::Path file;
	rainbow::Directory::getModuleDirAndSubDir(file);
	file.append(LPCTSTR(CString(relative_tool_dir ? relative_tool_dir : _T("")) + name));

	SHELLEXECUTEINFO ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = file;
	ShExecInfo.lpParameters = param;
	ShExecInfo.lpDirectory = dir;
	ShExecInfo.nShow = SW_HIDE;		// SW_HIDE ��ʾ����ʾ CMD ����
	ShExecInfo.hInstApp = NULL;
	if (!ShellExecuteEx(&ShExecInfo))
		return FALSE;
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);	// �ȴ� CMD ִ�н���

	DWORD exit_code = 0;
	if (!GetExitCodeProcess(ShExecInfo.hProcess, &exit_code))	// ��ȡһ�����жϽ��̵��˳�����
		exit_code = DWORD(-1);	// ��ʾִ��ʧ��
	CloseHandle(ShExecInfo.hProcess);	// �رվ��

	return exit_code;
}