// ResizedTable.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "ResizedTable.h"
#include "commctrl.h"
#include <string>
#define MoveToEx(hdc, x, y) MoveToEx(hdc, x, y, NULL)
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
// Метрики текста.
TEXTMETRIC tm; 
// Средняя ширина и полная высота символов.
int cxChar, cyChar;
// Ширина и высота рабочей области.
int cxClient, cyClient;
// Количество строк таблицы.
const int gridRows = 3;
// Количество колонок таблицы.
const int gridColumns = 5;

// Ординаты каждой строки таблицы.
int gridRowsPositionY[gridRows];
// Содержимое ячеек таблицы.
wchar_t gridCellContent[gridRows * gridColumns][200];
// Размер содержимого ячеек таблицы.
int gridCellContentSize[gridRows * gridColumns];



// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void                PaintGrid(HWND);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_RESIZEDTABLE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RESIZEDTABLE));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RESIZEDTABLE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_RESIZEDTABLE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   //InitCommonControls();

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

// Инициализация глобальных переменных.
// cxClient, cyClient, cxChar, cyChar.
void InitGlobalVars(HWND hWnd)
{
    HDC hdc = GetDC(hWnd);
    SetTextColor(hdc, RGB(255, 100, 66));
    // Получение ширины и высоты окна клиента.
    cxClient = GetDeviceCaps(hdc, HORZRES);
    cyClient = GetDeviceCaps(hdc, VERTRES);
    GetTextMetrics(hdc, &tm);
    cxChar = tm.tmAveCharWidth;
    cyChar = tm.tmHeight + tm.tmExternalLeading;
    for (int i = 0; i < gridRows; i++)
    {
        gridRowsPositionY[i] = 0;
    }
    // Инициализация содержимого ячеек.
    for (int cell = 0; cell < (gridRows * gridColumns); cell++)
    {
        gridCellContentSize[cell] = wsprintf(gridCellContent[cell],
            L"Немного статического контента. При превышении ширины ячейки, строка переносится.");  //  При превышении ширины ячейки, строка переносится.
    }
    gridCellContentSize[0] = wsprintf(gridCellContent[0],
        L"Вот это первая строчка. Содержимое отлично от остальныхххххх хххххххххххх керкеркерокеркерокер керкеркеркеркеркеркерк керкеркеркеркеркеркер!");
    SaveDC(hdc);
    ReleaseDC(hWnd, hdc);
    return;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        {
            InitGlobalVars(hWnd);
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_SIZE:
        {
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
        }
        break;
    case WM_PAINT:
        {
            PaintGrid(hWnd);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void PaintGridCells(HDC hdc)
{
    // Прорисовка периметра таблицы.
    LineTo(hdc, 0, cyClient);
    MoveToEx(hdc, 0, cyClient-1);
    LineTo(hdc, cxClient, cyClient-1);
    MoveToEx(hdc, cxClient-1, cyClient-1);
    LineTo(hdc, cxClient-1, 0);
    MoveToEx(hdc, cxClient, 0);
    LineTo(hdc, 0, 0);

    // Прорисовка колонок таблицы.
    int gridCellWidth = cxClient / gridColumns;
    for (int i = 1; i <= gridColumns; i++)
    {
        int x = gridCellWidth * i;
        MoveToEx(hdc, x, 0);
        LineTo(hdc, x, cyClient);
    }

    int gridCellHeight = cyClient / gridRows;
    int cell = 0;
    for (int row = 0; row < gridRows; row++)
    {
        for (int column = 0; column < gridColumns; column++)
        {
            // Вычисление высоты ячейки.
            int strcount = ((gridCellContentSize[cell] * cxChar) / gridCellWidth) + 2;
            int cellactheight = (strcount * cyChar) + cyChar;
            if (gridRowsPositionY[row] < cellactheight)
            {
                if ((row - 1) < 0)
                {
                    gridRowsPositionY[row] = cellactheight;
                }
                else
                {
                    gridRowsPositionY[row] = cellactheight + gridRowsPositionY[row - 1];
                }
            }
            int topY;
            if ((row - 1) < 0)
                topY = 0;
            else
                topY = gridRowsPositionY[row - 1];

            RECT rect;
            rect.top = topY + cyChar;
            rect.left = (gridCellWidth * column) + cxChar; 
            rect.right = (gridCellWidth * column) + gridCellWidth - cxChar; 
            rect.bottom = topY + cellactheight; 
            DrawText(hdc, gridCellContent[cell], gridCellContentSize[cell], &rect, DT_WORDBREAK);
            cell++;
        }
    }

    // Прорисовка строк таблицы.   
    for (int i = 0; i < gridRows; i++)
    {
        int y = gridRowsPositionY[i];
        MoveToEx(hdc, 0, y);
        LineTo(hdc, cxClient, y);
        gridRowsPositionY[i] = 0;
    }
    return;
}

// Прорисовка таблицы.
void PaintGrid(HWND hwnd)
{
    HDC hdc = GetDC(hwnd);
    PaintGridCells(hdc);
    ReleaseDC(hwnd, hdc);
    return;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
