/**************************************************************************/
/*                                                                        */
/*                 WWIV Initialization Utility Version 5.0                */
/*                 Copyright (C)2014, WWIV Software Services              */
/*                                                                        */
/*    Licensed  under the  Apache License, Version  2.0 (the "License");  */
/*    you may not use this  file  except in compliance with the License.  */
/*    You may obtain a copy of the License at                             */
/*                                                                        */
/*                http://www.apache.org/licenses/LICENSE-2.0              */
/*                                                                        */
/*    Unless  required  by  applicable  law  or agreed to  in  writing,   */
/*    software  distributed  under  the  License  is  distributed on an   */
/*    "AS IS"  BASIS, WITHOUT  WARRANTIES  OR  CONDITIONS OF ANY  KIND,   */
/*    either  express  or implied.  See  the  License for  the specific   */
/*    language governing permissions and limitations under the License.   */
/*                                                                        */
/**************************************************************************/
#ifndef __INCLUDED_PLATFORM_CURSES_WIN_H__
#define __INCLUDED_PLATFORM_CURSES_WIN_H__

#include <map>
#include <string>
#include <curses.h>
#include "colors.h"

#ifdef INSERT // defined in wconstants.h
#undef INSERT
#endif  // INSERT

// Curses implementation of screen display routines for Init.
class CursesWindow {
 public:
  // Constructor/Destructor
  CursesWindow(CursesWindow* parent, int nlines, int ncols, int begin_y, int begin_x);
  CursesWindow(const CursesWindow& copy) = delete;
  virtual ~CursesWindow();

  int AddCh(chtype ch) { return waddch(window_, ch); }
  int AddStr(const std::string s) { return waddstr(window_, s.c_str()); }
  int MvAddStr(int y, int x, const std::string s) { return mvwaddstr(window_, y, x, s.c_str()); }
  void Bkgd(chtype ch) { wbkgd(window_, ch); }
  int RedrawWin() { return redrawwin(window_); } 
  int TouchWin() { return touchwin(window_); }
  int Refresh() { return wrefresh(window_); }
  int Move(int y, int x) { return wmove(window_, y, x); }
  int GetcurX() { return getcurx(window_); }
  int GetcurY() { return getcury(window_); }
  int Clear() { return wclear(window_); }
  int Erase() { return werase(window_); }
  int GetChar() { return wgetch(window_); }
  int AttrSet(chtype attrs) { return wattrset(window_, attrs); }
  int Keypad(bool b) { return keypad(window_, b); }
  int GetMaxX() const { return getmaxx(window_); }
  int GetMaxY() const { return getmaxy(window_); }
  int ClrtoEol() const { return wclrtoeol(window_); }
  int AttrGet(attr_t* a, short* c) const { return wattr_get(window_, a, c, nullptr); }
  int Box(chtype vert_ch, chtype horiz_ch) { return box(window_, vert_ch, horiz_ch); }

  void GotoXY(int x, int y);
  void Putch(unsigned char ch);
  void Puts(const std::string& text);
  void PutsXY(int x, int y, const std::string& text);
  void Printf(const char *pszFormat, ...);
  void PrintfXY(int x, int y, const char *pszFormat, ...);

  void SetColor(ColorScheme* scheme, SchemeId id);
  

  WINDOW* window() const { return window_; }
  CursesWindow* parent() const { parent_; }

private:
  WINDOW* window_;
  CursesWindow* parent_;
};

#endif // __INCLUDED_PLATFORM_CURSES_WIN_H__
