/************************************************************************/
/*
* Author:      Clouderman
* Created:     2012-12-14
* Filename:    file_system.h
* Purpose:     File system.
* Note:
*/
/************************************************************************/
#ifndef RAINBOW_FILESYSTEM_H_
#define RAINBOW_FILESYSTEM_H_

#include "common_defs.h"
#include <shlwapi.h>
#include <shellapi.h>
#include <psapi.h>
#include <shlobj.h>

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "psapi.lib")

namespace rainbow
{

	//////////////////////////////////////////////////////////////////////////

	class Path // copyable
	{
	public:
		Path(const WCHAR* path = nullptr)
		{
			if (path && !::wcscpy_s(path_, MAX_PATH, path))
				return;

			path_[0] = L'\0';
		}

		// Because the data member is a static array, so no need 
		// to define the copy constructor and assign operator.

		Path& operator=(const WCHAR* path)
		{
			if (path && !::wcscpy_s(path_, MAX_PATH, path))
				return *this;

			path_[0] = L'\0';
			return *this;
		}

		virtual ~Path()
		{

		}

		operator const WCHAR*() const
		{
			return path_;
		}

		const WCHAR* c_str() const
		{
			return path_;
		}

		WCHAR* c_buf()
		{
			return path_;
		}

		bool empty() const
		{
			return path_[0] == L'\0';
		}

		bool addBackslash()
		{
			return ::PathAddBackslash(path_) != nullptr;
		}

		void removeBackslash()
		{
			::PathRemoveBackslash(path_);
		}

		bool addExtension(const WCHAR* extension)
		{
			return ::PathAddExtension(path_, extension) != FALSE;
		}

		const WCHAR* findExtension() const
		{
			return ::PathFindExtension(path_);
		}

		void removeExtension()
		{
			::PathRemoveExtension(path_);
		}

		bool renameExtension(const WCHAR* extension)
		{
			return ::PathRenameExtension(path_, extension) != FALSE;
		}

		const WCHAR* findArgs() const
		{
			return ::PathGetArgs(path_);
		}

		void removeArgs()
		{
			::PathRemoveArgs(path_);
		}

		void removeBlanks()
		{
			::PathRemoveBlanks(path_);
		}

		const WCHAR* findFilename() const
		{
			return ::PathFindFileName(path_);
		}

		bool removeFileSpec()
		{
			return ::PathRemoveFileSpec(path_) != FALSE;
		}

		bool append(const WCHAR* more)
		{
			return ::PathAppend(path_, more) != FALSE;
		}

		void buildRoot(int drive_index)
		{
			ASSERT(drive_index >= 0 && drive_index <= 25);
			::PathBuildRoot(path_, drive_index);
		}

		int getDriveIndex() const
		{
			return ::PathGetDriveNumber(path_);
		}

		void canonicalize()
		{
			WCHAR path[MAX_PATH];
			if (::PathCanonicalize(path, path_))
				::wcscpy_s(path_, MAX_PATH, path);
		}

		bool combine(const WCHAR* dir, const WCHAR* file)
		{
			return ::PathCombine(path_, dir, file) != nullptr;
		}

		Path commonPrefix(const WCHAR* other_path)
		{
			WCHAR path[MAX_PATH];
			::PathCommonPrefix(path_, other_path, path);
			return Path(path);
		}

		bool fileExists() const
		{
			return ::PathFileExists(path_) != FALSE;
		}

		bool isDirectory() const
		{
			return ::PathIsDirectory(path_) != FALSE;
		}

		bool isDirectoryEmpty() const
		{
			return ::PathIsDirectoryEmpty(path_) != FALSE;
		}

		bool isFileSpec() const
		{
			return ::PathIsFileSpec(path_) != FALSE;
		}

		bool isPrefix(const WCHAR* prefix) const
		{
			return ::PathIsPrefix(prefix, path_) != FALSE;
		}

		bool isRelative() const
		{
			return ::PathIsRelative(path_) != FALSE;
		}

		bool isRoot() const
		{
			return ::PathIsRoot(path_) != FALSE;
		}

		bool isSameRoot(const WCHAR* other_path) const
		{
			return ::PathIsSameRoot(path_, other_path) != FALSE;
		}

		bool isUNC() const
		{
			return ::PathIsUNC(path_) != FALSE;
		}

		bool isUNCServer() const
		{
			return ::PathIsUNCServer(path_) != FALSE;
		}

		bool isUNCServerShare() const
		{
			return ::PathIsUNCServerShare(path_) != FALSE;
		}

		bool makePretty()
		{
			return ::PathMakePretty(path_) != FALSE;
		}

		bool matchSpec(const WCHAR* spec) const
		{
			return ::PathMatchSpec(path_, spec) != FALSE;
		}

		bool quoteSpaces()
		{
			return ::PathQuoteSpaces(path_) != FALSE;
		}

		bool unquoteSpaces()
		{
			return ::PathUnquoteSpaces(path_) != FALSE;
		}

		bool relativePathTo(
			const WCHAR* from,
			DWORD attr_from,
			const WCHAR* to,
			DWORD attr_to
			)
		{
			return ::PathRelativePathTo(path_, from, attr_from, to, attr_to) != FALSE;
		}

		const WCHAR* skipRoot() const
		{
			return ::PathSkipRoot(path_);
		}

		void stripPath()
		{
			::PathStripPath(path_);
		}

		bool stripToRoot()
		{
			return ::PathStripToRoot(path_) != FALSE;
		}

