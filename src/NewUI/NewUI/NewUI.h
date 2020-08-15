// PpsHik.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <operator.h>
#include <interface.h>
#include <securityprotects.h>
#include <converter.h>
#include <randnonce.h>
#include <windowapi.h>
#include <Authority.h>
#include <Iexplorer.h>
#include <nifnotify.h>
#include <dbgoutput.h>

#include <tlhelp32.h>

#define SPACE_X 10
#define SPACE_Y 10

typedef enum ControlID {
	ID_CONTROL_MIN = WM_USER,
	ID_EDIT_USERNAME_TEXT,
	ID_EDIT_MARKDOWN_TEXT,
	ID_EDIT_STATUS_TEXT,
	ID_BUTTON_MINIMUM,
	ID_BUTTON_OPEN_URL= ID_BUTTON_MINIMUM,
	ID_BUTTON_AUTOLOGON,
	ID_BUTTON_ENTER_INBOX,
	ID_BUTTON_SELECT_MAIL,
	ID_BUTTON_READ_MAIL,
	ID_BUTTON_MAXIMUM = ID_BUTTON_READ_MAIL,
	ID_CONTROL_MAX,
	ID_CONTROL_NUM = ID_CONTROL_MAX - ID_CONTROL_MIN,
};

BOOL CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	BOOL bHandled = FALSE;

	NotifyTray::Inst()->HandleEvent(hDlg, uMsg, wParam, lParam, bHandled);
	IexplorerUtils::Inst()->HandleEvent(hDlg, uMsg, wParam, lParam, bHandled);

	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(128));
		SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)hIcon);
		SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hIcon);
		SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_SMALL2, (LPARAM)hIcon);

		//SetWindowLong(GetDlgItem(hDlg, ID_BUTTON_DBGLOGCAT), GWL_STYLE, GetWindowLong(GetDlgItem(hDlg, ID_BUTTON_DBGLOGCAT), GWL_STYLE) | BS_OWNERDRAW);

		DisplayRequestRepaint(hDlg, 800, 600, TRUE);

		SetWindowText(hDlg, TEXT("MarkDown工具"));
		NotifyTray::Inst()->init(hDlg, hIcon, TEXT("MarkDown工具"));

		SetDlgItemText(hDlg, ID_EDIT_USERNAME_TEXT, TEXT("请输入MarkDown用户名"));
		RECT rcDlg = { 0 };
		RECT rcBtn = { 0 };
		GetDlgItemRect(rcBtn, hDlg, ID_BUTTON_OPEN_URL);
		GetClientRect(hDlg, &rcDlg);
		rcDlg.left = SPACE_X;
		rcDlg.top = rcBtn.bottom + SPACE_Y;
		rcDlg.right = rcDlg.left + (rcDlg.right - SPACE_X - SPACE_X - SPACE_X) / 2;
		rcDlg.bottom = rcDlg.bottom - SPACE_Y;
		MoveWindow(GetDlgItem(hDlg, ID_EDIT_MARKDOWN_TEXT), rcDlg.left, rcDlg.top, rcDlg.right - rcDlg.left, rcDlg.bottom - rcDlg.top, TRUE);
		SendMessage(GetDlgItem(hDlg, ID_EDIT_MARKDOWN_TEXT), EM_LIMITTEXT, (WPARAM)UINT_MAX, (LPARAM)0);
		GetClientRect(hDlg, &rcDlg);
		rcDlg.left = rcDlg.left + (rcDlg.right - SPACE_X - SPACE_X - SPACE_X) / 2 + SPACE_X;
		rcDlg.top = rcBtn.bottom + SPACE_Y;
		rcDlg.right = rcDlg.right - SPACE_X;
		rcDlg.bottom = rcDlg.bottom - SPACE_Y;
		IexplorerUtils::Inst()->Init(hDlg, rcDlg);
		IexplorerUtils::Inst()->PutSilent(TRUE);
		IexplorerUtils::Inst()->PutStatusCtrlID(ID_EDIT_STATUS_TEXT);
	}
	return TRUE;
	case WM_DRAWITEM:
	{
		RECT rect;
		LPDRAWITEMSTRUCT lpDrawItemStruct = (LPDRAWITEMSTRUCT)lParam;
		UINT itemState = lpDrawItemStruct->itemState;
		if (lpDrawItemStruct->CtlID >= ID_BUTTON_MINIMUM
			&& lpDrawItemStruct->CtlID <= ID_BUTTON_MAXIMUM || lpDrawItemStruct->CtlID == IDOK || lpDrawItemStruct->CtlID == IDCANCEL)
		{
			TCHAR tText[MAX_PATH] = { 0 };
			GetWindowText(lpDrawItemStruct->hwndItem, tText, sizeof(tText) / sizeof(*tText));
			GetClientRect(lpDrawItemStruct->hwndItem, &rect);

			UINT edge_type = EDGE_SUNKEN;
			COLORREF bk_color = RGB(0, 255, 0);
			COLORREF text_color = RGB(255, 0, 255);
			COLORREF lt_color = RGB(255, 0, 0);
			COLORREF rb_color = RGB(255, 0, 255);

			if ((itemState & ODS_SELECTED) == ODS_SELECTED)
			{
				edge_type = EDGE_SUNKEN;
				bk_color = RGB(0, 128, 0);
				text_color = RGB(123, 0, 255);
				lt_color = RGB(128, 0, 0);
				rb_color = RGB(128, 0, 255);
			}
			else if ((itemState & ODS_FOCUS) == ODS_FOCUS)
			{
				edge_type = EDGE_RAISED;
				bk_color = RGB(0, 180, 0);
				text_color = RGB(255, 255, 255);
				lt_color = RGB(180, 0, 180);
				rb_color = RGB(180, 0, 180);
			}
			else
			{
				edge_type = EDGE_RAISED;
				bk_color = RGB(0, 180, 0);
				text_color = RGB(255, 255, 255);
				lt_color = RGB(180, 0, 180);
				rb_color = RGB(180, 0, 180);
			}
			SetBkColor(lpDrawItemStruct->hDC, bk_color);
			SetTextColor(lpDrawItemStruct->hDC, text_color);
			FillSolidRect(lpDrawItemStruct->hDC, &rect, bk_color);
			Draw3dRect(lpDrawItemStruct->hDC, &rect, lt_color, rb_color, 3);
			DrawEdge(lpDrawItemStruct->hDC, &rect, edge_type, BF_RECT);
			DrawText(lpDrawItemStruct->hDC, tText, lstrlen(tText), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);//Redraw the Caption of Button Window 
		}

		return 0;
	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_BUTTON_OPEN_URL:
		{
			IexplorerUtils::Inst()->LoadHtml("");
			{
				//IexplorerUtils::Inst()->Nav2Url((BSTR)(StringConvertUtils::Instance()->AToW(APP_DIR) + OLESTR("/test.html")).c_str());
			}
			//IexplorerUtils::Inst()->FrameConvert();
		}
		return TRUE;
		case ID_BUTTON_AUTOLOGON:
		{
			IexplorerUtils::Inst()->FrameConvert();
		}
		return TRUE;
		case ID_BUTTON_ENTER_INBOX:
		{
			std::vector<std::string> tagList;
			std::vector<std::string> valueList;
			std::string strText(10 * 1024 * 1024, '\0');
			GetDlgItemText(hDlg, ID_EDIT_MARKDOWN_TEXT, (LPSTR)strText.data(), strText.size());
			//IexplorerUtils::Inst()->MarkdownToHtmlview(tagList, valueList, strText);
			//IexplorerUtils::Inst()->AddNewElem(tagList, valueList);
			//IexplorerUtils::Inst()->LoadHtml(IexplorerUtils::Inst()->MarkDownHtml(strText.c_str(), false).c_str());
		}
		return TRUE;
		case ID_BUTTON_SELECT_MAIL:
		{
			IexplorerUtils::Inst()->FrameConvert();
		}
		return TRUE;
		case ID_BUTTON_READ_MAIL:
		{
			IexplorerUtils::Inst()->FrameConvert();
		}
		return TRUE;
		//case IDOK:
		case IDCANCEL:
		{
			EndDialog(hDlg, 0);
			NotifyTray::Inst()->DelNotifyIcon();
			CSecurityProtects(2030, 9, 8).CheckValid();
		}
		return TRUE;
		}
	}
	break;
	case WM_SIZE:
	{
		RECT rcDlg = { 0 };
		RECT rcHtml = { 0 };
		GetClientRect(hDlg, &rcDlg);
		RECT rcBtn = { 0 };
		GetDlgItemRect(rcBtn, hDlg, ID_BUTTON_OPEN_URL);
		rcDlg.left = SPACE_X;
		rcDlg.top = rcBtn.bottom + SPACE_Y;
		rcDlg.right = rcDlg.left + (rcDlg.right - SPACE_X - SPACE_X - SPACE_X) / 2;
		rcDlg.bottom = rcDlg.bottom - SPACE_Y;
		MoveWindow(GetDlgItem(hDlg, ID_EDIT_MARKDOWN_TEXT), rcDlg.left, rcDlg.top, rcDlg.right - rcDlg.left, rcDlg.bottom - rcDlg.top, TRUE);

		GetClientRect(hDlg, &rcDlg);
		GetDlgItemRect(rcHtml, hDlg, GetDlgCtrlID(IexplorerUtils::Inst()->hWnd));
		CalculateWindowSize(rcHtml, rcDlg, SPACE_X, SPACE_Y);
		rcHtml.left = SPACE_X + (rcDlg.right - SPACE_X - SPACE_X - SPACE_X) / 2;
		ResizeWindow(hDlg, GetDlgCtrlID(IexplorerUtils::Inst()->hWnd), &rcHtml, FALSE);
		NotifyUpdateWindowControl(hDlg, GetDlgCtrlID(IexplorerUtils::Inst()->hWnd), SPACE_X, SPACE_Y);
	}
	break;
	default:
	{
		//
	}
	break;
	}

	return FALSE;
}

