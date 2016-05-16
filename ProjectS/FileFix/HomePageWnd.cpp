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
功能：初始化成员变量、通过 DragAcceptFiles() 函数让窗体接受文件的拖放
*/
void Manager::InitWindow()
{
	DragAcceptFiles(m_hWnd, TRUE);
	file_path_list_ = static_cast<CListUI *>(paint_manager_->FindControl(_T("files_list")));
}

/**
功能：用户消息路由。根据消息来源的控件名来路由消息响应
参数：const TNotifyUI& msg 消息结构体
*/
BOOL Manager::OnClickUser(const TNotifyUI& msg)
{
	if (msg.pSender->GetName() == TEXT("add_file"))				//	添加文件路径
	{
		OnBtnClickAddFile();
		return TRUE;
	}
	else if (msg.pSender->GetName() == TEXT("add_folder"))		//	添加文件夹路径
	{
		OnBtnClickAddFolder();
		return TRUE;
	}
	else if (msg.pSender->GetName() == TEXT("clear"))			//	清空列表
	{
		OnBtnClickClear();
		return TRUE;
	}
	else if (msg.pSender->GetName() == TEXT("fix"))				//	修复文件
	{
		OnBtnClickFix();
		return TRUE;
	}

	return FALSE;
}

/**
功能：复写基类的 消息控制函数，为了接受到 WM_DROPFILES（文件拖放）消息.
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
  功能：打开选择文件对话框，将选择文件的路径信息添加到列表框中, 支持多个文件的选中。
  实现：通过调用 GetOpenFileName() 函数，将存储了相关信息的 OPENFILENAME 结构体作为参数传入，同时将文件信息也存储在该结构体传出。
        文件路径、名称的属性存储在结构体的 lpstrFile 成员中，两种风格的格式分别为：
          Explorer风格：D:\Dow\360Yun\videoNULL111.aviNULL222.mp4NULLNULL (本风格使用 NULL 做为分割符， 末尾额外有个 NULL)
          旧风格，以空格作为分割，并使用短文件名
 */
void Manager::OnBtnClickAddFile()
{
	TCHAR open_files_name[80 * MAX_PATH] = { 0 };	// 存储所有所选文件的路径
	TCHAR files_name[MAX_PATH] = { 0 };		// 截取出来的单个文件带路径文件名
	TCHAR folder_path[MAX_PATH] = { 0 };	// 文件夹路径
	OPENFILENAME open_file_struct = { 0 };
	open_file_struct.lStructSize = sizeof(OPENFILENAME);
	open_file_struct.nMaxFile = MAX_PATH;	// 必填
	open_file_struct.lpstrFilter = _T("MP4文件(*.mp4)\0*.mp4\0\0");
	open_file_struct.lpstrFile = open_files_name;
	open_file_struct.nFilterIndex = 1;
	/* 下面风格的设置，填了 OFN_ALLOWMULTISELECT 可支持多文件的选取，但需要 OFN_EXPLORER 的配合，不然默认为旧的浏览模式 */
	open_file_struct.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

	if (::GetOpenFileName(&open_file_struct))
	{
		lstrcpyn(folder_path, open_files_name, open_file_struct.nFileOffset);	// 截取路径
		folder_path[open_file_struct.nFileOffset] = '\0';	// 添加结束标志
		int name_len = lstrlen(folder_path);
		if (folder_path[name_len - 1] != '\\')
			lstrcat(folder_path, _T("\\"));
		TCHAR *p = open_files_name + open_file_struct.nFileOffset;

		while (*p)
		{
			ZeroMemory(files_name, sizeof(files_name));
			lstrcat(files_name, folder_path);
			lstrcat(files_name, p);
			p += lstrlen(p) + 1;	// 使指向下一个文件

			AddFileToList(files_name);
		}
	}

	CProgressUI *progress = static_cast<CProgressUI *>(paint_manager_->FindControl(_T("progress")));
	progress->SetValue(0);
}

/**
  功能：添加一个文件夹下的匹配文件到列表中。
  实现：使用 BROWSEINFO 结果体来保存相关配置信息， 通过 SHBrowseForFolder() 函数和 SHGetPathFromIDList() 函数获取文件夹所在路径。
        再通过 TraversingResource 类实现文件的遍历，遍历的同时筛选出需要的文件类型，添加到列表中
 */
