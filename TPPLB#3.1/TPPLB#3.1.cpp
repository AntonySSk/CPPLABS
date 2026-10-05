#include <Windows.h>
#include <string>

// Ідентифікатори елементів керування
#define ID_EDIT_X 101
#define ID_EDIT_Y 102
#define ID_BUTTON 103
#define ID_STATIC_RESULT 104

// Функція обробки повідомлень вікна
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

// Головна функція Windows-застосунку
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Опис класу вікна (використовуємо WNDCLASSEXW для Unicode)
    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.lpszClassName = L"MyWindowClass";

    // Реєстрація класу вікна
    RegisterClassExW(&wc);

    // Створення головного вікна
    HWND hwnd = CreateWindowExW(
        0,
        L"MyWindowClass",
        L"Обчислення функції f(x, y) = 2y - 4x",
        WS_OVERLAPPEDWINDOW,
        500, 300,
        500, 350,
        nullptr, nullptr, hInstance, nullptr
    );

    if (hwnd == nullptr)
        return 0;

    // Відображення вікна
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Цикл обробки повідомлень
    MSG msg{};
    while (GetMessageW(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return static_cast<int>(msg.wParam);
}

// Функція обробки повідомлень
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Дескриптори елементів керування
    static HWND hEditX;
    static HWND hEditY;
    static HWND hButton;
    static HWND hResult;

    switch (message)
    {
        // Створення дочірніх елементів
    case WM_CREATE:
    {
        HINSTANCE hInstance = reinterpret_cast<LPCREATESTRUCT>(lParam)->hInstance;

        // Поле введення X
        hEditX = CreateWindowExW(
            0,
            L"EDIT",
            L"0",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
            50, 70,
            100, 25,
            hwnd,
            reinterpret_cast<HMENU>(ID_EDIT_X),
            hInstance,
            nullptr
        );

        // Поле введення Y
        hEditY = CreateWindowExW(
            0,
            L"EDIT",
            L"0",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
            180, 70,
            100, 25,
            hwnd,
            reinterpret_cast<HMENU>(ID_EDIT_Y),
            hInstance,
            nullptr
        );

        // Кнопка
        hButton = CreateWindowExW(
            0,
            L"BUTTON",
            L"Розрахувати",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            50, 120,
            120, 30,
            hwnd,
            reinterpret_cast<HMENU>(ID_BUTTON),
            hInstance,
            nullptr
        );

        // Статичне поле для результату
        hResult = CreateWindowExW(
            0,
            L"STATIC",
            L"0",
            WS_CHILD | WS_VISIBLE,
            180, 170,
            200, 25,
            hwnd,
            reinterpret_cast<HMENU>(ID_STATIC_RESULT),
            hInstance,
            nullptr
        );
        break;
    }

    // Обробка команд від елементів керування
    case WM_COMMAND:
    {
        if (LOWORD(wParam) == ID_BUTTON)
        {
            wchar_t bufferX[50]{};
            wchar_t bufferY[50]{};

            // Отримання тексту з полів введення (Unicode)
            GetWindowTextW(hEditX, bufferX, 50);
            GetWindowTextW(hEditY, bufferY, 50);

            try
            {
                // Перетворення широкого рядка у число
                double x = std::stod(bufferX);
                double y = std::stod(bufferY);

                // Розрахунок функції f(x, y) = 2y - 4x
                double result = 2.0 * y - 4.0 * x;

                // Перетворення результату у широкий рядок
                std::wstring resultStr = std::to_wstring(result);

                // Виведення результату у статичне поле
                SetWindowTextW(hResult, resultStr.c_str());
            }
            catch (...)
            {
                // Повідомлення про некоректне введення
                SetWindowTextW(hResult, L"Помилка введення");
            }
        }
        break;
    }

    // Перемальовування головного вікна
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Малювання тексту з Unicode
        const wchar_t* titleText = L"Обчислення функції f(x, y) = 2y - 4x";
        const wchar_t* labelX = L"x:";
        const wchar_t* labelY = L"y:";
        const wchar_t* resultLabel = L"Результат f(x,y):";

        TextOutW(hdc, 50, 20, titleText, static_cast<int>(wcslen(titleText)));
        TextOutW(hdc, 50, 48, labelX, static_cast<int>(wcslen(labelX)));
        TextOutW(hdc, 180, 48, labelY, static_cast<int>(wcslen(labelY)));
        TextOutW(hdc, 50, 170, resultLabel, static_cast<int>(wcslen(resultLabel)));

        EndPaint(hwnd, &ps);
        break;
    }

    // Закриття вікна
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProcW(hwnd, message, wParam, lParam);
    }
    return 0;
}