__inline static
INT_PTR PpsNewUI(HINSTANCE hInstance, HWND hParendWnd = NULL, LPARAM lParam = NULL)
{
    LRESULT lResult;

    DIALOGBOX_STARTUP();

    ////////////////////////////////////////////////////////////////////////////////
    CREATE_DIALOGBOX(
        WS_POPUP | WS_BORDER | WS_SYSMENU | WS_MINIMIZEBOX | DS_MODALFRAME | WS_CAPTION | WS_SIZEBOX | WS_MAXIMIZEBOX | DS_CENTER,
        0L,
		ID_CONTROL_NUM,
        10, 10, 200, 110,
        L"MarkDown工具"
    );
    ////////////////////////////////////////////////////////////////////////////////

	CREATE_EDIT(
		ID_EDIT_USERNAME_TEXT,
		ES_LEFT | WS_BORDER | WS_TABSTOP | WS_CHILD | WS_VISIBLE,
		0L,
		10, 10, 60, 15,
		L"");
	CREATE_EDIT(
		ID_EDIT_MARKDOWN_TEXT,
		ES_LEFT | ES_MULTILINE | ES_WANTRETURN | WS_BORDER | WS_TABSTOP | WS_CHILD | WS_VISIBLE,
		0L,
		70, 10, 60, 15,
		L"");
	CREATE_BUTTON(
		ID_BUTTON_OPEN_URL,
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0L,
		130, 10, 60, 15,
		L"打开MarkDown");
	CREATE_BUTTON(
		ID_BUTTON_AUTOLOGON,
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0L,
		190, 10, 60, 15,
		L"登录MarkDown");
	CREATE_BUTTON(
		ID_BUTTON_ENTER_INBOX,
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0L,
		250, 10, 60, 15,
		L"进入MarkDown");
	CREATE_BUTTON(
		ID_BUTTON_SELECT_MAIL,
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0L,
		310, 10, 60, 15,
		L"选中MarkDown");
	CREATE_BUTTON(
		ID_BUTTON_READ_MAIL,
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0L,
		370, 10, 60, 15,
		L"读取MarkDown");
	CREATE_BUTTON(
		IDCANCEL,
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0L,
		430, 10, 60, 15,
		L"退出窗口");
    CREATE_EDIT(
		ID_EDIT_STATUS_TEXT,
        ES_CENTER | ES_READONLY | WS_BORDER | WS_TABSTOP | WS_CHILD | WS_VISIBLE,
        0L,
        490, 10, 100, 15,
        L"");
    ///////////////////////////////////////////////////////////////////////////////////////

    lResult = DIALOG_RUN(hInstance, hParendWnd, (DLGPROC)DialogProc, (LPARAM)lParam);

    DIALOGBOX_CLEANUP();

    return lResult;
}

