#pragma once
#include "window_impl_base.h"

class Manager : public WindowImplBase
{
public:
	Manager();
	~Manager() {}

public:
	virtual LRESULT OnInit() override;
	virtual CDuiString GetSkinFolder() override { return _T("skin"); }
	virtual CDuiString GetSkinFile() override { return _T("manager.xml"); }
	virtual LPCTSTR GetWindowClassName(void) const override { return _T("Manager"); }
};