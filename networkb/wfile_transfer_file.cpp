/**************************************************************************/
/*                                                                        */
/*                          WWIV Version 5.x                              */
/*             Copyright (C)2015-2016, WWIV Software Services             */
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
#include "networkb/wfile_transfer_file.h"

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <string>

#include "core/log.h"
#include "core/strings.h"

using std::chrono::seconds;
using std::chrono::system_clock;
using std::clog;
using std::endl;
using std::string;
using wwiv::strings::StrCat;
using wwiv::strings::StringPrintf;

namespace wwiv {
namespace net {

WFileTransferFile::WFileTransferFile(const string& filename,
	  std::unique_ptr<File>&& file)
  : TransferFile(filename, file->Exists() ? file->last_write_time() : time(nullptr)), file_(std::move(file)) {
  if (filename.find(File::pathSeparatorChar) != string::npos) {
    // Don't allow filenames with slashes in it.
    throw std::invalid_argument("filename can not be relative pathed");
  }
}

WFileTransferFile::~WFileTransferFile() {}

int WFileTransferFile::file_size() const {
  return file_->GetLength();
}

bool WFileTransferFile::Delete() {
  return file_->Delete();
}

bool WFileTransferFile::GetChunk(char* chunk, size_t start, size_t size) {
  if (!file_->IsOpen()) {
    if (!file_->Open(File::modeBinary | File::modeReadOnly)) {
      return false;
    }
  }
  
  if (static_cast<int>(start + size) > file_size()) {
    LOG << "ERROR WFileTransferFile::GetChunk (start + size) > file_size():"
        << "values[ start: " << start << "; size: " << size
	 << "; file_size(): " << file_size() << " ]";
    return false;
  }

  // TODO(rushfan): Cache the current file pointer and only re-seek
  // if needed (realistically we should ever have to seek after the
  // first time.
  file_->Seek(start, File::seekBegin);
  return file_->Read(chunk, size) == size;
}

bool WFileTransferFile::WriteChunk(const char* chunk, size_t size) {
  if (!file_->IsOpen()) {
    if (file_->Exists()) {
      // Don't overwrite an existing file.  Rename it away to: FILENAME.timestamp
      string timestamp = std::to_string(system_clock::to_time_t(system_clock::now()));
      File::Rename(file_->full_pathname(), StrCat(file_->full_pathname(), timestamp));
    }
    if (!file_->Open(File::modeBinary | File::modeReadWrite | File::modeCreateFile)) {
      return false;
    }
  }
  int num_written = file_->Write(chunk, size);
  return num_written == size;
}

bool WFileTransferFile::Close() {
  file_->Close();
  return true;
}


}  // namespace net
} // namespace wwiv
