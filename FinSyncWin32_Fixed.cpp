#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <commctrl.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <windowsx.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "gdi32.lib")

struct Transaction {
    std::wstring type;
    double amount;
    std::wstring category;
    std::wstring date;

    Transaction(std::wstring t, double a, std::wstring c, std::wstring d)
        : type(t), amount(a), category(c), date(d) {}
};

// Dialog data structure
struct DialogData {
    double amount;
    std::wstring date;
    std::wstring category;
    bool accepted;
};

class FinSyncApp {
private:
    HWND hwndMain;
    HWND hwndListView;
    HWND hwndIncomeLabel;
    HWND hwndExpenseLabel;
    HWND hwndSavingsLabel;
    HWND hwndStatusBar;
    
    std::vector<Transaction> transactions;
    const std::vector<std::wstring> categories = {
        L"Food", L"Rent", L"Entertainment", L"Transportation", L"Utilities", L"Other"
    };

    static FinSyncApp* instance;
    static DialogData dialogData;

public:
    FinSyncApp() : hwndMain(nullptr), hwndListView(nullptr) {
        instance = this;
    }

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK IncomeDialogProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK ExpenseDialogProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK EditDialogProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam);
    
    void CreateMainWindow(HINSTANCE hInstance);
    void AddIncome();
    void AddExpense();
    void EditTransaction();
    void DeleteTransaction();
    void GenerateReport();
    void RefreshListView();
    void UpdateSummary();
    void SaveData();
    void LoadData();
    std::wstring GetCurrentDate();
};

FinSyncApp* FinSyncApp::instance = nullptr;
DialogData FinSyncApp::dialogData;

// Control IDs
#define ID_BTN_ADD_INCOME 1001
#define ID_BTN_ADD_EXPENSE 1002
#define ID_BTN_EDIT 1003
#define ID_BTN_DELETE 1004
#define ID_BTN_REPORT 1005
#define ID_BTN_SAVE 1006
#define ID_LISTVIEW 1007
#define ID_EDIT_AMOUNT 2001
#define ID_EDIT_DATE 2002
#define ID_COMBO_CATEGORY 2003

std::wstring FinSyncApp::GetCurrentDate() {
    SYSTEMTIME st;
    GetLocalTime(&st);
    wchar_t buffer[32];
    swprintf_s(buffer, L"%02d/%02d/%04d", st.wDay, st.wMonth, st.wYear);
    return buffer;
}

LRESULT CALLBACK FinSyncApp::IncomeDialogProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == IDOK)) {
                wchar_t buffer[256];
                wchar_t dateBuffer[256];
                GetWindowText(GetDlgItem(hwndDlg, ID_EDIT_AMOUNT), buffer, 256);
                GetWindowText(GetDlgItem(hwndDlg, ID_EDIT_DATE), dateBuffer, 256);
                
                try {
                    double amount = std::stod(buffer);
                    if (amount > 0) {
                        dialogData.amount = amount;
                        dialogData.date = dateBuffer;
                        dialogData.accepted = true;
                        DestroyWindow(hwndDlg);
                        return 0;
                    } else {
                        MessageBox(hwndDlg, L"Amount must be greater than 0!", L"Error", MB_OK | MB_ICONERROR);
                    }
                } catch (...) {
                    MessageBox(hwndDlg, L"Please enter a valid number!", L"Error", MB_OK | MB_ICONERROR);
                }
                return 0;
            } else if (LOWORD(wParam) == IDCANCEL || (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == IDCANCEL)) {
                dialogData.accepted = false;
                DestroyWindow(hwndDlg);
                return 0;
            }
            break;
            
        case WM_CLOSE:
            dialogData.accepted = false;
            DestroyWindow(hwndDlg);
            return 0;
            
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwndDlg, msg, wParam, lParam);
}

