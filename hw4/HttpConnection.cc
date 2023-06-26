/*
 * Copyright ©2023 Justin Hsia.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Winter Quarter 2023 for purposes of
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

#include "./HttpRequest.h"
#include "./HttpUtils.h"
#include "./HttpConnection.h"

using std::map;
using std::string;
using std::vector;

namespace hw4 {

static const int read_len = 500;
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

  size_t end_offset = buffer_.find("\r\n\r\n");
  if (end_offset == string::npos) {
    int bytes_read;
    unsigned char buf[read_len];

    while (1) {
      bytes_read = WrappedRead(fd_, buf, read_len);
      if (bytes_read == 0) {
        break;
      } else if (bytes_read == -1) {
        return false;
      } else {
        buffer_ += string(reinterpret_cast<char *>(buf), bytes_read);
        end_offset = buffer_.find("\r\n\r\n");
        if (end_offset != string::npos) {
          break;
        }
      }  // end of else
    }  // end of while loop
  }  // end of if (end == string::npos)

  // now we check again if it is at the end of the string
  if (end_offset == string::npos) {
    return false;
  }

  // Now parse the request
  *request = ParseRequest(buffer_.substr(0, end_offset));

  // clear the buffer
  buffer_ = buffer_.substr(end_offset + kHeaderEndLen);
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

  // parse GET request URI
  vector<string> lines;  // vector of lines from the GET request
  boost::split(lines,
               request,
               boost::is_any_of("\r\n"),
               boost::token_compress_on);
  vector<string> components;  // components of first line
  boost::split(components,
               lines[0],
               boost::is_any_of(" "),
               boost::token_compress_on);

  // check if components follow format: [<method>, <uri>, <version>]
  // if so, set URI to the second component
  if (components.size() == 3) {
    req.set_uri(components[1]);
  }

  // loop through headers
  for (auto line = lines.begin()+1; line != lines.end(); line++) {
    vector<string> header;
    boost::split(header,
                 *line,
                 boost::is_any_of(":"),
                 boost::token_compress_on);
    if (header.size() != 2) {
      continue;
    }
    // parse header name
    string name = header[0];
    boost::algorithm::trim(name);
    boost::to_lower(name);
    // parse header value
    string value = header[1];
    boost::algorithm::trim(value);

    req.AddHeader(name, value);
  }

  return req;
}

}  // namespace hw4

