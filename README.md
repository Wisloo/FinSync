# 💰 FinSync - Financial Synchronization Hub

A beautiful, modern GUI application for tracking your income and expenses built with native Windows API (Win32) and C++.

**✅ No external libraries required!** Works out of the box on Windows.

## 📖 Quick Links

- **[Quick Start Guide](QUICKSTART.md)** - Get up and running in 2 minutes!

## Main File

**[FinSyncWin32_Fixed.cpp](FinSyncWin32_Fixed.cpp)** - The main application file with fully functional dialogs and smooth UI.

The [CMakeLists.txt](CMakeLists.txt) is already configured to build the application.

## Features

- ✅ **Add Income & Expenses** - Track all your financial transactions
- 📊 **Category Management** - Organize expenses by Food, Rent, Entertainment, Transportation, Utilities, and more
- 📈 **Financial Reports** - Generate detailed reports with category breakdowns and percentages
- 💾 **Data Persistence** - Automatically save and load your data
- 🎨 **Modern UI** - Beautiful, intuitive interface with color-coded transactions and smooth dialogs
- 📅 **Date Tracking** - Easy date input with current date default
- ✏️ **Edit & Delete** - Modify or remove transactions as needed
- 📊 **Real-time Summary** - Always see your total income, expenses, and net savings
- 💱 **Philippine Peso (₱)** - Full support for Philippine currency

## Screenshots

### Main Application Window
![FinSync Main Window](FinSyncScreenshots/MainPage.png)

The application features:
- A clean header with "FinSync" branding and modern blue color scheme
- Financial overview panel showing Total Income (green), Total Expenses (red), and Net Savings (blue)
- Action buttons for adding income, expenses, editing, deleting, generating reports, and saving
- A comprehensive transaction table with color-coded entries
- **FULLY RESPONSIVE** dialog boxes with working OK, Cancel, and X buttons
- Philippine Peso (₱) currency display throughout

## Recent Fixes (December 12, 2025)

✅ **Fixed laggy/unresponsive dialog buttons** - Dialogs now use proper Windows message handling
✅ **Updated to Philippine Peso (₱)** - Full currency support throughout the application  
✅ **Improved GUI appearance** - Better colors, spacing, and modern design
✅ **Removed unnecessary files** - Cleaned up old console and duplicate versions
✅ **Smooth dialog operations** - OK, Cancel, and X buttons work instantly

## Installation

### Prerequisites

- **C++17 compatible compiler** (MSVC, MinGW-GCC, or Clang)
- **Windows OS** (uses native Win32 API)
- **CLion, Visual Studio, or any C++ IDE**
- **NO external libraries needed!**

### Windows (CLion - Recommended)

1. **Open the project in CLion:**
   - Launch CLion
   - File → Open
   - Select the FinSync folder

2. **CLion will automatically detect CMakeLists.txt and configure the project**

3. **Build & Run:**
   - Click the Build button (🔨) or press **Ctrl+F9**
   - Click the Run button (▶️) or press **Shift+F10**
   - The GUI window will appear immediately!

**Alternative: Pre-built Executable**
- Simply run `FinSync.exe` in the main folder or cmake-build-debug folder!

### Windows (Command Line)

#### Option 1: Using the Build Script (Easiest!)
```bash
build.bat
```
This will automatically detect your compiler (Visual Studio or MinGW) and build the application.

#### Option 2: Manual Build with MinGW
```bash
g++ -std=c++17 -municode -mwindows FinSyncWin32_Fixed.cpp -o FinSync.exe -lcomctl32 -lgdi32 -luser32
```

#### Option 3: Manual Build with Visual Studio (cl.exe)
```bash
cl /EHsc /std:c++17 /D UNICODE /D _UNICODE FinSyncWin32_Fixed.cpp /Fe:FinSync.exe /link user32.lib gdi32.lib comctl32.lib
```

### Windows (Visual Studio)

1. Open the FinSync folder in Visual Studio
2. Visual Studio will detect CMakeLists.txt
3. Press F5 to build and run

## Usage

### Adding Income
1. Click the "➕ Add Income" button
2. Enter the amount
3. Select the date (defaults to today)
4. Click OK

### Adding an Expense
1. Click the "➖ Add Expense" button
2. Select a category from the dropdown
3. Enter the amount
4. Select the date
5. Click OK

### Editing a Transaction
1. Click on a transaction in the table to select it
2. Click the "✏️ Edit" button
3. Modify the fields as needed
4. Click OK

### Deleting a Transaction
1. Select a transaction in the table
2. Click the "🗑️ Delete" button
3. Confirm the deletion

### Generating Reports
1. Click the "📊 Generate Report" button
2. View the comprehensive financial summary including:
   - Total income, expenses, and net savings
   - Expense breakdown by category with percentages

### Saving Data
- Data is automatically saved when you close the application
- You can manually save by clicking "💾 Save" or using File → Save
- Data is stored in `transactions.txt` in the application directory

## Project Structure

```
FinSync/
├── FinSyncWin32_Fixed.cpp  # Main application file (UPDATED & FIXED!)
├── CMakeLists.txt          # CMake build file (for CLion)
├── transactions.txt        # Data file (generated at runtime)
└── README.md               # This file
```

## Data Format

Transactions are saved in CSV format:
```
Type,Amount,Category,Date
Income,5000.00,,15/12/2025
Expense,50.25,Food,15/12/2025
Expense,1500.00,Rent,01/12/2025
```

## Customization

### Adding New Categories
Edit the `categories` vector in [FinSyncWin32_Fixed.cpp](FinSyncWin32_Fixed.cpp):
```cpp
const std::vector<std::wstring> categories = {
    L"Food", L"Rent", L"Entertainment", L"Transportation", 
    L"Utilities", L"Healthcare", L"Other"  // Add new categories here
};
```

## Author
Louis Antonio D. Lapayag - Lead Developer

## License

This project is open source and available for personal and educational use.

## Support

For issues or questions, please create an issue in the project repository.

---

**FinSync** - Smart Financial Synchronization & Expense Tracking 💰