LRESULT CALLBACK FinSyncApp::ExpenseDialogProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == IDOK)) {
                wchar_t buffer[256];
                wchar_t dateBuffer[256];
                GetWindowText(GetDlgItem(hwndDlg, ID_EDIT_AMOUNT), buffer, 256);
                GetWindowText(GetDlgItem(hwndDlg, ID_EDIT_DATE), dateBuffer, 256);
                
                int catIdx = SendMessage(GetDlgItem(hwndDlg, ID_COMBO_CATEGORY), CB_GETCURSEL, 0, 0);
                if (catIdx < 0) catIdx = 0;
                dialogData.category = instance->categories[catIdx];
                
                try {
                    double amount = std::stod(buffer);
                    if (amount > 0) {
                        dialogData.amount = amount;
                        dialogData.date = dateBuffer;
                        dialogData.accepted = true;
                        DestroyWindow(hwndDlg);
                        return 0;
                    } else {
                        MessageBox(hwndDlg, L"Amount must be greater than 0!", L"Error", MB_OK | MB_ICONERROR);
                    }
                } catch (...) {
                    MessageBox(hwndDlg, L"Please enter a valid number!", L"Error", MB_OK | MB_ICONERROR);
                }
                return 0;
            } else if (LOWORD(wParam) == IDCANCEL || (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == IDCANCEL)) {
                dialogData.accepted = false;
                DestroyWindow(hwndDlg);
                return 0;
            }
            break;
            
        case WM_CLOSE:
            dialogData.accepted = false;
            DestroyWindow(hwndDlg);
            return 0;
            
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwndDlg, msg, wParam, lParam);
}

LRESULT CALLBACK FinSyncApp::EditDialogProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_INITDIALOG:
            SetFocus(GetDlgItem(hwndDlg, ID_EDIT_AMOUNT));
            return FALSE;
            
        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == IDOK)) {
                wchar_t buffer[256];
                wchar_t dateBuffer[256];
                GetWindowText(GetDlgItem(hwndDlg, ID_EDIT_AMOUNT), buffer, 256);
                GetWindowText(GetDlgItem(hwndDlg, ID_EDIT_DATE), dateBuffer, 256);
                
                HWND hwndCombo = GetDlgItem(hwndDlg, ID_COMBO_CATEGORY);
                if (hwndCombo != NULL) {
                    int catIdx = ComboBox_GetCurSel(hwndCombo);
                    if (catIdx >= 0) {
                        dialogData.category = instance->categories[catIdx];
                    }
                }
                
                try {
                    double amount = std::stod(buffer);
                    if (amount > 0) {
                        dialogData.amount = amount;
                        dialogData.date = dateBuffer;
                        dialogData.accepted = true;
                        DestroyWindow(hwndDlg);
                        return 0;
                    } else {
                        MessageBox(hwndDlg, L"Amount must be greater than 0!", L"Error", MB_OK | MB_ICONERROR);
                    }
                } catch (...) {
                    MessageBox(hwndDlg, L"Please enter a valid amount!", L"Invalid Input", MB_OK | MB_ICONERROR);
                }
                return 0;
            } else if (LOWORD(wParam) == IDCANCEL || (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == IDCANCEL)) {
                dialogData.accepted = false;
                DestroyWindow(hwndDlg);
                return 0;
            }
            break;
            
        case WM_CLOSE:
            dialogData.accepted = false;
            DestroyWindow(hwndDlg);
            return 0;
            
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwndDlg, msg, wParam, lParam);
}

