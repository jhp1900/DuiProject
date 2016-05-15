#pragma once
#include <vector>
#include "stdafx.h"

using std::vector;

class TraversingResource
{
public:
	TraversingResource();
	~TraversingResource();

public:
	void FirstFile(LPCTSTR file_name);		// 第一个文件 
	void NextFile();		// 下一个文件
	BOOL IsOK() const;		// 是否可以查找下一个
	const WIN32_FIND_DATA &GetCurFile();	// 返回文件属性

protected:
	WIN32_FIND_DATA find_file_data_; // 当前文件属性
	HANDLE find_handle_;	// 当前查找位置句柄
	BOOL find_found_;		// 当前查找是否成功
};

