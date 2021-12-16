#include "framework.h"
#include "laba_2.h"
#include<string>

#define MAX_LOADSTRING 100


// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);



void drawCircle(HDC dc, int x, int y, int r)
{
    Ellipse(dc, x - r, y - r, x + r, y + r);
}

bool pointInRect(POINT p, int x, int y, int w, int h)
{
    return (p.x >= x) && (p.y >= y) && (p.x <= x + w) && (p.y <= y + h);
}

void drawFrame(HDC dc, int x, int y, int w, int h)
{
    POINT p;
    MoveToEx(dc, x, y, &p);
    LineTo(dc, x + w, y);
    LineTo(dc, x + w, y + h);
    LineTo(dc, x, y + h);
    LineTo(dc, x, y);
}

class Program
{
public:
    Program()
    {
        ismove = false;
        oldScale = 1;
        newScale = 1;
        scale = 1;
        ticks = 0;
        brR = 0;
        brG = 0;
        brB = 255;

        pnR = 127;
        pnG = 127;
        pnB = 127;
        wndScale = 1;
    }

    void draw(HDC dc)
    {
        if (scale < 0.01)
            scale = 0.01;
        else if (scale > 5)
            scale = 5;

        br = CreateSolidBrush(RGB(255, 255, 255));

        FillRect(dc, &clientRct, br);

        br = CreateSolidBrush(RGB(brR, brG, brB));
        pn = CreatePen(0, 4.f * scale * wndScale, RGB(pnR, pnG, pnB));
        SelectObject(dc, br);
        SelectObject(dc, pn);

        POINT p;
        MoveToEx(dc, (450 + offset.x) * scale * wndScale, (150 + offset.y) * scale * wndScale, &p);
        LineTo(dc, (300 + offset.x) * scale * wndScale, (400 + offset.y) * scale * wndScale);
        LineTo(dc, (600 + offset.x) * scale * wndScale, (400 + offset.y) * scale * wndScale);
        LineTo(dc, (450 + offset.x) * scale * wndScale, (150 + offset.y) * scale * wndScale);

        drawCircle(dc, (450 + offset.x) * scale * wndScale, (150 + offset.y) * scale * wndScale, 80 * scale * wndScale);
        drawCircle(dc, (300 + offset.x) * scale * wndScale, (400 + offset.y) * scale * wndScale, 80 * scale * wndScale);
        drawCircle(dc, (600 + offset.x) * scale * wndScale, (400 + offset.y) * scale * wndScale, 80 * scale * wndScale);

        if (fmove)
            SetCursor(LoadCursor(0, IDC_HAND));
        else
            SetCursor(LoadCursor(0, IDC_ARROW));
    }

    void setBrushColor(int r, int g, int b)
    {
        brR = r;
        brG = g;
        brB = b;
    }

    void setPenColor(int r, int g, int b)
    {
        pnR = r;
        pnG = g;
        pnB = b;
    }

    void wndResize(float x, float y)
    {
        float scaleX = x / firstClientRct.right;
        float scaleY = y / firstClientRct.bottom;

        wndScale = scaleX < scaleY ? scaleX : scaleY;

    }