void Manager::OnBtnClickAddFolder()
{
	TCHAR folder_path[MAX_PATH] = { 0 };		// 存储文件夹路径 
	TCHAR path_all_files[MAX_PATH] = { 0 };		// 存储文件夹下 *.* 文件，用于文件的遍历
	TCHAR file_path_name[MAX_PATH] = { 0 };		// 存储筛选出的带路径的文件名
	BOOL is_match = FALSE;		// 标准是否找到匹配的文件
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = folder_path;
	bi.lpszTitle = _T("从下面选文件夹目录:");
	bi.ulFlags = BIF_RETURNONLYFSDIRS;		// BIF_RETURNONLYFSDIRS 只用于选中目录， BIF_BROWSEINCLUDEFILES 可用于目录和文件
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);	// 打开文件对话框

	if (NULL == idl)
		return;
	SHGetPathFromIDList(idl, folder_path);	// 获取文件夹的路径 如：D:\Dow\360Yun\video
	lstrcpy(path_all_files, folder_path);	// 拷贝
	lstrcat(path_all_files, _T("\\*.*"));	// 连接筛选通配符，最终形式如：D:\Dow\360Yun\video\*.*
	lstrcat(folder_path, _T("\\"));			// 让文件夹路径以 '\' 结尾，方便稍后连接文件名。最终形式如：D:\Dow\360Yun\video\

	TraversingResource search_file;
	for (search_file.FirstFile(path_all_files); search_file.IsOK(); search_file.NextFile())
	{
		TCHAR ex_name[20] = { 0 };
		lstrcpy(ex_name, ::PathFindExtension(search_file.GetCurFile().cFileName));

		/* 后缀名为 .avi 或 .mp4 (区分大小写) 时，加入列表 */
		if (lstrcmp(ex_name, _T(".")) && lstrcmp(ex_name, _T("..")) && !StrCmpI(ex_name, _T(".mp4")))
		{
			lstrcpy(file_path_name, folder_path);
			lstrcat(file_path_name, search_file.GetCurFile().cFileName);
			is_match = AddFileToList(file_path_name);
		}
	}
	if (!is_match)		// 
		MessageBox(NULL, _T("该文件夹里没有与 .mp4 类型匹配的文件"), _T("信息提示"), MB_OK);

	CProgressUI *progress = static_cast<CProgressUI *>(paint_manager_->FindControl(_T("progress")));
	progress->SetValue(0);
}

