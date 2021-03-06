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

#include "bbs/bbs.h"
#include "bbs/fcns.h"
#include "bbs/vars.h"
#include "core/strings.h"
#include "core/wwivassert.h"
#include "sdk/filenames.h"

void read_bbs_list();
int system_index(int ts);

using namespace wwiv::strings;

void zap_call_out_list() {
  if (session()->current_net().con) {
    free(session()->current_net().con);
    session()->current_net().con = nullptr;
    session()->current_net().num_con = 0;
  }
}


void read_call_out_list() {
  net_call_out_rec *con;

  zap_call_out_list();

  File fileCallout(session()->network_directory(), CALLOUT_NET);
  if (fileCallout.Open(File::modeBinary | File::modeReadOnly)) {
    long lFileLength = fileCallout.GetLength();
    char *ss = nullptr;
    if ((ss = static_cast<char*>(BbsAllocA(lFileLength + 512))) == nullptr) {
      WWIV_ASSERT(ss != nullptr);
      session()->AbortBBS(true);
    }
    fileCallout.Read(ss, lFileLength);
    fileCallout.Close();
    for (long lPos = 0L; lPos < lFileLength; lPos++) {
      if (ss[lPos] == '@') {
        ++session()->current_net().num_con;
      }
    }
    free(ss);
    if ((session()->current_net().con = (net_call_out_rec *)
         BbsAllocA(static_cast<long>((session()->current_net().num_con + 2) *
                                     sizeof(net_call_out_rec)))) == nullptr) {
      WWIV_ASSERT(session()->current_net().con != nullptr);
      session()->AbortBBS(true);
    }
    con = session()->current_net().con;
    con--;
    fileCallout.Open(File::modeBinary | File::modeReadOnly);
    if ((ss = static_cast<char*>(BbsAllocA(lFileLength + 512))) == nullptr) {
      WWIV_ASSERT(ss != nullptr);
      session()->AbortBBS(true);
    }
    fileCallout.Read(ss, lFileLength);
    fileCallout.Close();
    long p = 0L;
    while (p < lFileLength) {
      while ((p < lFileLength) && (strchr("@%/\"&-=+~!();^|#$_*", ss[p]) == nullptr)) {
        ++p;
      }
      if (p < lFileLength) {
        switch (ss[p]) {
        case '@':
          ++p;
          con++;
          con->macnum     = 0;
          con->options    = 0;
          con->call_anyway  = 0;
          con->password[0]  = 0;
          con->sysnum     = StringToUnsignedShort(&(ss[p]));
          con->min_hr     = -1;
          con->max_hr     = -1;
          con->times_per_day  = 0;
          con->min_k      = 0;
          con->call_x_days  = 0;
          break;
        case '&':
          con->options |= options_sendback;
          ++p;
          break;
        case '-':
          con->options |= options_ATT_night;
          ++p;
          break;
        case '_':
          con->options |= options_ppp;
          ++p;
          break;
        case '+':
          con->options |= options_no_call;
          ++p;
          break;
        case '~':
          con->options |= options_receive_only;
          ++p;
          break;
        case '!':
          con->options |= options_once_per_day;
          ++p;
          con->times_per_day = StringToUnsignedChar(&(ss[p]));
          if (!con->times_per_day) {
            con->times_per_day = 1;
          }
          break;
        case '%':
          ++p;
          con->macnum = StringToUnsignedChar(&(ss[p]));
          break;
        case '/':
          ++p;
          con->call_anyway = StringToUnsignedChar(&(ss[p]));
          ++p;
          break;
        case '#':
          ++p;
          con->call_x_days = StringToUnsignedChar(&(ss[p]));
          break;
        case '(':
          ++p;
          con->min_hr = StringToChar(&(ss[p]));
          break;
        case ')':
          ++p;
          con->max_hr = StringToChar(&(ss[p]));
          break;
        case '|':
          ++p;
          con->min_k = StringToUnsignedShort(&(ss[p]));
          if (!con->min_k) {
            con->min_k = 0;
          }
          break;
        case ';':
          ++p;
          con->options |= options_compress;
          break;
        case '^':
          ++p;
          con->options |= options_hslink;
          break;
        case '$':
          ++p;
          con->options |= options_force_ac;
          break;
        case '=':
          ++p;
          con->options |= options_hide_pend;
          break;
        case '*':
          ++p;
          con->options |= options_dial_ten;
          break;
        case '\"': {
          ++p;
          int i = 0;
          while ((i < 19) && (ss[p + static_cast<long>(i)] != '\"')) {
            ++i;
          }
          for (int i1 = 0; i1 < i; i1++) {
            con->password[i1] = ss[ p + static_cast<long>(i1) ];
          }
          con->password[i] = 0;
          p += static_cast<long>(i + 1);
        }
        break;
        }
      }
    }
    free(ss);
  }
}



