// Operator.h : Include file for standard system include files,
// or project specific include files.

#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include <windows.h>
__inline static
void AppendEditText(HWND hWndEdit, LPCSTR pszFormat, ...)
{
	CHAR szText[4096] = { 0 };
	va_list vlArgs;
	va_start(vlArgs, pszFormat);
	_vsnprintf(szText, sizeof(szText) - 1, pszFormat, vlArgs);
	va_end(vlArgs);

	lstrcatA(szText, "\r\n");

	//以下两条语句为在edit中追加字符串
	SendMessageA(hWndEdit, EM_SETSEL, -2, -1);
	SendMessageA(hWndEdit, EM_REPLACESEL, (WPARAM)TRUE, (LPARAM)szText);

	//设置滚轮到末尾，这样就可以看到最新信息
	SendMessageA(hWndEdit, WM_VSCROLL, SB_BOTTOM, 0);
}
__inline static
void AppendEditText(HWND hWndEdit, LPCWSTR pszFormat, ...)
{
	WCHAR szText[4096] = { 0 };
	va_list vlArgs;
	va_start(vlArgs, pszFormat);
	_vsnwprintf(szText, sizeof(szText) - 1, pszFormat, vlArgs);
	va_end(vlArgs);

	lstrcatW(szText, L"\r\n");

	//以下两条语句为在edit中追加字符串
	SendMessageW(hWndEdit, EM_SETSEL, -2, -1);
	SendMessageW(hWndEdit, EM_REPLACESEL, (WPARAM)TRUE, (LPARAM)szText);

	//设置滚轮到末尾，这样就可以看到最新信息
	SendMessageW(hWndEdit, WM_VSCROLL, SB_BOTTOM, 0);
}

typedef struct RectArea {
public:
	RectArea(RectArea* pRectArea)
	{
		this->left = pRectArea->left;
		this->top = pRectArea->top;
		this->right = pRectArea->right;
		this->bottom = pRectArea->bottom;
	}
	RectArea(const RectArea& rectArea)
	{
		this->left = rectArea.left;
		this->top = rectArea.top;
		this->right = rectArea.right;
		this->bottom = rectArea.bottom;
	}
	RectArea(LONG left, LONG top, LONG right, LONG bottom)
	{
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
	}
	RectArea(const RECT& rect)
	{
		this->left = rect.left;
		this->top = rect.top;
		this->right = rect.right;
		this->bottom = rect.bottom;
	}
	RectArea(LPRECT lpRect)
	{
		this->left = lpRect->left;
		this->top = lpRect->top;
		this->right = lpRect->right;
		this->bottom = lpRect->bottom;
	}
	RectArea(LPPOINT lpPoint, LPSIZE lpSize)
	{
		this->left = lpPoint->x;
		this->top = lpPoint->y;
		this->right = lpSize->cx - this->left;
		this->bottom = lpSize->cy - this->top;
	}
	RectArea(const POINT& point, const SIZE& size)
	{
		this->left = point.x;
		this->top = point.y;
		this->right = size.cx - this->left;
		this->bottom = size.cy - this->top;
	}
public:
	LONG    left;
	LONG    top;
	LONG    right;
	LONG    bottom;
public:
	LONG X() {
		return left;
	}
	LONG Y() {
		return top;
	}
	LONG Width() {
		return (right - left);
	}
	LONG Height() {
		return (bottom - top);
	}
	void X(LONG X) {
		right = X + Width();
		left = X;
	}
	void Y(LONG Y) {
		bottom = Y + Height();
		top = Y;
	}
	void Width(LONG width) {
		right = left + width;
	}
	void Height(LONG height) {
		bottom = top + height;
	}
};
__inline static
LPRECT ScreenToDialog(HWND hWnd, LPRECT lpRect, INT nRectNum = 1)
{
	for (SIZE_T i = 0; i < nRectNum * sizeof(RECT) / sizeof(POINT); i++)
	{
		::ScreenToClient(hWnd, &((LPPOINT)lpRect)[i]);
	}
	return lpRect;
}

__inline static
LPPOINT ScreenToClient(HWND hWnd, LPPOINT lpPoint, INT nPointNum)
{
	for (SIZE_T i = 0; i < nPointNum; i++)
	{
		::ScreenToClient(hWnd, &((LPPOINT)lpPoint)[i]);
	}
	return lpPoint;
}

__inline static
LPRECT GetDlgItemRect(RECT& rcRect, HWND hWnd, INT nIDWndItem)
{
	::GetWindowRect(::GetDlgItem(hWnd, nIDWndItem), &rcRect);
	return ScreenToDialog(hWnd, &rcRect, sizeof(rcRect) / sizeof(RECT));
}