typedef enum ChildrenControls {
	IDCC_MINIMUM = WM_USER,
	IDCC_BUTTON_TEST,
	IDCC_BUTTON_TEST2,
	IDCC_BUTTON_TEST3,
	IDCC_MAXIMUM,
};

class cc_item {
public:
	typedef enum ScaleType {
		ST_FIXED=0,
		ST_X_SCALE,
		ST_Y_SCALE,
		ST_XY_SCALE,
	};
public:
	cc_item(ScaleType sType, int w, int h, double xScale, double yScale, WNDPROC fnWndProc):
		sType(sType),w(w),h(h),xScale(xScale),yScale(yScale),fnWndProc(fnWndProc)
	{
	}
public:
	ScaleType sType;
	int w;
	int h;
	int xExclude;//当前行的所有固定项宽度
	int yExclude;//当前列的所有固定项高度
	int xN;//有效行数
	int yN;//有效列数
	double xScale;
	double yScale;
	WNDPROC fnWndProc;
};
std::vector<std::vector<uint16_t>> cc_array;
std::unordered_map<uint16_t, cc_item> cc_umap;

HWND CreatePpsdlg(HWND hParent = NULL, LPVOID lpWndProc = NULL, BOOL bModal = TRUE, BOOL bCenter = TRUE)
{
	HWND hWnd = NULL;
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };

	bModal &= (hParent != NULL);
	if (bModal) { EnableWindow(hParent, FALSE); }

	GetClassInfoEx(GetModuleHandle(NULL), WC_DIALOG, &wcex);
	wcex.lpszClassName = TEXT(__func__);
	wcex.lpfnWndProc = ([](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)->LRESULT
		{
			WNDPROC fnWndProc = NULL;
			switch (uMsg)
			{
			case WM_CREATE:
			{
				HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(128));
				SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
				SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
				SendMessage(hWnd, WM_SETICON, ICON_SMALL2, (LPARAM)hIcon);
				if ((lParam != NULL) && (fnWndProc = *(WNDPROC*)lParam) != NULL)
				{
					fnWndProc(hWnd, uMsg, wParam, lParam);
					SetProp(hWnd, TEXT(__func__), (HANDLE)(fnWndProc));
				}
			}
			break;
			case WM_DESTROY:
			{
				PostMessage(hWnd, WM_QUIT, (WPARAM)0, (LPARAM)0);
			}
			break;
			case WM_PAINT:
			{
				HDC hDC = nullptr;
				PAINTSTRUCT ps = { 0 };
				hDC = BeginPaint(hWnd, &ps);
				if (hDC != nullptr)
				{
					RECT rc = { 0 };
					GetClientRect(hWnd, &rc);
					FillRect(hDC, &rc, (HBRUSH)LTGRAY_BRUSH);
					EndPaint(hWnd, &ps);
				}
			}
			default:
			{
				if ((fnWndProc = (WNDPROC)GetProp(hWnd, TEXT(__func__))) != NULL )
				{
					fnWndProc(hWnd, uMsg, wParam, lParam);
				}
			}
			break;
			}
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		});
	RegisterClassEx(&wcex);

	/////////////////////////////////////////////////
	if ((hWnd = CreateWindowEx(
		WS_EX_DLGMODALFRAME,
		wcex.lpszClassName,
		wcex.lpszClassName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		hParent,
		NULL,
		wcex.hInstance,
		lpWndProc)
		) != NULL) {

		if (bCenter == TRUE)
		{
			if (hParent != NULL)
			{
				CenterWindowInParent(hWnd, hParent);
			}
			else
			{
				CenterWindowInScreen(hWnd);
			}
		}
		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);

		while (1)
		{
			MSG msg = { 0 };
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					break;
				}
				else
				{
					if (bModal && (msg.hwnd != hWnd))
					{
						continue;
					}
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else
			{
				// 完成某些工作的其他行程式 
			}
		}
	}

	if (bModal) { EnableWindow(hParent, TRUE); SetForegroundWindow(hParent); }

	return 0;
}

