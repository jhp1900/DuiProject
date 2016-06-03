#pragma once
#include "stdafx.h"

using std::vector;

class IpControlUI : public CHorizontalLayoutUI
{
public:
  IpControlUI();
  ~IpControlUI();

  BEGIN_DUIMSG_MAP(Manager)
  END_DUIMSG_MAP()

public:
  LPCTSTR GetClass() const override { return _T("IpControlUI"); }
  LPVOID GetInterface(LPCTSTR pstrName) override;
  virtual void Init() override;
  virtual CDuiString GetText() const override;
  virtual void SetText(LPCTSTR pstrText) override;

public:
  BOOL IsReasonable();
  void SetStateEdit(BOOL state);

private:
  vector<PDUI_EDIT> edit_vector_;
  vector<LPCTSTR> edit_str_;
};

