/*
 * Copyright ©2024 Hannah C. Tang.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Spring Quarter 2024 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include "./FileIndexReader.h"

#include <sys/types.h>  // for stat()
#include <sys/stat.h>   // for stat()
#include <unistd.h>     // for stat()

extern "C" {
  #include "libhw1/CSE333.h"
}
#include "./Utils.h"    // for class CRC32.

using std::string;

namespace hw3 {

FileIndexReader::FileIndexReader(const string& file_name,
                                 bool validate) {
  // Stash a copy of the index file's name.
  file_name_ = file_name;

  // Open a (FILE*) associated with filename.  Crash on error.
  file_ = fopen(file_name_.c_str(), "rb");
  Verify333(file_ != nullptr);

  setvbuf(file_, NULL, _IONBF, 0);

  // Read the entire file header and convert to host format.
  fseek(file_, 0, SEEK_SET);
  Verify333(fread(&header_, sizeof(header_), 1, file_) == 1);
  header_.ToHostFormat();

  // Verify that the magic number is correct.  Crash if not.
  Verify333(header_.magic_number == kMagicNumber);

  // Make sure the index file's length lines up with the header fields.
  struct stat f_stat;
  Verify333(stat(file_name_.c_str(), &f_stat) == 0);
  Verify333(
    f_stat.st_size == static_cast<unsigned int>(
      sizeof(IndexFileHeader) + header_.doctable_bytes + header_.index_bytes));

  if (validate) {
    // Re-calculate the checksum, make sure it matches that in the header.
    CRC32 crc_obj;
    static constexpr int kBufSize = 512;
    uint8_t buf[kBufSize];
    int left_to_read = header_.doctable_bytes + header_.index_bytes;
    while (left_to_read > 0) {
      int read = fread(buf, 1, kBufSize, file_);
      for (int i = 0; i < read; i++) {
        crc_obj.FoldByteIntoCRC(buf[i]);
      }
      left_to_read -= read;
    }
    Verify333(crc_obj.GetFinalCRC() == header_.checksum);
  }
}

FileIndexReader::~FileIndexReader() {
  // Close the (FILE*).
  Verify333(fclose(file_) == 0);
}

DocTableReader* FileIndexReader::NewDocTableReader() const {
  // The docid->name mapping starts at offset sizeof(IndexFileHeader) in
  // the index file.  Be sure to dup the (FILE*) rather than sharing
  // it across objects, just so that we don't end up with the possibility
  // of threads contending for the (FILE*) and associated with race
  // conditions.
  FILE* fdup = FileDup(file_);
  IndexFileOffset_t file_offset = sizeof(IndexFileHeader);
  return new DocTableReader(fdup, file_offset);
}

IndexTableReader* FileIndexReader::NewIndexTableReader() const {
  // The index (word-->docid table) mapping starts at offset
  // (sizeof(IndexFileHeader) + doctable_size_) in the index file.  Be
  // sure to dup the (FILE*) rather than sharing it across objects,
  // just so that we don't end up with the possibility of threads
  // contending for the (FILE*) and associated race conditions.
  return new IndexTableReader(FileDup(file_),
                              sizeof(IndexFileHeader) + header_.doctable_bytes);
}

}  // namespace hw3