/**
功能：响应清除列表按钮的点击，根据是否存在修复失败的文件给出是否保留的提示
*/
void Manager::OnBtnClickClear()
{
	CProgressUI *progress = static_cast<CProgressUI *>(paint_manager_->FindControl(_T("progress")));
	CLabelUI *prompt = static_cast<CLabelUI *>(paint_manager_->FindControl(_T("prompt")));
	progress->SetValue(0);
	prompt->SetText(_T("暂无进行修复工作"));
	if (!fail_names_.empty())
	{
		switch (MessageBox(NULL, _T("存在着修复失败的文件，是否保留在列表中？"), _T("消息提示"), MB_YESNO))
		{
		case IDYES:	// 用户选择保留，则先清空现有列表，再添加修复失败的文件到列表中
			file_path_list_->RemoveAll();
			for (vector<CDuiString>::iterator iter = fail_names_.begin(); iter != fail_names_.end(); ++iter)
			{
				list_element_point_ = new CListTextElementUI;
				file_path_list_->Add(list_element_point_);
				list_element_point_->SetText(0, iter->GetData());
			}
			break;
		case IDNO:		// 用户选择不保留，直接清空当前列表
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
  功能：响应修复按钮的点击，并根据修复成功与否给出对于的提示和选择。
  实现：遍历文件列表，调用 Exec() 函数进行文件修复，若没成功，这将文件名保存到 vector 变量 fail_name_ 中。
       通过判断进度条数值与文件总数是否一致来判断是否所以的文件都已修复成功，
       若需要再次修复失败文件，则清空当前列表，重新添加失败文件到列表中，然后调用自己进行新一次的文件修复
 */
void Manager::OnBtnClickFix()
{
	CProgressUI *progress = static_cast<CProgressUI *>(paint_manager_->FindControl(_T("progress")));
	CLabelUI *prompt = static_cast<CLabelUI *>(paint_manager_->FindControl(_T("prompt")));
	TCHAR param[MAX_PATH + 10] = { 0 };	// 存储指令参数
	TCHAR file_name[MAX_PATH] = { 0 };	// 存带路径的文件名，当存在未能成功修复的文件时，便通过此变量将文件名添加到 vector 里
	TCHAR prompt_tchar[MAX_PATH + 10] = { 0 };	// 存储
	BOOL is_fail = TRUE;
	int file_count = file_path_list_->GetCount();
	if (0 == file_count)
	{
		MessageBox(NULL, _T("没有添加修复文件"), _T("消息提醒"), MB_OK);
		return;
	}
	progress->SetMaxValue(file_count);	// 设置进度条的最大值
	progress->SetValue(0);				// 设置进度条的当前值
	fail_names_.clear();		// 清空存储修复失败文件的 vector 
	for (int i = 0, j = 0; i < file_count; ++i)
	{
		lstrcpy(param, _T("\""));	// 这里，在参数 param 前后添加 " 是为了保证有空格的执行命令也能正常执行
		lstrcpy(file_name, static_cast<CListTextElementUI *>(file_path_list_->GetItemAt(i))->GetText(0));
		lstrcat(param, file_name);
		lstrcat(param, _T("\""));
		lstrcpy(prompt_tchar, _T("正在修复文件:"));
		lstrcat(prompt_tchar, file_name);
		prompt->SetText(prompt_tchar);
		if (-1 == Exec(_T("repair"), param, NULL, 0))
		{
			fail_names_.push_back(file_name);	// 将未能成功修复的文件名保存到 vector 里
			is_fail = FALSE;		// 当存在文件修复不成功的情况，就将此标准设为 FALSE 
			continue;
		}
		progress->SetValue(++j);	// 设置进度条的值
	}

	if (is_fail)		// 有文件修复不成功
	{
		prompt->SetText(_T("文件修复完成！"));
		MessageBox(NULL, _T("文件修复完成！"), _T("消息提示"), MB_OK);
	}
	else
	{
		if (IDRETRY == MessageBox(NULL, _T("有文件修复不成功，是否重试？"), _T("消息提示"), MB_RETRYCANCEL))
		{	/* 当选择重试时， 就清空现有文件列表，并将修复失败的文件再次添加到列表中， 然后调用自己实现再次修复 */
			file_path_list_->RemoveAll();
			for (vector<CDuiString>::iterator iter = fail_names_.begin(); iter != fail_names_.end(); ++iter)
			{
				list_element_point_ = new CListTextElementUI;
				file_path_list_->Add(list_element_point_);
				list_element_point_->SetText(0, iter->GetData());
			}
			OnBtnClickFix();	// 重新修复，递归调用
		}
		else
			prompt->SetText(_T("暂无进行修复工作"));
	}
}

/**
  功能：响应文件的拖放，并将拖放的且满足规定格式的文件添加到列表框里
  实现：调用 DragQueryFile() 函数，将文件索引编号 iFile 设为 0xFFFFFFFF 时，函数返回成功拖放文件的个数；
       再次调用，获取到文件名，最后通过 DragFinish() 释放由 wParam 转换而来的结构体句柄
  参数：UINT uMsg, WPARAM wParam, LPARAM lParam
  返回值：LRESULT
*/
LRESULT Manager::OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDROP hDrop = (HDROP)wParam;
	int nFileNum = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0); // 拖拽文件个数
	TCHAR fine_name[MAX_PATH] = { 0 };
	TCHAR ex_name[20] = { 0 };
	for (int i = 0; i < nFileNum; i++)
	{
		DragQueryFile(hDrop, i, fine_name, MAX_PATH);//获得拖曳的文件名
		lstrcpy(ex_name, ::PathFindExtension(fine_name));	// 得到文件后缀名
		if (lstrcmp(ex_name, _T(".")) && lstrcmp(ex_name, _T("..")) && !StrCmpI(ex_name, _T(".mp4")))
			AddFileToList(fine_name);
		else
		{
			lstrcat(fine_name, _T("\n不是 .mp4 类型匹配的文件\n请重新选择"));
			MessageBox(NULL, fine_name, _T("信息提示"), MB_OK);
		}
	}
	DragFinish(hDrop);      //释放hDrop

	return 0;
}

/**
功能：添加文件信息到列表中。遍历列表，若列表中已经相同信息，则不添加，否则添加；
参数：LPCTSTR file_path_name，需要添加的文件信息，是一个带路径的文件名字符串；
返回值：BOOL， 添加成功返回 TURE； 当已存在相同的信息，或者添加不成功，返回 FALSE；
*/
BOOL Manager::AddFileToList(LPCTSTR file_path_name)
{
	for (int i = 0; i < file_path_list_->GetCount(); ++i)
	{
		if (lstrcmp(static_cast<CListTextElementUI *>(file_path_list_->GetItemAt(i))->GetText(0), file_path_name) == 0)
		{
			TCHAR hint[MAX_PATH + 20] = { 0 };
			lstrcpy(hint, file_path_name);
			lstrcat(hint, _T("\n该文件已经在列表列表框里存在！\n请重新选择！"));
			MessageBox(NULL, hint, _T("信息提示"), MB_OK);
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
  功能：	调用 ShellExecuteEx() 函数，通过 SHELLEXECUTEINFO 结构体传递参数，来启动 CMD 命令行并执行相关命令。
  参数：	const TCHAR* name ： 需要打开的应用程序名
        const TCHAR* param ： 命令行参数
        const TCHAR* dir ： 应用程序路径，如果设为 NULL 则表示当前目录
        const TCHAR* relative_tool_dir
  返回值：DWORD ： 表示调用 CMD 的退出代码
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
	ShExecInfo.nShow = SW_HIDE;		// SW_HIDE 表示不显示 CMD 窗口
	ShExecInfo.hInstApp = NULL;
	if (!ShellExecuteEx(&ShExecInfo))
		return FALSE;
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);	// 等待 CMD 执行结束

	DWORD exit_code = 0;
	if (!GetExitCodeProcess(ShExecInfo.hProcess, &exit_code))	// 获取一个已中断进程的退出代码
		exit_code = DWORD(-1);	// 表示执行失败
	CloseHandle(ShExecInfo.hProcess);	// 关闭句柄

	return exit_code;
}