	private:
		WCHAR path_[MAX_PATH];
	};

	//////////////////////////////////////////////////////////////////////////

	class Directory
	{
	public:
		/*
		*	This function works like ::SHCreateDirectory, ::SHCreateDirectoryEx.
		*  Becuse of the MSDN note the two functions might be altered or
		*	unavailable in subsequent versions of Windows. So I wrapper
		*  ::CreateDirectory to do it.
		*/
		static bool createDir(
			const Path& path,
			SECURITY_ATTRIBUTES* security_attributes = nullptr
			) NOEXCEPT
		{
			Path path_prefix;
			const WCHAR* pos = ::wcschr(path, L'\\');
			while (pos != nullptr)
			{
				if (::wcsncpy_s(path_prefix.c_buf(), MAX_PATH, path, pos - path) != 0)
					return false;

				if (!path_prefix.fileExists() && !::CreateDirectory(path_prefix, security_attributes))
					return false;

				pos = ::wcschr(++pos, L'\\');
			}

			if (path.fileExists())
				return true;

			return ::CreateDirectory(path, security_attributes) != FALSE;
		}

		static bool removeDir(
			const Path& path,
			bool remove_files = false
			) NOEXCEPT
		{
			if (remove_files)
			{
				if (!path.isDirectory())
					return false;

				WCHAR from_path[MAX_PATH] = { 0 };
				if (::wcscpy_s(from_path, MAX_PATH, path))
					return false;

				// Must be double-null terminated.
				from_path[::wcslen(from_path) + 1] = L'\0';

				SHFILEOPSTRUCT fopt = { 0 };
				fopt.hwnd = nullptr;
				fopt.wFunc = FO_DELETE;
				fopt.pFrom = from_path;
				fopt.pTo = nullptr;
				fopt.fFlags = FOF_NO_UI;
				return ::SHFileOperation(&fopt) == 0;
			}
			else
			{
				return ::RemoveDirectory(path) != FALSE;
			}
		}

		//////////////////////////////////////////////////////////////////////////

		static bool setCurrentDir(const Path& path) NOEXCEPT
		{
			return ::SetCurrentDirectory(path) != FALSE;
		}

		static bool getCurrentDir(Path& path) NOEXCEPT
		{
			return ::GetCurrentDirectory(MAX_PATH, path.c_buf()) != 0
				&& path.addBackslash();
		}

		static bool getTempDir(Path& path) NOEXCEPT
		{
			return ::GetTempPath(MAX_PATH, path.c_buf()) != 0
				&& path.addBackslash();
		}

		static bool getSystemDir(Path& path) NOEXCEPT
		{
			return ::GetSystemDirectory(path.c_buf(), MAX_PATH) != 0
				&& path.addBackslash();
		}

		static bool getSystemWindowsDir(Path& path) NOEXCEPT
		{
			return ::GetSystemWindowsDirectory(path.c_buf(), MAX_PATH) != 0
				&& path.addBackslash();
		}

		static bool getSystemWow64Dir(Path& path) NOEXCEPT
		{
			return ::GetSystemWow64Directory(path.c_buf(), MAX_PATH) != 0
				&& path.addBackslash();
		}

		static bool getWindowsDir(Path& path) NOEXCEPT
		{
			return ::GetWindowsDirectory(path.c_buf(), MAX_PATH) != 0
				&& path.addBackslash();
		}

		//////////////////////////////////////////////////////////////////////////

		static bool getModuleDirAndSubDir(
			Path& path,
			const WCHAR* subdir = nullptr,
			bool create_subdir = false,
			HANDLE process = nullptr,
			HMODULE module = nullptr
			) NOEXCEPT
		{
			if (!process)
				process = ::GetCurrentProcess();

			if (::GetModuleFileNameEx(process, module, path.c_buf(), MAX_PATH)
				&& path.removeFileSpec()
				&& path.addBackslash()
				)
			{
				if (!subdir)
					return true;

				if (path.append(subdir) && path.addBackslash())
				{
					if (path.fileExists())
						return true;
					else if (create_subdir && createDir(path, nullptr))
						return true;
				}
			}

			path.c_buf()[0] = L'\0';
			return false;
		}

		static bool getSpecailDirAndSubDir(
			int csidl,
			Path& path,
			const WCHAR* subdir = nullptr,
			bool create_subdir = false,
			DWORD flags = SHGFP_TYPE_CURRENT,
			HANDLE token = nullptr
			) NOEXCEPT
		{
			HRESULT hr = ::SHGetFolderPathAndSubDir(nullptr, csidl,
				token, flags, subdir, path.c_buf());

			if (FAILED(hr) && create_subdir)
			{
				hr = ::SHGetFolderPathAndSubDir(nullptr, csidl,
					token, flags, nullptr, path.c_buf());
				if (SUCCEEDED(hr) && path.append(subdir))
					return createDir(path, nullptr) && path.addBackslash();
			}
			return SUCCEEDED(hr) && path.addBackslash();
		}
	};

	//////////////////////////////////////////////////////////////////////////

	// class File : public InStream, public OutStream // noncopyable
	// {
	// public:
	//     File();
	//     virtual ~File();
	// 
	// private:
	//     File(const File&);
	//     File& operator=(const File&);
	// 
	// private:
	//     HANDLE _file;
	// };

	//////////////////////////////////////////////////////////////////////////

} // namespace rainbow

#endif // !RAINBOW_FILESYSTEM_H_
