/**************************************************************************/
/*                                                                        */
/*                              WWIV Version 5.0x                         */
/*             Copyright (C)1998-2014, WWIV Software Services             */
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
#ifndef __INCLUDED_INPUT_H__
#define __INCLUDED_INPUT_H__

#include <string>

void input1(char *pszOutText, int nMaxLength, int lc, bool crend, bool bAutoMpl = false);
void input1(std::string *strOutText, int nMaxLength, int lc, bool crend, bool bAutoMpl = false);
void input(char *pszOutText, int nMaxLength, bool bAutoMpl = false);
void input(std::string *strOutText, int nMaxLength, bool bAutoMpl = false);
void inputl(char *pszOutText, int nMaxLength, bool bAutoMpl = false);
void inputl(std::string *strOutText, int nMaxLength, bool bAutoMpl = false);
int  Input1(char *pszOutText, std::string origText, int nMaxLength, bool bInsert, int mode);
int  Input1(std::string *strOutText, std::string origText, int nMaxLength, bool bInsert, int mode);
void input_password(std::string promptText, std::string *strOutPassword, int nMaxLength);

#endif  // __INCLUDED_INPUT_H__