__inline static
void CalculateWindowSize(RECT& rect, INT nSpaceX, INT nSpaceY)
{
	rect.right -= nSpaceX;
	rect.bottom -= nSpaceY;
}
__inline static
void CalculateWindowSize(RECT& rect, INT nSpaceL, INT nSpaceT, INT nSpaceR, INT nSpaceB)
{
	rect.left += nSpaceL;
	rect.top += nSpaceT;
	rect.right -= nSpaceR;
	rect.bottom -= nSpaceB;
}
__inline static
void CalculateWindowSize(RECT& rect, const RECT& newRect, INT nSpaceX, INT nSpaceY)
{
	rect.right = newRect.right - nSpaceX;
	rect.bottom = newRect.bottom - nSpaceY;
}
__inline static
void CalculateWindowSize(RECT& rect, const RECT& newRect, INT nSpaceL, INT nSpaceT, INT nSpaceR, INT nSpaceB)
{
	rect.left = newRect.left + nSpaceL;
	rect.top = newRect.top + nSpaceT;
	rect.right = newRect.right - nSpaceR;
	rect.bottom = newRect.bottom - nSpaceB;
}
__inline static
void ResizeWindow(HWND hWnd, INT nIDWndItem, LPCRECT lpRect, BOOL bRepaint = TRUE)
{
	MoveWindow(GetDlgItem(hWnd, nIDWndItem), lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, bRepaint);
}
__inline static
void NotifyUpdateWindowControl(HWND hWnd, INT nIDWndItem, INT nSpaceX, INT nSpaceY)
{
	RECT rc = { 0 }; GetDlgItemRect(rc, hWnd, nIDWndItem);
	rc.right += nSpaceX; rc.bottom += nSpaceY; ::InvalidateRect(hWnd, &rc, TRUE);
}
__inline static
void NotifyUpdateWindowControl(HWND hWnd, INT nIDWndItem, INT nSpaceL, INT nSpaceT, INT nSpaceR, INT nSpaceB)
{
	RECT rc = { 0 }; GetDlgItemRect(rc, hWnd, nIDWndItem);
	rc.left -= nSpaceL;	rc.top -= nSpaceT;	rc.right += nSpaceR; rc.bottom += nSpaceB; ::InvalidateRect(hWnd, &rc, TRUE);
}

__inline static
void DisplayRequestRepaint(HWND hWnd, LONG width, LONG height, BOOL bRepaint)
{
	RECT rect = { 0 }; GetWindowRect(hWnd, &rect);
	rect.left -= (width - (rect.right - rect.left)) / 2;
	rect.top -= (height - (rect.bottom - rect.top)) / 2;
	rect.right = rect.left + width; rect.bottom = rect.top + height;
	MoveWindow(hWnd, rect.left, rect.top, width, height, bRepaint);
}

__inline static
void FillSolidRect(HDC hdc, LONG x, LONG y, LONG cx, LONG cy, COLORREF clr)
{
	RECT rect = { x,y,x + cx,y + cy };
	HBRUSH hBrush = CreateSolidBrush(clr);
	FillRect(hdc, &rect, hBrush);
	DeleteObject(hBrush);
}
__inline static
void FillSolidRect(HDC hdc, LPRECT lpRect, COLORREF clr)
{
	FillSolidRect(hdc, lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, clr);
}
__inline static
void Draw3dRect(HDC hdc, LONG x, LONG y, LONG cx, LONG cy, COLORREF clrLeftTop, COLORREF clrRightBottom, LONG border = 1)
{
	FillSolidRect(hdc, x, y, border, cy - border, clrLeftTop); // left
	FillSolidRect(hdc, x, y, cx - border, border, clrLeftTop); // top
	FillSolidRect(hdc, x + cx, y, -border, cy, clrRightBottom); // right
	FillSolidRect(hdc, x, y + cy, cx, -border, clrRightBottom); // bottom
}
__inline static
void Draw3dRect(HDC hdc, LPRECT lpRect, COLORREF clrLeftTop, COLORREF clrRightBottom, LONG border = 1)
{
	Draw3dRect(hdc, lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, clrLeftTop, clrRightBottom, border);
}