LRESULT CALLBACK HandleButtonTestWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CreatePpsdlg(hWnd, NULL, TRUE);
	return 0;
}
#define ROW_NUM 3
#define COL_NUM 3
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		for (auto row = 0; row < ROW_NUM; row++)
		{
			cc_array.push_back({});
			for (auto col = 0; col < COL_NUM; col++)
			{
				cc_array[row].push_back(0);
			}
		}
		int nControlId = IDCC_BUTTON_TEST;
		cc_umap.emplace(nControlId, cc_item(cc_item::ST_XY_SCALE, 60, 40, 0.5, 0.5, HandleButtonTestWndProc));
		CreateWindowEx(0,
			WC_BUTTON,
			TEXT("test"),
			BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
			0,
			0,
			cc_umap.at(nControlId).w,
			cc_umap.at(nControlId).h,
			hWnd,
			(HMENU)nControlId,
			NULL,
			NULL);
		cc_array[0][0] = nControlId;

		nControlId = IDCC_BUTTON_TEST2;
		cc_umap.emplace(nControlId, cc_item(cc_item::ST_XY_SCALE, 60, 40, 0.5, 0.5, HandleButtonTestWndProc));
		CreateWindowEx(0,
			WC_BUTTON,
			TEXT("test"),
			BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
			0,
			0,
			cc_umap.at(nControlId).w,
			cc_umap.at(nControlId).h,
			hWnd,
			(HMENU)nControlId,
			NULL,
			NULL);
		cc_array[1][0] = nControlId;

		nControlId = IDCC_BUTTON_TEST3;
		cc_umap.emplace(nControlId, cc_item(cc_item::ST_XY_SCALE, 60, 40, 0.5, 0.5, HandleButtonTestWndProc));
		CreateWindowEx(0,
			WC_BUTTON,
			TEXT("test"),
			BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
			0,
			0,
			cc_umap.at(nControlId).w,
			cc_umap.at(nControlId).h,
			hWnd,
			(HMENU)nControlId,
			NULL,
			NULL);
		cc_array[1][1] = nControlId;

		{
			//行运算
			auto rows = cc_array;
			auto cols = rows.at(0);
			int xN = 0;
			int yN = 0;
			for (auto row = 0; row < rows.size(); row++)
			{
				bool bFound = false;
				int xExclude = 0;
				for (auto col = 0; col < cols.size(); col++)
				{
					auto item = rows.at(row).at(col);
					if (cc_umap.find(item) != cc_umap.end())
					{
						bFound = true;
						if (cc_umap.at(item).sType == cc_item::ST_FIXED)
						{
							xExclude += cc_umap.at(item).w;
						}
					}
				}
				if (bFound > 0)
				{
					yN++;
				}
				//修正
				for (auto col = 0; col < cols.size(); col++)
				{
					auto item = rows.at(row).at(col);
					if (cc_umap.find(item) != cc_umap.end())
					{
						if (cc_umap.at(item).sType != cc_item::ST_FIXED)
						{
							cc_umap.at(item).xExclude = xExclude;
						}
					}
				}
			}
			//列运算
			for (auto col = 0; col < cols.size(); col++)
			{
				bool bFound = false;
				int yExclude = 0;
				for (auto row = 0; row < rows.size(); row++)
				{
					auto item = rows.at(row).at(col);				
					if (cc_umap.find(item) != cc_umap.end())
					{
						bFound = true;
						if (cc_umap.at(item).sType == cc_item::ST_FIXED)
						{
							yExclude += cc_umap.at(item).h;
						}
					}
				}
				if (bFound > 0)
				{
					xN ++;
				}
				//修正
				for (auto row = 0; row < rows.size(); row++)
				{
					auto item = rows.at(row).at(col);
					if (cc_umap.find(item) != cc_umap.end())
					{
						bFound = true;
						if (cc_umap.at(item).sType != cc_item::ST_FIXED)
						{
							cc_umap.at(item).yExclude = yExclude;
						}
					}
				}
			}
			//优化数值
			for (auto col = 0; col < cols.size(); col++)
			{
				for (auto row = 0; row < rows.size(); row++)
				{
					auto item = rows.at(row).at(col);
					if (cc_umap.find(item) != cc_umap.end())
					{
						cc_umap.at(item).xN = xN;
						cc_umap.at(item).yN = yN;
					}
				}
			}
		}
	}
		return 0;
	case WM_COMMAND:
	{
		if (cc_umap.find(LOWORD(wParam)) != cc_umap.end()) {
			cc_umap.at(LOWORD(wParam)).fnWndProc(hWnd, uMsg, wParam, lParam);
		}
	}
	break;
	case WM_SIZE:
	{
		RECT rcWnd = { 0 };
		GetClientRect(hWnd, &rcWnd);

		int left = 0;
		int top = 0;
		auto rows = cc_array;
		for (auto row = 0; row < rows.size(); row++)
		{
			auto cols = rows.at(row);
			uint16_t uItem = 0;
			left = 0;
			for (auto col = 0; col < cols.size(); col++)
			{
				auto item = cols.at(col);
				//存在控件
				if (item > 0)
				{
					cc_item* p_cc_item = &cc_umap.at(item);
					if (p_cc_item->sType == cc_item::ST_FIXED)
					{
						MoveWindow(GetDlgItem(hWnd, item), left, top, p_cc_item->w, p_cc_item->h, TRUE);
						left += p_cc_item->w;
						uItem = p_cc_item->h;
					}
					else if (p_cc_item->sType == cc_item::ST_X_SCALE)
					{
						p_cc_item->w = (rcWnd.right - rcWnd.left - p_cc_item->xExclude) * p_cc_item->xScale;
						MoveWindow(GetDlgItem(hWnd, item), left, top, p_cc_item->w, p_cc_item->h, TRUE);
						left += p_cc_item->w;
						uItem = p_cc_item->h;
					}
					else if (p_cc_item->sType == cc_item::ST_Y_SCALE)
					{
						p_cc_item->h = (rcWnd.bottom - rcWnd.top - p_cc_item->yExclude) * p_cc_item->yScale;
						MoveWindow(GetDlgItem(hWnd, item), left, top, p_cc_item->w, p_cc_item->h, TRUE);
						left += p_cc_item->w;
						uItem = p_cc_item->h;
					}
					else if (p_cc_item->sType == cc_item::ST_XY_SCALE)
					{
						p_cc_item->w = (rcWnd.right - rcWnd.left - p_cc_item->xExclude) * p_cc_item->xScale;
						p_cc_item->h = (rcWnd.bottom - rcWnd.top - p_cc_item->yExclude) * p_cc_item->yScale;
						MoveWindow(GetDlgItem(hWnd, item), left, top, p_cc_item->w, p_cc_item->h, TRUE);
						left += p_cc_item->w;
						uItem = p_cc_item->h;
					}
				}
			}
			top += uItem;
		}
		/*int row = 0;
		int col = 0;
		int xLeft = 0;
		int yLeft = 0;
		int xTop = 0;
		int yTop = 0;

		row = 0;
		col = 0;
		if (cc_array[row][col] > 0)
		{//存在控件
			cc_item* p_cc_item = &cc_umap.at(cc_array[row][col]);
			if (p_cc_item->sType == cc_item::ST_FIXED)
			{
				MoveWindow(GetDlgItem(hWnd, cc_array[row][col]), xLeft, xTop, p_cc_item->w, p_cc_item->h, TRUE);
				xLeft += p_cc_item->w;
				yTop += p_cc_item->h;
			}
			else
			{
				//查找两外两行是否存在控件以确定高度
			}
		}
		row = 0;
		col = 1;
		if (cc_array[row][col] > 0)
		{//存在控件
			cc_item* p_cc_item = &cc_umap.at(cc_array[row][col]);
			if (p_cc_item->sType == cc_item::ST_FIXED)
			{
				MoveWindow(GetDlgItem(hWnd, cc_array[row][col]), xLeft, xTop, p_cc_item->w, p_cc_item->h, TRUE);
				xLeft += p_cc_item->w;
				xTop += p_cc_item->h;
			}
			else
			{
				//查找两外两行是否存在控件以确定高度
			}
		}
		row = 1;
		col = 0;
		if (cc_array[row][col] > 0)
		{//存在控件
			cc_item* p_cc_item = &cc_umap.at(cc_array[row][col]);
			if (p_cc_item->sType == cc_item::ST_FIXED)
			{
				MoveWindow(GetDlgItem(hWnd, cc_array[row][col]), yLeft, yTop, p_cc_item->w, p_cc_item->h, TRUE);
				yLeft += p_cc_item->w;
			}
			else
			{
				//查找两外两行是否存在控件以确定高度
			}
		}
		row = 1;
		col = 1;
		if (cc_array[row][col] > 0)
		{//存在控件
			cc_item* p_cc_item = &cc_umap.at(cc_array[row][col]);
			if (p_cc_item->sType == cc_item::ST_FIXED)
			{
				MoveWindow(GetDlgItem(hWnd, cc_array[row][col]), yLeft, yTop, p_cc_item->w, p_cc_item->h, TRUE);
				yLeft += p_cc_item->w;
			}
			else
			{
				//查找两外两行是否存在控件以确定高度
			}
		}*/
	}
	break;
	case WM_PAINT:
	{
		HDC hDC = nullptr;
		PAINTSTRUCT ps = { 0 };
		hDC = BeginPaint(hWnd, &ps);
		if (hDC != nullptr)
		{
			RECT rc = { 0 };
			GetClientRect(hWnd, &rc);
			FillRect(hDC, &rc, (HBRUSH)LTGRAY_BRUSH);
			TextOut(hDC, 0, 0, TEXT("Hello world!"), lstrlen(TEXT("Hello world!")));
			TextOut(hDC, 32, 32, TEXT("Win32 Api!"), lstrlen(TEXT("Win32 Api!")));
			EndPaint(hWnd, &ps);
		}
	}
		break;
	}
	return 0;
}

__inline static
DWORD WINAPI ThreadStartRoutine(LPVOID lpThreadParameter)
{
    UNREFERENCED_PARAMETER(lpThreadParameter);
	//PpsNewUI(GetModuleHandle(NULL));
	CreatePpsdlg(NULL, WndProc);
	return 0;
}