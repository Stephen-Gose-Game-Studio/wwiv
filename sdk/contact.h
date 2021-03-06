/**************************************************************************/
/*                                                                        */
/*                          WWIV Version 5.x                              */
/*             Copyright (C)2015-2016 WWIV Software Services              */
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
/**************************************************************************/
#ifndef __INCLUDED_SDK_CONTACT_H__
#define __INCLUDED_SDK_CONTACT_H__

#include <ctime>
#include <initializer_list>
#include <map>
#include <string>
#include <vector>

#include "sdk/net.h"

namespace wwiv {
namespace sdk {

 /**
  * Class for manipulating CONTACT.NET
  */
class Contact {
 public:
  Contact(const std::string& network_dir, bool save_on_destructor);
  // VisibleForTesting
  Contact(std::initializer_list<net_contact_rec> l);
  virtual ~Contact();

  // Was this list initialized properly.
  bool IsInitialized() const { return initialized_; }
  // returns a mutable net_contact_rec for system number "node"
  net_contact_rec* contact_rec_for(int node);

  /** add a connection to node, including bytes send and received. */
  void add_connect(int node, time_t time, uint32_t bytes_sent, uint32_t bytes_received);
  /** add a failure attempt to node */
  void add_failure(int node, time_t time);

  bool Save();
  std::string ToString() const;

 private:
   /** add a contact. called by connect or failure. */
   void add_contact(net_contact_rec* c, time_t time);

   std::string network_dir_;
   bool save_on_destructor_;
   std::vector<net_contact_rec> contacts_;
   bool initialized_;
};


}  // namespace net
}  // namespace wwiv

#endif  // __INCLUDED_SDK_CONTACT_H__