void FinSyncApp::CreateMainWindow(HINSTANCE hInstance) {
    const wchar_t CLASS_NAME[] = L"FinSyncWindowClass";
    
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = CreateSolidBrush(RGB(240, 244, 248));
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    
    RegisterClass(&wc);
    
    hwndMain = CreateWindowEx(
        0, CLASS_NAME, L"💰 FinSync - Financial Synchronization Hub",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 1100, 750,
        nullptr, nullptr, hInstance, nullptr
    );
    
    if (hwndMain == nullptr) return;
    
    // Initialize common controls
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_BAR_CLASSES;
    InitCommonControlsEx(&icex);
    
    // Create styled summary labels
    hwndIncomeLabel = CreateWindow(L"STATIC", L"Total Income: ₱0.00",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        30, 80, 330, 50, hwndMain, nullptr, hInstance, nullptr);
    
    hwndExpenseLabel = CreateWindow(L"STATIC", L"Total Expenses: ₱0.00",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        380, 80, 330, 50, hwndMain, nullptr, hInstance, nullptr);
    
    hwndSavingsLabel = CreateWindow(L"STATIC", L"Net Savings: ₱0.00",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        730, 80, 330, 50, hwndMain, nullptr, hInstance, nullptr);
    
    // Create buttons with better styling
    CreateWindow(L"BUTTON", L"➕ Add Income",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        30, 150, 170, 45, hwndMain, (HMENU)ID_BTN_ADD_INCOME, hInstance, nullptr);
    
    CreateWindow(L"BUTTON", L"➖ Add Expense",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        210, 150, 170, 45, hwndMain, (HMENU)ID_BTN_ADD_EXPENSE, hInstance, nullptr);
    
    CreateWindow(L"BUTTON", L"✏️ Edit",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        390, 150, 140, 45, hwndMain, (HMENU)ID_BTN_EDIT, hInstance, nullptr);
    
    CreateWindow(L"BUTTON", L"🗑️ Delete",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        540, 150, 140, 45, hwndMain, (HMENU)ID_BTN_DELETE, hInstance, nullptr);
    
    CreateWindow(L"BUTTON", L"📊 Report",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        690, 150, 170, 45, hwndMain, (HMENU)ID_BTN_REPORT, hInstance, nullptr);
    
    CreateWindow(L"BUTTON", L"💾 Save",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        870, 150, 170, 45, hwndMain, (HMENU)ID_BTN_SAVE, hInstance, nullptr);
    
    // Create ListView
    hwndListView = CreateWindow(WC_LISTVIEW, L"",
        WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | WS_BORDER,
        30, 215, 1030, 450, hwndMain, (HMENU)ID_LISTVIEW, hInstance, nullptr);
    
    // Setup ListView columns
    LVCOLUMN lvc;
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvc.fmt = LVCFMT_CENTER;
    
    lvc.iSubItem = 0;
    lvc.pszText = (LPWSTR)L"#";
    lvc.cx = 50;
    ListView_InsertColumn(hwndListView, 0, &lvc);
    
    lvc.iSubItem = 1;
    lvc.pszText = (LPWSTR)L"Type";
    lvc.cx = 130;
    ListView_InsertColumn(hwndListView, 1, &lvc);
    
    lvc.iSubItem = 2;
    lvc.pszText = (LPWSTR)L"Amount (₱)";
    lvc.cx = 180;
    ListView_InsertColumn(hwndListView, 2, &lvc);
    
    lvc.iSubItem = 3;
    lvc.pszText = (LPWSTR)L"Category";
    lvc.cx = 250;
    ListView_InsertColumn(hwndListView, 3, &lvc);
    
    lvc.iSubItem = 4;
    lvc.pszText = (LPWSTR)L"Date";
    lvc.cx = 180;
    ListView_InsertColumn(hwndListView, 4, &lvc);
    
    ListView_SetExtendedListViewStyle(hwndListView, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    
    // Status bar
    hwndStatusBar = CreateWindow(L"STATIC", L"Ready | Transactions: 0",
        WS_CHILD | WS_VISIBLE,
        30, 675, 1030, 25, hwndMain, nullptr, hInstance, nullptr);
    
    LoadData();
    RefreshListView();
    UpdateSummary();
    
    ShowWindow(hwndMain, SW_SHOW);
    UpdateWindow(hwndMain);
}

void FinSyncApp::AddIncome() {
    dialogData.accepted = false;
    
    // Register dialog class
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = IncomeDialogProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"IncomeDialogClass";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    
    UnregisterClass(L"IncomeDialogClass", GetModuleHandle(NULL)); // Clean up if exists
    RegisterClassEx(&wc);
    
    // Create dialog window
    HWND hwndDlg = CreateWindowEx(
        WS_EX_DLGMODALFRAME | WS_EX_TOPMOST,
        L"IncomeDialogClass",
        L"Add Income",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
        (GetSystemMetrics(SM_CXSCREEN) - 450) / 2,
        (GetSystemMetrics(SM_CYSCREEN) - 240) / 2,
        450, 240,
        hwndMain, NULL, GetModuleHandle(NULL), NULL
    );
    
    if (hwndDlg == NULL) {
        MessageBox(hwndMain, L"Failed to create dialog window!", L"Error", MB_OK | MB_ICONERROR);
        return;
    }
    
    CreateWindow(L"STATIC", L"Enter income amount (₱):",
        WS_CHILD | WS_VISIBLE,
        25, 25, 400, 22, hwndDlg, NULL, NULL, NULL);
    
    CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
        WS_CHILD | WS_VISIBLE | ES_NUMBER | WS_TABSTOP | ES_AUTOHSCROLL,
        25, 52, 400, 30, hwndDlg, (HMENU)ID_EDIT_AMOUNT, NULL, NULL);
    
    CreateWindow(L"STATIC", L"Date (DD/MM/YYYY):",
        WS_CHILD | WS_VISIBLE,
        25, 95, 400, 22, hwndDlg, NULL, NULL, NULL);
    
    CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", GetCurrentDate().c_str(),
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL,
        25, 122, 400, 30, hwndDlg, (HMENU)ID_EDIT_DATE, NULL, NULL);
    
    CreateWindow(L"BUTTON", L"OK",
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_TABSTOP,
        140, 175, 120, 40, hwndDlg, (HMENU)IDOK, NULL, NULL);
    
    CreateWindow(L"BUTTON", L"Cancel",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP,
        270, 175, 120, 40, hwndDlg, (HMENU)IDCANCEL, NULL, NULL);
    
    SetFocus(GetDlgItem(hwndDlg, ID_EDIT_AMOUNT));
    EnableWindow(hwndMain, FALSE);
    
    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!IsWindow(hwndDlg)) break;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    EnableWindow(hwndMain, TRUE);
    SetForegroundWindow(hwndMain);
    
    if (dialogData.accepted) {
        transactions.emplace_back(L"Income", dialogData.amount, L"N/A", dialogData.date);
        RefreshListView();
        UpdateSummary();
        SetWindowText(hwndStatusBar, L"✓ Income added successfully!");
    }
    
    UnregisterClass(L"IncomeDialogClass", GetModuleHandle(NULL));
}