    bool fmove;
    HBRUSH br;
    HPEN pn;
    RECT clientRct;
    RECT firstClientRct;
    POINT offset;
    POINT startoffset;
    POINT endoffset;
    bool ismove;
    float scale;
    float oldScale;
    float newScale;
    int ticks;
    float wndScale;
private:
    int brR, brG, brB;
    int pnR, pnG, pnB;
}myProg;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LABA2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LABA2));
    
    MSG msg;

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

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_LABA2));
    wcex.hCursor = 0;// LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LABA2);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 900, 600, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);

        switch (wmId)
        {
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case ID_32775:
            SetTimer(hWnd, 1, 10, NULL);
            myProg.ticks = 0;
            myProg.oldScale = myProg.scale;
            myProg.newScale = 1;
            break;
        case ID_32777:
            SetTimer(hWnd, 1, 10, NULL);
            myProg.ticks = 0;
            myProg.oldScale = myProg.scale;
            myProg.newScale = 0.5;
            break;
        case ID_32776:
            SetTimer(hWnd, 1, 10, NULL);
            myProg.ticks = 0;
            myProg.oldScale = myProg.scale;
            myProg.newScale = 0.25;
            break;
        case ID_32782:
            myProg.setBrushColor(127, 127, 127);
            break;
        case ID_32783:
            myProg.setBrushColor(255, 255, 0);
            break;
        case ID_32784:
            myProg.setBrushColor(0, 0, 255);
            break;
        case ID_32788:
            myProg.setPenColor(127, 127, 127);
            break;
        case ID_32786:
            myProg.setPenColor(255, 255, 0);
            break;
        case ID_32789:
            myProg.setPenColor(0, 0, 255);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        InvalidateRect(hWnd, &myProg.clientRct, false);
    }
    break;

    case WM_SIZE:
    {
        GetClientRect(hWnd, &myProg.clientRct);
        myProg.wndResize(LOWORD(lParam), HIWORD(lParam));
    }
    break;

    case WM_TIMER:
    {
        myProg.ticks++;
        if (myProg.ticks > 50)
        {
            myProg.scale = myProg.newScale;
            KillTimer(hWnd, 1);
        }
        else
        {
            myProg.scale = myProg.oldScale - (myProg.oldScale - myProg.newScale) / 50.f * (float)myProg.ticks;
        }
        InvalidateRect(hWnd, &myProg.clientRct, false);
    }
    break;
    case WM_MOUSEMOVE:
    {

        POINT t;
        GetCursorPos(&t);
        ScreenToClient(hWnd, &t);

        myProg.fmove = pointInRect(t, (250 + myProg.offset.x) * myProg.scale * myProg.wndScale, (150 + myProg.offset.y) * myProg.scale * myProg.wndScale, 400 * myProg.scale * myProg.wndScale, 300 * myProg.scale * myProg.wndScale);

        if (myProg.ismove && myProg.fmove)
        {
            POINT temp;
            GetCursorPos(&temp);
            ScreenToClient(hWnd, &temp);
            myProg.offset.x = myProg.endoffset.x - (myProg.startoffset.x - temp.x) / myProg.scale / myProg.wndScale;
            myProg.offset.y = myProg.endoffset.y - (myProg.startoffset.y - temp.y) / myProg.scale / myProg.wndScale;
        }


        InvalidateRect(hWnd, &myProg.clientRct, false);
    }
    break;

    case WM_LBUTTONDOWN:
    {
        myProg.ismove = true;
        GetCursorPos(&myProg.startoffset);
        ScreenToClient(hWnd, &myProg.startoffset);
    }
    break;

    case WM_LBUTTONUP:
    {
        myProg.ismove = false;
        myProg.endoffset = myProg.offset;
        InvalidateRect(hWnd, &myProg.clientRct, false);
    }
    break;

    case WM_NCLBUTTONUP:
    {
        myProg.ismove = false;
        myProg.endoffset = myProg.offset;
        InvalidateRect(hWnd, &myProg.clientRct, false);
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        HDC mdc = CreateCompatibleDC(hdc);
        HBITMAP memBm = CreateCompatibleBitmap(hdc, myProg.clientRct.right - myProg.clientRct.left, myProg.clientRct.bottom - myProg.clientRct.top);
        SelectObject(mdc, memBm);

        myProg.draw(mdc);

        BitBlt(hdc, 0, 0, myProg.clientRct.right - myProg.clientRct.left, myProg.clientRct.bottom - myProg.clientRct.top, mdc, 0, 0, SRCCOPY);
        DeleteDC(mdc);
        DeleteObject(memBm);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_CREATE:
        GetClientRect(hWnd, &myProg.firstClientRct);
        break;
    case WM_DESTROY:
    {
        PostQuitMessage(0);
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}