int bbs_list_net_no = -1;


void zap_bbs_list() {
  if (csn) {
    free(csn);
    csn = nullptr;
  }
  if (csn_index) {
    free(csn_index);
    csn_index = nullptr;
  }
  session()->num_sys_list  = 0;
  bbs_list_net_no     = -1;
}


void read_bbs_list() {
  zap_bbs_list();

  if (net_sysnum == 0) {
    return;
  }
  File fileBbsData(session()->network_directory(), BBSDATA_NET);
  if (fileBbsData.Open(File::modeBinary | File::modeReadOnly)) {
    long lFileLength = fileBbsData.GetLength();
    session()->num_sys_list = static_cast<int>(lFileLength / sizeof(net_system_list_rec));
    csn = static_cast<net_system_list_rec *>(BbsAllocA(lFileLength + 512L));
    for (int i = 0; i < session()->num_sys_list; i += 256) {
      fileBbsData.Read(&(csn[i]), 256 * sizeof(net_system_list_rec));
    }
    fileBbsData.Close();
  }
  bbs_list_net_no = session()->net_num();
}


void read_bbs_list_index() {
  zap_bbs_list();

  if (net_sysnum == 0) {
    return;
  }
  File fileBbsData(session()->network_directory(), BBSDATA_IND);
  if (fileBbsData.Open(File::modeBinary | File::modeReadOnly)) {
    long lFileLength = fileBbsData.GetLength();
    session()->num_sys_list = static_cast<int>(lFileLength / 2);
    csn_index = static_cast<unsigned short *>(BbsAllocA(lFileLength));
    fileBbsData.Read(csn_index, lFileLength);
    fileBbsData.Close();
  } else {
    read_bbs_list();
  }
  bbs_list_net_no = session()->net_num();
}

int system_index(int ts) {
  if (bbs_list_net_no != session()->net_num()) {
    read_bbs_list_index();
  }

  if (csn) {
    for (int i = 0; i < session()->num_sys_list; i++) {
      if (csn[i].sysnum == ts && csn[i].forsys != 65535) {
        return i;
      }
    }
  } else {
    for (int i = 0; i < session()->num_sys_list; i++) {
      if (csn_index[i] == ts) {
        return i;
      }
    }
  }
  return -1;
}


bool valid_system(int ts) {
  return (system_index(ts) == -1) ? false : true;
}


net_system_list_rec *next_system(int ts) {
  static net_system_list_rec csne;
  int nIndex = system_index(ts);

  if (nIndex == -1) {
    return nullptr;
  } else if (csn) {
    return ((net_system_list_rec *) & (csn[nIndex]));
  } else {
    File fileBbsData(session()->network_directory(), BBSDATA_NET);
    fileBbsData.Open(File::modeBinary | File::modeReadOnly);
    fileBbsData.Seek(sizeof(net_system_list_rec) * static_cast<long>(nIndex), File::seekBegin);
    fileBbsData.Read(&csne, sizeof(net_system_list_rec));
    fileBbsData.Close();
    return (csne.forsys == 65535) ? nullptr : (&csne);
  }
}

void zap_contacts() {
  if (session()->current_net().ncn) {
    free(session()->current_net().ncn);
    session()->current_net().ncn = nullptr;
    session()->current_net().num_ncn = 0;
  }
}

void read_contacts() {
  zap_contacts();

  File fileContact(session()->network_directory(), CONTACT_NET);
  if (fileContact.Open(File::modeBinary | File::modeReadOnly)) {
    long lFileLength = fileContact.GetLength();
    session()->current_net().num_ncn = static_cast<int16_t>(lFileLength / sizeof(net_contact_rec));
    if ((session()->current_net().ncn =
           static_cast<net_contact_rec *>(BbsAllocA((session()->current_net().num_ncn + 2) *
           sizeof(net_contact_rec)))) == nullptr) {
      WWIV_ASSERT(session()->current_net().ncn != nullptr);
      session()->AbortBBS(true);
    }
    fileContact.Seek(0L, File::seekBegin);
    fileContact.Read(session()->current_net().ncn,
        session()->current_net().num_ncn * sizeof(net_contact_rec));
    fileContact.Close();
  }
}

void set_net_num(int network_number) {
  if (network_number >= 0 && network_number < session()->max_net_num()) {
    session()->set_net_num(network_number);
    net_sysnum = session()->current_net().sysnum;
    session()->set_net_type(session()->current_net().type);
  }
}

