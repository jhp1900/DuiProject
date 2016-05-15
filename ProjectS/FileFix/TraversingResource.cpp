#include "TraversingResource.h"


TraversingResource::TraversingResource()
{
	find_found_ = FALSE;
	find_handle_ = INVALID_HANDLE_VALUE;
	memset(&find_file_data_, 0, sizeof(find_file_data_));
}


TraversingResource::~TraversingResource()
{
	if (find_handle_ != INVALID_HANDLE_VALUE)
		::FindClose(find_handle_);
}

void TraversingResource::FirstFile(LPCTSTR file_name)
{
	find_handle_ = ::FindFirstFile(file_name, &find_file_data_);
	if (find_handle_ != INVALID_HANDLE_VALUE)
		find_found_ = TRUE;
	else
		find_found_ = FALSE;
}

void TraversingResource::NextFile()
{
	find_found_ = ::FindNextFile(find_handle_, &find_file_data_);
}

BOOL TraversingResource::IsOK() const
{
	return find_found_;
}

const WIN32_FIND_DATA &TraversingResource::GetCurFile()
{
	return find_file_data_;
}