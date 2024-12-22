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

#include <stdint.h>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <map>
#include <string>
#include <vector>
#include <iostream>

#include "./HttpRequest.h"
#include "./HttpUtils.h"
#include "./HttpConnection.h"

#define kbufSize 1024

using std::map;
using std::string;
using std::vector;

namespace hw4 {

static const char* kHeaderEnd = "\r\n\r\n";
static const int kHeaderEndLen = 4;

bool HttpConnection::GetNextRequest(HttpRequest* const request) {
  // Use WrappedRead from HttpUtils.cc to read bytes from the files into
  // private buffer_ variable. Keep reading until:
  // 1. The connection drops
  // 2. You see a "\r\n\r\n" indicating the end of the request header.
  //
  // Hint: Try and read in a large amount of bytes each time you call
  // WrappedRead.
  //
  // After reading complete request header, use ParseRequest() to parse into
  // an HttpRequest and save to the output parameter request.
  //
  // Important note: Clients may send back-to-back requests on the same socket.
  // This means WrappedRead may also end up reading more than one request.
  // Make sure to save anything you read after "\r\n\r\n" in buffer_ for the
  // next time the caller invokes GetNextRequest()!

  // STEP 1:

  // Loop through and read from the request
  bool read = false;
  while (1) {
    unsigned char request_str[kbufSize];
    if (read) {
      int written = WrappedRead(fd_, request_str, kbufSize);
      if (written == 0) {
        break;
      }
      if (written == -1) {
        if ((errno == EAGAIN) || (errno == EINTR)) {
          continue;
        }
        return false;
      }
      buffer_.append(reinterpret_cast<char*>(request_str), written);
    }

    // Find the header ending
    if (buffer_.find(std::string(kHeaderEnd)) == std::string::npos) {
      read = true;
      continue;
    }

    // Parse the buffer into a header and parse it
    std::string header =
                buffer_.substr(0, buffer_.find(std::string(kHeaderEnd)));
    buffer_ =
    buffer_.substr(buffer_.find(std::string(kHeaderEnd)) + kHeaderEndLen);
    *request = ParseRequest(header);
    break;
  }
  return true;
}

bool HttpConnection::WriteResponse(const HttpResponse& response) const {
  string str = response.GenerateResponseString();
  int res = WrappedWrite(fd_,
                         reinterpret_cast<const unsigned char*>(str.c_str()),
                         str.length());
  if (res != static_cast<int>(str.length()))
    return false;
  return true;
}

HttpRequest HttpConnection::ParseRequest(const string& request) const {
  HttpRequest req("/");  // by default, get "/".

  // Plan for STEP 2:
  // 1. Split the request into different lines (split on "\r\n").
  // 2. Extract the URI from the first line and store it in req.URI.
  // 3. For the rest of the lines in the request, track the header name and
  //    value and store them in req.headers_ (e.g. HttpRequest::AddHeader).
  //
  // Hint: Take a look at HttpRequest.h for details about the HTTP header
  // format that you need to parse.
  //
  // You'll probably want to look up boost functions for:
  // - Splitting a string into lines on a "\r\n" delimiter
  // - Trimming whitespace from the end of a string
  // - Converting a string to lowercase.
  //
  // Note: If a header is malformed, skip that line.

  // STEP 2:

  // Splits header components
  std::vector<std::string> headers;
  boost::split(headers, request, boost::is_any_of("\r\n"),
                                 boost::token_compress_on);

  // Should be more than 1 line
  if (headers.size() < 1) {
    return req;
  }

  // Gets and parses uri, removes from headers
  std::vector<std::string> get_uri;
  std::string delim = " ";
  boost::split(get_uri, headers.front(), boost::is_any_of(" "),
                                         boost::token_compress_on);

  // Should have 3 components
  if (get_uri.size() != 3) {
    return req;
  }

  // Sets the uri and removes it from headers
  req.set_uri(get_uri[1]);
  headers.erase(headers.begin());

  // Adds headers to req one by one
  for (auto& header : headers) {
    if (header == "") {
      continue;
    }

    // Splits header on comma
    std::vector<std::string> key_value;
    boost::split(key_value, header, boost::is_any_of(":"),
                                    boost::token_compress_on);

    // Pulls out the key
    std::string key = key_value.front();
    boost::trim(key);
    boost::algorithm::to_lower(key);

    // Pulls out the value
    std::string value = key_value.back();
    boost::trim(value);
    boost::algorithm::to_lower(value);

    req.AddHeader(key, value);
  }

  return req;
}

}  // namespace hw4
