#include <windows.h>
#include <commctrl.h>
#include <fstream>
#include <vector>
#include <string>

#pragma comment(lib, "comctl32.lib")

#define IDC_SCRAPE_BUTTON 101
#define IDC_LISTVIEW      102

struct Product {
    std::string name;
    std::string price;
    std::string rating;
};

// Dummy product data
std::vector<Product> products = {
    {"Laptop", "50000", "4.5"},
    {"Phone", "20000", "4.2"},
    {"Headphones", "1500", "4.0"}
};

void saveDummyData() {
    std::ofstream csv("products.csv");
    csv << "Name,Price,Rating\n";
    for (auto &p : products)
        csv << p.name << "," << p.price << "," << p.rating << "\n";
    csv.close();
}

void addColumns(HWND hList) {
    LVCOLUMN lvCol = {};
    lvCol.mask = LVCF_TEXT | LVCF_WIDTH;

    lvCol.cx = 100; lvCol.pszText = const_cast<char*>("Name");
    ListView_InsertColumn(hList, 0, &lvCol);

    lvCol.cx = 80; lvCol.pszText = const_cast<char*>("Price");
    ListView_InsertColumn(hList, 1, &lvCol);

    lvCol.cx = 60; lvCol.pszText = const_cast<char*>("Rating");
    ListView_InsertColumn(hList, 2, &lvCol);
}


void addItems(HWND hList) {
    LVITEM lvItem = {};
    lvItem.mask = LVIF_TEXT;

    for (int i = 0; i < products.size(); i++) {
        lvItem.iItem = i;
        lvItem.iSubItem = 0;
        lvItem.pszText = const_cast<char*>(products[i].name.c_str());
        ListView_InsertItem(hList, &lvItem);

        ListView_SetItemText(hList, i, 1, const_cast<char*>(products[i].price.c_str()));
        ListView_SetItemText(hList, i, 2, const_cast<char*>(products[i].rating.c_str()));
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hList;

    switch (uMsg) {
    case WM_CREATE:
        InitCommonControls();

        // Create ListView
        hList = CreateWindow(WC_LISTVIEW, "",
            WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL,
            20, 20, 260, 100, hwnd, (HMENU)IDC_LISTVIEW,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

        addColumns(hList);
        addItems(hList);

        // Create button
        CreateWindow("BUTTON", "Generate CSV",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            80, 130, 120, 30,
            hwnd, (HMENU)IDC_SCRAPE_BUTTON,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_SCRAPE_BUTTON) {
            saveDummyData();
            MessageBox(hwnd, "Product data saved to products.csv", "Done", MB_OK);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    const char CLASS_NAME[] = "BasicWin32App";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "Product Viewer",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 320, 220,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
