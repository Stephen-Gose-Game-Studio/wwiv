/**************************************************************************/
/*                                                                        */
/*                              WWIV Version 5.x                          */
/*             Copyright (C)1998-2016, WWIV Software Services             */
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
#include "sdk/usermanager.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <memory>
#include "core/strings.h"
#include "core/file.h"
#include "sdk/names.h"
#include "sdk/filenames.h"
#include "sdk/user.h"

using namespace wwiv::strings;

namespace wwiv {
namespace sdk {

/////////////////////////////////////////////////////////////////////////////
// class UserManager

UserManager::UserManager(std::string data_directory, int userrec_length, int max_number_users)
  : data_directory_(data_directory), userrec_length_(userrec_length),
    max_number_users_(max_number_users), allow_writes_(true) {}

UserManager::~UserManager() { }

int  UserManager::GetNumberOfUserRecords() const {
  File userList(data_directory_, USER_LST);
  if (userList.Open(File::modeReadOnly | File::modeBinary)) {
    long nSize = userList.GetLength();
    int nNumRecords = static_cast<int>(nSize / userrec_length_) - 1;
    return nNumRecords;
  }
  return 0;
}

bool UserManager::ReadUserNoCache(User *pUser, int user_number) {
  File userList(data_directory_, USER_LST);
  if (!userList.Open(File::modeReadOnly | File::modeBinary)) {
    pUser->data.inact = inact_deleted;
    pUser->FixUp();
    return false;
  }
  long nSize = userList.GetLength();
  int nNumUserRecords = static_cast<int>(nSize / userrec_length_) - 1;

  if (user_number > nNumUserRecords) {
    pUser->data.inact = inact_deleted;
    pUser->FixUp();
    return false;
  }
  long pos = static_cast<long>(userrec_length_) * static_cast<long>(user_number);
  userList.Seek(pos, File::seekBegin);
  userList.Read(&pUser->data, userrec_length_);
  pUser->FixUp();
  return true;
}

bool UserManager::ReadUser(User *pUser, int user_number) {
  return this->ReadUserNoCache(pUser, user_number);
}

bool UserManager::WriteUserNoCache(User *pUser, int user_number) {
  File userList(data_directory_, USER_LST);
  if (userList.Open(File::modeReadWrite | File::modeBinary | File::modeCreateFile)) {
    long pos = static_cast<long>(userrec_length_) * static_cast<long>(user_number);
    userList.Seek(pos, File::seekBegin);
    userList.Write(&pUser->data, userrec_length_);
    return true;
  }
  return false;
}

bool UserManager::WriteUser(User *pUser, int user_number) {
  if (user_number < 1 || user_number > max_number_users_ || !IsUserWritesAllowed()) {
    return true;
  }

  return this->WriteUserNoCache(pUser, user_number);
}

}  // namespace sdk
}  // namespace wwiv
