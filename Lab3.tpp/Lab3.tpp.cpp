#include "framework.h"
#include "Lab3.tpp.h" // Згенерований заголовочний файл проєкту
#include <cmath>
#include "Resource.h"

// Глобальні змінні стану для графіки
bool g_showScene = true;
bool g_isBlueCar = false;
double g_scale = 1.0;

// Класи графічних об'єктів
class Sun {
public:
    void show(HDC dc, int X, int Y) {
        HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 165, 0));
        HGDIOBJ oldPen = SelectObject(dc, pen);
        for (int i = 0; i < 8; ++i) {
            MoveToEx(dc, X, Y, nullptr);
            int dx = static_cast<int>((40 * g_scale) * cos(i * 0.785));
            int dy = static_cast<int>((40 * g_scale) * sin(i * 0.785));
            LineTo(dc, X + dx, Y + dy);
        }
        SelectObject(dc, oldPen);
        DeleteObject(pen);

        HBRUSH brush = CreateSolidBrush(RGB(255, 215, 0));
        HGDIOBJ oldBrush = SelectObject(dc, brush);
        int r = static_cast<int>(25 * g_scale);
        Ellipse(dc, X - r, Y - r, X + r, Y + r);
        SelectObject(dc, oldBrush);
        DeleteObject(brush);
    }
};

class Cloud {
public:
    void show(HDC dc, int X, int Y) {
        HBRUSH brush = CreateSolidBrush(RGB(240, 248, 255));
        HPEN pen = CreatePen(PS_SOLID, 1, RGB(176, 196, 222));
        HGDIOBJ oldBrush = SelectObject(dc, brush);
        HGDIOBJ oldPen = SelectObject(dc, pen);

        int w = static_cast<int>(50 * g_scale);
        int h = static_cast<int>(35 * g_scale);
        Ellipse(dc, X, Y, X + w, Y + h);
        Ellipse(dc, X + static_cast<int>(20 * g_scale), Y - static_cast<int>(15 * g_scale), X + static_cast<int>(70 * g_scale), Y + static_cast<int>(30 * g_scale));
        Ellipse(dc, X + static_cast<int>(45 * g_scale), Y, X + static_cast<int>(95 * g_scale), Y + h);

        SelectObject(dc, oldBrush);
        SelectObject(dc, oldPen);
        DeleteObject(brush);
        DeleteObject(pen);
    }
};

class FirTree {
public:
    void show(HDC dc, int X, int Y) {
        HBRUSH trunkBrush = CreateSolidBrush(RGB(139, 69, 19));
        HGDIOBJ oldBrush = SelectObject(dc, trunkBrush);
        int trunkW = static_cast<int>(6 * g_scale);
        int trunkH = static_cast<int>(25 * g_scale);
        Rectangle(dc, X - trunkW, Y, X + trunkW, Y + trunkH);
        SelectObject(dc, oldBrush);
        DeleteObject(trunkBrush);

        HBRUSH foliageBrush = CreateSolidBrush(RGB(34, 139, 34));
        oldBrush = SelectObject(dc, foliageBrush);
        POINT tier1[3] = { { X, Y - static_cast<int>(60 * g_scale) }, { X - static_cast<int>(25 * g_scale), Y - static_cast<int>(30 * g_scale) }, { X + static_cast<int>(25 * g_scale), Y - static_cast<int>(30 * g_scale) } };
        Polygon(dc, tier1, 3);
        POINT tier2[3] = { { X, Y - static_cast<int>(45 * g_scale) }, { X - static_cast<int>(30 * g_scale), Y - static_cast<int>(15 * g_scale) }, { X + static_cast<int>(30 * g_scale), Y - static_cast<int>(15 * g_scale) } };
        Polygon(dc, tier2, 3);
        POINT tier3[3] = { { X, Y - static_cast<int>(25 * g_scale) }, { X - static_cast<int>(35 * g_scale), Y + static_cast<int>(5 * g_scale) }, { X + static_cast<int>(35 * g_scale), Y + static_cast<int>(5 * g_scale) } };
        Polygon(dc, tier3, 3);

        SelectObject(dc, oldBrush);
        DeleteObject(foliageBrush);
    }
};

