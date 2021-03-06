/**************************************************************************/
/*                                                                        */
/*                              WWIV Version 5.x                          */
/*             Copyright (C)1998-2016,WWIV Software Services             */
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

#ifndef __INCLUDED_PLATFORM_WWIVPORT_H__
#define __INCLUDED_PLATFORM_WWIVPORT_H__

#if !defined(__unix__) && !defined(_WIN32) && defined(__APPLE__) && defined(__MACH__)
#define __unix__
#endif

#ifdef _MSC_VER
#ifdef _WIN64
  typedef int64_t ssize_t;
#else 
typedef int32_t ssize_t;
#endif  // _WIN64
#endif  // MSVC

#endif // __INCLUDED_PLATFORM_WWIVPORT_H__