#include <shellapi.h>
#include <string>
#include <unordered_map>
#if !defined(_UNICODE) && !defined(UNICODE)
#define TSTRING std::string
#else
#define TSTRING std::wstring
#endif
__inline static 
void RegisterDropFilesEvent(HWND hWnd)
{
#ifndef WM_COPYGLOBALDATA
#define WM_COPYGLOBALDAYA	0x0049
#endif

#ifndef MSGFLT_ADD
#define MSGFLT_ADD 1
#endif

#ifndef MSGFLT_REMOVE
#define MSGFLT_REMOVE 2
#endif
	SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_ACCEPTFILES);
	typedef BOOL(WINAPI* LPFN_ChangeWindowMessageFilter)(__in UINT message, __in DWORD dwFlag);
	LPFN_ChangeWindowMessageFilter pfnChangeWindowMessageFilter = (LPFN_ChangeWindowMessageFilter)GetProcAddress(GetModuleHandle(TEXT("USER32.DLL")), ("ChangeWindowMessageFilter"));
	if (pfnChangeWindowMessageFilter)
	{
		pfnChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
		pfnChangeWindowMessageFilter(WM_COPYDATA, MSGFLT_ADD);
		pfnChangeWindowMessageFilter(WM_COPYGLOBALDAYA, MSGFLT_ADD);// 0x0049 == WM_COPYGLOBALDATA
	}
}
__inline static 
size_t GetDropFiles(std::unordered_map<TSTRING, TSTRING>* pttmap, HDROP hDropInfo)
{
	UINT nIndex = 0;
	UINT nNumOfFiles = 0;
	TCHAR tszFilePathName[MAX_PATH + 1] = { 0 };

	//得到文件个数
	nNumOfFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	for (nIndex = 0; nIndex < nNumOfFiles; nIndex++)
	{
		//得到文件名
		DragQueryFile(hDropInfo, nIndex, (LPTSTR)tszFilePathName, _MAX_PATH);
		pttmap->emplace(tszFilePathName, tszFilePathName);
	}

	DragFinish(hDropInfo);

	return nNumOfFiles;
}
__inline static 
size_t GetDropFiles(std::vector<TSTRING>* ptv, HDROP hDropInfo)
{
	UINT nIndex = 0;
	UINT nNumOfFiles = 0;
	TCHAR tszFilePathName[MAX_PATH + 1] = { 0 };

	//得到文件个数
	nNumOfFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	for (nIndex = 0; nIndex < nNumOfFiles; nIndex++)
	{
		//得到文件名
		DragQueryFile(hDropInfo, nIndex, (LPTSTR)tszFilePathName, _MAX_PATH);
		ptv->emplace_back(tszFilePathName);
	}

	DragFinish(hDropInfo);

	return nNumOfFiles;
}
//显示在屏幕中央
__inline static 
void CenterWindowInScreen(HWND hWnd)
{
	RECT rcWindow = { 0 };
	RECT rcScreen = { 0 };
	SIZE szAppWnd = { 0, 0 };
	POINT ptAppWnd = { 0, 0 };

	// Get workarea rect.
	BOOL fResult = SystemParametersInfo(SPI_GETWORKAREA,   // Get workarea information
		0,              // Not used
		&rcScreen,    // Screen rect information
		0);             // Not used

	GetWindowRect(hWnd, &rcWindow);
	szAppWnd.cx = rcWindow.right - rcWindow.left;
	szAppWnd.cy = rcWindow.bottom - rcWindow.top;

	//居中显示
	ptAppWnd.x = (rcScreen.right - rcScreen.left - szAppWnd.cx) / 2;
	ptAppWnd.y = (rcScreen.bottom - rcScreen.top - szAppWnd.cy) / 2;
	MoveWindow(hWnd, ptAppWnd.x, ptAppWnd.y, szAppWnd.cx, szAppWnd.cy, TRUE);
}

//显示在父窗口中央
__inline static 
void CenterWindowInParent(HWND hWnd, HWND hParentWnd)
{
	RECT rcWindow = { 0 };
	RECT rcScreen = { 0 };
	RECT rcParent = { 0 };
	SIZE szAppWnd = { 0, 0 };
	POINT ptAppWnd = { 0, 0 };
	// Get workarea rect.
	BOOL fResult = SystemParametersInfo(SPI_GETWORKAREA,   // Get workarea information
		0,              // Not used
		&rcScreen,    // Screen rect information
		0);             // Not used

	GetWindowRect(hParentWnd, &rcParent);
	GetWindowRect(hWnd, &rcWindow);
	szAppWnd.cx = rcWindow.right - rcWindow.left;
	szAppWnd.cy = rcWindow.bottom - rcWindow.top;

	//居中显示
	if ((rcWindow.right - rcWindow.left) > (rcParent.right - rcParent.left))
	{
		ptAppWnd.x = (rcScreen.right - rcScreen.left - szAppWnd.cx) / 2;
	}
	else
	{
		ptAppWnd.x = (rcParent.right + rcParent.left - szAppWnd.cx) / 2;
	}
	if ((rcWindow.bottom - rcWindow.top) > (rcParent.bottom - rcParent.top))
	{
		ptAppWnd.y = (rcScreen.bottom - rcScreen.top - szAppWnd.cy) / 2;
	}
	else
	{
		ptAppWnd.y = (rcParent.bottom + rcParent.top - szAppWnd.cy) / 2;
	}
	MoveWindow(hWnd, ptAppWnd.x, ptAppWnd.y, szAppWnd.cx, szAppWnd.cy, TRUE);
}