class Car {
public:
    void show(HDC dc, int X, int Y) {
        COLORREF carColor = g_isBlueCar ? RGB(30, 144, 255) : RGB(220, 20, 60);
        HBRUSH bodyBrush = CreateSolidBrush(carColor);
        HGDIOBJ oldBrush = SelectObject(dc, bodyBrush);

        Rectangle(dc, X, Y - static_cast<int>(25 * g_scale), X + static_cast<int>(110 * g_scale), Y);
        Rectangle(dc, X + static_cast<int>(30 * g_scale), Y - static_cast<int>(45 * g_scale), X + static_cast<int>(80 * g_scale), Y - static_cast<int>(25 * g_scale));
        SelectObject(dc, oldBrush);
        DeleteObject(bodyBrush);

        HBRUSH windowBrush = CreateSolidBrush(RGB(173, 216, 230));
        oldBrush = SelectObject(dc, windowBrush);
        Rectangle(dc, X + static_cast<int>(35 * g_scale), Y - static_cast<int>(40 * g_scale), X + static_cast<int>(52 * g_scale), Y - static_cast<int>(28 * g_scale));
        Rectangle(dc, X + static_cast<int>(57 * g_scale), Y - static_cast<int>(40 * g_scale), X + static_cast<int>(75 * g_scale), Y - static_cast<int>(28 * g_scale));
        SelectObject(dc, oldBrush);
        DeleteObject(windowBrush);

        HBRUSH wheelBrush = CreateSolidBrush(RGB(40, 40, 40));
        oldBrush = SelectObject(dc, wheelBrush);
        int r = static_cast<int>(10 * g_scale);
        Ellipse(dc, X + static_cast<int>(25 * g_scale) - r, Y - r, X + static_cast<int>(25 * g_scale) + r, Y + r);
        Ellipse(dc, X + static_cast<int>(85 * g_scale) - r, Y - r, X + static_cast<int>(85 * g_scale) + r, Y + r);
        SelectObject(dc, oldBrush);
        DeleteObject(wheelBrush);
    }
};

void DrawScene(HDC dc, int width, int height) {
    if (!g_showScene) return;

    HBRUSH brush = CreateSolidBrush(RGB(135, 206, 235));
    HGDIOBJ oldBrush = SelectObject(dc, brush);
    Rectangle(dc, 0, 0, width, height - 120);
    SelectObject(dc, oldBrush);
    DeleteObject(brush);

    brush = CreateSolidBrush(RGB(60, 179, 113));
    oldBrush = SelectObject(dc, brush);
    Rectangle(dc, 0, height - 120, width, height);
    SelectObject(dc, oldBrush);
    DeleteObject(brush);

    brush = CreateSolidBrush(RGB(90, 90, 90));
    oldBrush = SelectObject(dc, brush);
    Rectangle(dc, 0, height - 80, width, height - 20);
    SelectObject(dc, oldBrush);
    DeleteObject(brush);

    HPEN pen = CreatePen(PS_DASH, 1, RGB(255, 255, 255));
    HGDIOBJ oldPen = SelectObject(dc, pen);
    MoveToEx(dc, 0, height - 50, nullptr);
    LineTo(dc, width, height - 50);
    SelectObject(dc, oldPen);
    DeleteObject(pen);

    Sun sun; sun.show(dc, width - 120, 80);
    Cloud cloud; cloud.show(dc, 80, 50); cloud.show(dc, 320, 80); cloud.show(dc, 550, 40);
    FirTree firTree; firTree.show(dc, 90, height - 130); firTree.show(dc, 220, height - 140); firTree.show(dc, 680, height - 130);
    Car car; car.show(dc, 150, height - 40); car.show(dc, 450, height - 40);
}

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB3TPP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB3TPP));

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

    return (int)msg.wParam;
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

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB3TPP));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB3TPP);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

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

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Обробка вибору пунктів меню:
        switch (wmId)
        {
        case ID_IMAGE_DRAW:         // "Рисунок"
            g_showScene = !g_showScene;
            InvalidateRect(hWnd, nullptr, TRUE);
            break;

        case ID_TRANSFORM_COLOR:    // "Зміна кольору"
            g_isBlueCar = !g_isBlueCar;
            InvalidateRect(hWnd, nullptr, TRUE);
            break;

        case ID_TRANSFORM_ZOOM:     // "Збільшення"
            if (g_scale >= 1.4)
                g_scale = 1.0;
            else
                g_scale += 0.2;
            InvalidateRect(hWnd, nullptr, TRUE);
            break;

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

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // Отримуємо розміри вікна
        RECT rect;
        GetClientRect(hWnd, &rect);
        int width = rect.right;
        int height = rect.bottom;

        // Викликаємо функцію малювання сцени
        DrawScene(hdc, width, height);

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_SIZE:
        // Перемальовуємо вікно при зміні його розміру
        InvalidateRect(hWnd, nullptr, TRUE);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обробник повідомлень для вікна "Про програму" (About)
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