void FinSyncApp::AddExpense() {
    dialogData.accepted = false;
    
    // Register dialog class
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = ExpenseDialogProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"ExpenseDialogClass";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    
    UnregisterClass(L"ExpenseDialogClass", GetModuleHandle(NULL)); // Clean up if exists
    RegisterClassEx(&wc);
    
    // Create dialog window
    HWND hwndDlg = CreateWindowEx(
        WS_EX_DLGMODALFRAME | WS_EX_TOPMOST,
        L"ExpenseDialogClass",
        L"Add Expense",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
        (GetSystemMetrics(SM_CXSCREEN) - 450) / 2,
        (GetSystemMetrics(SM_CYSCREEN) - 340) / 2,
        450, 340,
        hwndMain, NULL, GetModuleHandle(NULL), NULL
    );
    
    CreateWindow(L"STATIC", L"Category:",
        WS_CHILD | WS_VISIBLE,
        25, 25, 400, 22, hwndDlg, NULL, NULL, NULL);
    
    HWND hwndCombo = CreateWindow(L"COMBOBOX", NULL,
        WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_TABSTOP,
        25, 52, 400, 200, hwndDlg, (HMENU)ID_COMBO_CATEGORY, NULL, NULL);
    
    for (const auto& cat : categories) {
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)cat.c_str());
    }
    SendMessage(hwndCombo, CB_SETCURSEL, 0, 0);
    
    CreateWindow(L"STATIC", L"Enter amount (₱):",
        WS_CHILD | WS_VISIBLE,
        25, 105, 400, 22, hwndDlg, NULL, NULL, NULL);
    
    CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
        WS_CHILD | WS_VISIBLE | ES_NUMBER | WS_TABSTOP | ES_AUTOHSCROLL,
        25, 132, 400, 30, hwndDlg, (HMENU)ID_EDIT_AMOUNT, NULL, NULL);
    
    CreateWindow(L"STATIC", L"Date (DD/MM/YYYY):",
        WS_CHILD | WS_VISIBLE,
        25, 175, 400, 22, hwndDlg, NULL, NULL, NULL);
    
    CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", GetCurrentDate().c_str(),
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL,
        25, 202, 400, 30, hwndDlg, (HMENU)ID_EDIT_DATE, NULL, NULL);
    
    CreateWindow(L"BUTTON", L"OK",
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_TABSTOP,
        140, 260, 120, 40, hwndDlg, (HMENU)IDOK, NULL, NULL);
    
    CreateWindow(L"BUTTON", L"Cancel",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP,
        270, 260, 120, 40, hwndDlg, (HMENU)IDCANCEL, NULL, NULL);
    
    SetFocus(GetDlgItem(hwndDlg, ID_EDIT_AMOUNT));
    EnableWindow(hwndMain, FALSE);
    
    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!IsWindow(hwndDlg)) break;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    EnableWindow(hwndMain, TRUE);
    SetForegroundWindow(hwndMain);
    
    if (dialogData.accepted) {
        transactions.emplace_back(L"Expense", dialogData.amount, dialogData.category, dialogData.date);
        RefreshListView();
        UpdateSummary();
        SetWindowText(hwndStatusBar, L"✓ Expense added successfully!");
    }
    
    UnregisterClass(L"ExpenseDialogClass", GetModuleHandle(NULL));
}

