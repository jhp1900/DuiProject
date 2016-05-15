#include "manager.h"

LRESULT Manager::OnInit()
{

  {   // Layered 方式
    //SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    //// 设置透明色  
    //COLORREF clTransparent = RGB(0, 0, 0);
    //SetLayeredWindowAttributes(m_hWnd, clTransparent, 0, LWA_COLORKEY); // 设置异形
    //SetLayeredWindowAttributes(m_hWnd, 0, 125, LWA_ALPHA);    // 设置半透明
  }
  
  RECT rects[] = {
    { 10, 10, 66, 66 },
    { 76, 10, 132, 66 },
    { 142, 10, 198, 66 },
    { 10, 76, 66, 132 },
    { 76, 76, 132, 132 },
    { 142, 76, 198, 132 },
    { 10, 142, 66, 198 },
    { 76, 142, 132, 198 },
    { 142, 142, 198, 198 },
    { 5, 210, 203, 242 },
    { 5, 247, 203, 279 }
  };
  HRGN rgn = CreateRectRgn(0, 0, 0, 0);

  for (auto &i : rects) {
    HRGN temp = CreateRectRgnIndirect(&i);
    CombineRgn(rgn, rgn, temp, RGN_OR);
    DeleteRgn(temp);
  }

  SetWindowRgn(m_hWnd, rgn, TRUE);
  DeleteRgn(rgn);
  
  return 0;
}

LRESULT Manager::OnInitOtherWndMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
  return 0;
}

/**
  功能： 响应登陆窗体发来的登陆消息，即关闭登陆窗体，开启主窗体。
 */
LRESULT Manager::OnLoginMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	return 0;
}