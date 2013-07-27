
#include "WinCommon.h"
#include "WindowSelect.h"
#include "../resource.h"
#include <utility>              // for std::pair

using namespace Dasher;

CWinSel::CWinSel(HWND Parent, CEdit *m_pEdit)
:m_hwnd(0), m_pEdit(m_pEdit)
{
  g_hRectanglePen = CreatePen(PS_SOLID, 3, RGB(256, 0, 0));
  g_bStartSearchWindow = FALSE;
  g_hBitmapFinderToolFilled = LoadBitmap(WinHelper::hInstApp, MAKEINTRESOURCE(IDB_FINDER_FULL));
  
  g_hCursorSearchWindow = LoadCursor(WinHelper::hInstApp, MAKEINTRESOURCE(IDC_SEARCH_CURSOR));
  DialogBoxParam(WinHelper::hInstApp, (LPCTSTR) IDD_WINSELECT, Parent, (DLGPROC) WinWrapMap::WndProc, (LPARAM) this);
}

long CWinSel::RefreshWindow(HWND hwndWindowToBeRefreshed) 
    

  long lRet = 0;
  // Set the screen cursor to the BullsEye cursor.
  if(g_hCursorSearchWindow) {  
  else {

long CWinSel::DoMouseMove(HWND hwndDialog, UINT message, WPARAM wParam, LPARAM lParam) {
  // Must use GetCursorPos() instead of calculating 
  // Determine the window that lies underneath 
  // We have just found a new window.
  if(g_hwndFoundWindow) {

  // Indicate that this found window is now 
  // the current global found window.
  g_hwndFoundWindow = hwndFoundWindow;

  // We now highlight the found window.
  HighlightFoundWindow(hwndDialog, g_hwndFoundWindow);

long CWinSel::HighlightFoundWindow(HWND hwndDialog, HWND hwndFoundWindow) {
  // The DC of the found window.
  HDC hWindowDC = NULL;

  // Handle of the existing pen in the DC of the found window.
  HGDIOBJ hPrevPen = NULL;
  // Handle of the existing brush in the DC of the found window.
  HGDIOBJ hPrevBrush = NULL;
  long lRet = 0;
  // Get the screen coordinates of the rectangle 
  // of the found window.
  GetWindowRect(hwndFoundWindow, &rect);
  // Get the window DC of the found window.
  hWindowDC = GetWindowDC(hwndFoundWindow);
    // Select our created pen into the DC and 
    // Select a transparent brush into the DC and 
    // Draw a rectangle in the DC covering 
    // Reinsert the previous pen and brush 
    // Finally release the DC.

long CWinSel::DoMouseUp(HWND hwndDialog, UINT message, WPARAM wParam, LPARAM lParam) {
  // If we had a previous cursor, set the 
  // If there was a found window, refresh 
  }
  // Very important : must release the mouse capture.

void CWinSel::SetFinderToolImage(HWND hwndDialog, BOOL full) {
  HBITMAP hBmpToSet = NULL;
  if(full) {
    // Set a FILLED image.
    hBmpToSet = g_hBitmapFinderToolFilled;
  else {
    // Set an EMPTY image.
  }
  SendDlgItemMessage 
    (int)IDC_DRAGCONTROL, // identifier of control
    (UINT) STM_SETIMAGE,  // message to send
    (WPARAM) IMAGE_BITMAP,        // first message parameter
    (LPARAM) hBmpToSet    // second message parameter
    );
}

LRESULT CWinSel::WndProc(HWND Window, UINT message, WPARAM wParam, LPARAM lParam) {
  switch (message) {
  case WM_MOUSEMOVE: {
      // Only when we have started the 
      // Window Searching operation will we 
      // track mouse movement.
      DoMouseMove(Window, message, wParam, lParam);
  case WM_LBUTTONUP: {
      // Only when we have started the 
      // window searching operation will we
      // be interested when the user lifts 
      // up the left mouse button.
      DoMouseUp(Window, message, wParam, lParam);
  case WM_INITDIALOG:
    {
      if(!m_hwnd) {             // If this is the initial dialog for the first time
        m_hwnd = Window;
      }
      return TRUE;
      break;
    }
  case WM_COMMAND:
    switch (LOWORD(wParam)) {
    case (IDC_DISPLAY):
      if(HIWORD(wParam) == EN_CHANGE) {
        HWND Control = GetDlgItem(Window, IDC_DISPLAY);
        LRESULT BufferLength = SendMessage(Control, WM_GETTEXTLENGTH, 0, 0) + 1;        // +1 to allow for terminator
        TCHAR *Buffer = new TCHAR[BufferLength];
        SendMessage(Control, WM_GETTEXT, BufferLength, (LPARAM) Buffer);
        std::string ItemName;
        SendMessage(GetDlgItem(Window, IDC_TEXT), WM_SETTEXT, 0, (LPARAM) Buffer);
        delete[]Buffer;
      }
      break;
    case (IDOK):
      m_pEdit->SetWindow(g_hwndFoundWindow);
      // Fall through 
    case (IDCANCEL):
      {
        EndDialog(Window, LOWORD(wParam));
        return TRUE;
      }
    case IDC_DRAGCONTROL:
      {
        SearchWindow(Window);
        return TRUE;
        break;
      }
    }
  default:
    return FALSE;
  }
}