void FinSyncApp::EditTransaction() {
    int selected = ListView_GetNextItem(hwndListView, -1, LVNI_SELECTED);
    if (selected < 0) {
        MessageBox(hwndMain, L"Please select a transaction to edit!", L"No Selection", MB_OK | MB_ICONINFORMATION);
        return;
    }
    
    Transaction& trans = transactions[selected];
    dialogData.accepted = false;
    dialogData.amount = trans.amount;
    dialogData.date = trans.date;
    dialogData.category = trans.category;
    
    // Register dialog class
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = EditDialogProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"EditDialogClass";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    
    UnregisterClass(L"EditDialogClass", GetModuleHandle(NULL));
    RegisterClassEx(&wc);
    
    // Create dialog window
    HWND hwndDlg = CreateWindowEx(
        WS_EX_DLGMODALFRAME | WS_EX_TOPMOST,
        L"EditDialogClass",
        L"Edit Transaction",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
        (GetSystemMetrics(SM_CXSCREEN) - 450) / 2,
        (GetSystemMetrics(SM_CYSCREEN) - 340) / 2,
        450, 340,
        hwndMain, NULL, GetModuleHandle(NULL), NULL
    );
    
    int yPos = 25;
    std::wstring typeLabel = L"Type: " + trans.type;
    CreateWindow(L"STATIC", typeLabel.c_str(),
        WS_CHILD | WS_VISIBLE,
        25, yPos, 400, 22, hwndDlg, NULL, NULL, NULL);
    yPos += 35;
    
    HWND hwndCombo = NULL;
    if (trans.type == L"Expense") {
        CreateWindow(L"STATIC", L"Category:",
            WS_CHILD | WS_VISIBLE,
            25, yPos, 400, 22, hwndDlg, NULL, NULL, NULL);
        yPos += 28;
        
        hwndCombo = CreateWindow(L"COMBOBOX", NULL,
            WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_TABSTOP,
            25, yPos, 400, 200, hwndDlg, (HMENU)ID_COMBO_CATEGORY, NULL, NULL);
        
        for (size_t i = 0; i < categories.size(); ++i) {
            ComboBox_AddString(hwndCombo, categories[i].c_str());
            if (categories[i] == trans.category) {
                ComboBox_SetCurSel(hwndCombo, i);
            }
        }
        yPos += 45;
    }
    
    CreateWindow(L"STATIC", L"Amount (₱):",
        WS_CHILD | WS_VISIBLE,
        25, yPos, 400, 22, hwndDlg, NULL, NULL, NULL);
    yPos += 28;
    
    wchar_t amountStr[50];
    swprintf_s(amountStr, L"%.2f", trans.amount);
    CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", amountStr,
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL,
        25, yPos, 400, 30, hwndDlg, (HMENU)ID_EDIT_AMOUNT, NULL, NULL);
    yPos += 45;
    
    CreateWindow(L"STATIC", L"Date (DD/MM/YYYY):",
        WS_CHILD | WS_VISIBLE,
        25, yPos, 400, 22, hwndDlg, NULL, NULL, NULL);
    yPos += 28;
    
    CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", trans.date.c_str(),
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL,
        25, yPos, 400, 30, hwndDlg, (HMENU)ID_EDIT_DATE, NULL, NULL);
    yPos += 50;
    
    CreateWindow(L"BUTTON", L"OK",
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_TABSTOP,
        140, yPos, 120, 40, hwndDlg, (HMENU)IDOK, NULL, NULL);
    
    CreateWindow(L"BUTTON", L"Cancel",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP,
        270, yPos, 120, 40, hwndDlg, (HMENU)IDCANCEL, NULL, NULL);
    
    SetFocus(GetDlgItem(hwndDlg, ID_EDIT_AMOUNT));
    EnableWindow(hwndMain, FALSE);
    
    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!IsWindow(hwndDlg)) break;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    EnableWindow(hwndMain, TRUE);
    SetForegroundWindow(hwndMain);
    
    if (dialogData.accepted) {
        trans.amount = dialogData.amount;
        trans.date = dialogData.date;
        if (trans.type == L"Expense") {
            trans.category = dialogData.category;
        }
        RefreshListView();
        UpdateSummary();
        SetWindowText(hwndStatusBar, L"✓ Transaction updated successfully!");
    }
    
    UnregisterClass(L"EditDialogClass", GetModuleHandle(NULL));
}

void FinSyncApp::DeleteTransaction() {
    int selected = ListView_GetNextItem(hwndListView, -1, LVNI_SELECTED);
    if (selected < 0) {
        MessageBox(hwndMain, L"Please select a transaction to delete!", L"No Selection", MB_OK | MB_ICONINFORMATION);
        return;
    }
    
    int result = MessageBox(hwndMain, L"Are you sure you want to delete this transaction?", 
                           L"Confirm Delete", MB_YESNO | MB_ICONQUESTION);
    
    if (result == IDYES) {
        transactions.erase(transactions.begin() + selected);
        RefreshListView();
        UpdateSummary();
        SetWindowText(hwndStatusBar, L"✓ Transaction deleted successfully!");
    }
}

void FinSyncApp::GenerateReport() {
    double totalIncome = 0, totalExpense = 0;
    std::vector<double> categoryTotals(categories.size(), 0);
    
    for (const auto& t : transactions) {
        if (t.type == L"Income") {
            totalIncome += t.amount;
        } else {
            totalExpense += t.amount;
            auto it = std::find(categories.begin(), categories.end(), t.category);
            if (it != categories.end()) {
                categoryTotals[std::distance(categories.begin(), it)] += t.amount;
            }
        }
    }
    
    std::wstringstream report;
    report << L"💰 FINANCIAL REPORT 💰\n\n";
    report << L"═══════════════════════════════\n";
    report << L"Total Income:    ₱" << std::fixed << std::setprecision(2) << totalIncome << L"\n";
    report << L"Total Expenses:  ₱" << totalExpense << L"\n";
    report << L"───────────────────────────────\n";
    report << L"Net Savings:     ₱" << (totalIncome - totalExpense) << L"\n";
    report << L"═══════════════════════════════\n\n";
    
    report << L"📊 EXPENSES BY CATEGORY:\n";
    for (size_t i = 0; i < categories.size(); ++i) {
        if (categoryTotals[i] > 0) {
            double percentage = (totalExpense > 0) ? (categoryTotals[i] / totalExpense * 100) : 0;
            report << L"\n" << categories[i] << L": ₱" << categoryTotals[i] 
                   << L" (" << std::setprecision(1) << percentage << L"%)";
        }
    }
    
    MessageBox(hwndMain, report.str().c_str(), L"Financial Report", MB_OK | MB_ICONINFORMATION);
}

void FinSyncApp::RefreshListView() {
    ListView_DeleteAllItems(hwndListView);
    
    LVITEM lvi = {0};
    lvi.mask = LVIF_TEXT;
    
    for (size_t i = 0; i < transactions.size(); ++i) {
        wchar_t index[10];
        swprintf_s(index, L"%d", i + 1);
        
        lvi.iItem = i;
        lvi.iSubItem = 0;
        lvi.pszText = index;
        ListView_InsertItem(hwndListView, &lvi);
        
        ListView_SetItemText(hwndListView, i, 1, (LPWSTR)transactions[i].type.c_str());
        
        wchar_t amount[50];
        swprintf_s(amount, L"%.2f", transactions[i].amount);
        ListView_SetItemText(hwndListView, i, 2, amount);
        
        std::wstring category = transactions[i].category.empty() ? L"N/A" : transactions[i].category;
        ListView_SetItemText(hwndListView, i, 3, (LPWSTR)category.c_str());
        ListView_SetItemText(hwndListView, i, 4, (LPWSTR)transactions[i].date.c_str());
    }
    
    wchar_t statusText[256];
    swprintf_s(statusText, L"Ready | Transactions: %d", transactions.size());
    SetWindowText(hwndStatusBar, statusText);
}

void FinSyncApp::UpdateSummary() {
    double totalIncome = 0, totalExpense = 0;
    
    for (const auto& t : transactions) {
        if (t.type == L"Income") {
            totalIncome += t.amount;
        } else {
            totalExpense += t.amount;
        }
    }
    
    wchar_t buffer[256];
    swprintf_s(buffer, L"Total Income: ₱%.2f", totalIncome);
    SetWindowText(hwndIncomeLabel, buffer);
    
    swprintf_s(buffer, L"Total Expenses: ₱%.2f", totalExpense);
    SetWindowText(hwndExpenseLabel, buffer);
    
    swprintf_s(buffer, L"Net Savings: ₱%.2f", totalIncome - totalExpense);
    SetWindowText(hwndSavingsLabel, buffer);
}

void FinSyncApp::SaveData() {
    std::wofstream file(L"transactions.txt");
    if (!file.is_open()) {
        MessageBox(hwndMain, L"Failed to save data!", L"Error", MB_OK | MB_ICONERROR);
        return;
    }
    
    for (const auto& t : transactions) {
        file << t.type << L"," << t.amount << L"," 
             << t.category << L"," << t.date << L"\n";
    }
    
    file.close();
    SetWindowText(hwndStatusBar, L"✓ Data saved successfully!");
}

void FinSyncApp::LoadData() {
    std::wifstream file(L"transactions.txt");
    if (!file.is_open()) return;
    
    transactions.clear();
    std::wstring line;
    
    while (std::getline(file, line)) {
        std::wstringstream ss(line);
        std::wstring type, category, date;
        double amount;
        
        std::getline(ss, type, L',');
        ss >> amount;
        ss.ignore();
        std::getline(ss, category, L',');
        std::getline(ss, date);
        
        transactions.emplace_back(type, amount, category, date);
    }
    
    file.close();
}

LRESULT CALLBACK FinSyncApp::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_BTN_ADD_INCOME:
                    instance->AddIncome();
                    break;
                case ID_BTN_ADD_EXPENSE:
                    instance->AddExpense();
                    break;
                case ID_BTN_EDIT:
                    instance->EditTransaction();
                    break;
                case ID_BTN_DELETE:
                    instance->DeleteTransaction();
                    break;
                case ID_BTN_REPORT:
                    instance->GenerateReport();
                    break;
                case ID_BTN_SAVE:
                    instance->SaveData();
                    break;
            }
            return 0;
            
        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wParam;
            HWND hwndStatic = (HWND)lParam;
            
            if (hwndStatic == instance->hwndIncomeLabel) {
                SetTextColor(hdcStatic, RGB(0, 128, 0));
                SetBkColor(hdcStatic, RGB(230, 255, 230));
                return (LRESULT)CreateSolidBrush(RGB(230, 255, 230));
            } else if (hwndStatic == instance->hwndExpenseLabel) {
                SetTextColor(hdcStatic, RGB(200, 0, 0));
                SetBkColor(hdcStatic, RGB(255, 230, 230));
                return (LRESULT)CreateSolidBrush(RGB(255, 230, 230));
            } else if (hwndStatic == instance->hwndSavingsLabel) {
                SetTextColor(hdcStatic, RGB(0, 0, 200));
                SetBkColor(hdcStatic, RGB(230, 240, 255));
                return (LRESULT)CreateSolidBrush(RGB(230, 240, 255));
            }
            break;
        }
        
        case WM_DESTROY:
            instance->SaveData();
            PostQuitMessage(0);
            return 0;
            
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            // Draw header
            RECT rect = {0, 0, 1100, 70};
            HBRUSH hBrush = CreateSolidBrush(RGB(52, 152, 219));
            FillRect(hdc, &rect, hBrush);
            DeleteObject(hBrush);
            
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(255, 255, 255));
            HFONT hFont = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 
                                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                    CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Segoe UI");
            SelectObject(hdc, hFont);
            
            DrawText(hdc, L"💰 FinSync", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            DeleteObject(hFont);
            
            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int) {
    FinSyncApp app;
    app.CreateMainWindow(hInstance);
    
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return (int)msg.wParam;
}
