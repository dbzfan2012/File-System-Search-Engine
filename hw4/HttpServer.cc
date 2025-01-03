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

#include <boost/algorithm/string.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <string>
#include <sstream>

#include "./FileReader.h"
#include "./HttpConnection.h"
#include "./HttpRequest.h"
#include "./HttpUtils.h"
#include "./HttpServer.h"
#include "./libhw3/QueryProcessor.h"

using std::cerr;
using std::cout;
using std::endl;
using std::list;
using std::map;
using std::string;
using std::stringstream;
using std::unique_ptr;

namespace hw4 {
///////////////////////////////////////////////////////////////////////////////
// Constants, internal helper functions
///////////////////////////////////////////////////////////////////////////////
static const char* kThreegleStr =
  "<html><head><title>333gle</title></head>\n"
  "<body>\n"
  "<center style=\"font-size:500%;\">\n"
  "<span style=\"position:relative;bottom:-0.33em;color:orange;\">3</span>"
    "<span style=\"color:red;\">3</span>"
    "<span style=\"color:gold;\">3</span>"
    "<span style=\"color:blue;\">g</span>"
    "<span style=\"color:green;\">l</span>"
    "<span style=\"color:red;\">e</span>\n"
  "</center>\n"
  "<p>\n"
  "<div style=\"height:20px;\"></div>\n"
  "<center>\n"
  "<form action=\"/query\" method=\"get\">\n"
  "<input type=\"text\" size=30 name=\"terms\" />\n"
  "<input type=\"submit\" value=\"Search\" />\n"
  "</form>\n"
  "</center><p>\n";

// static
const int HttpServer::kNumThreads = 100;

// This is the function that threads are dispatched into
// in order to process new client connections.
static void HttpServer_ThrFn(ThreadPool::Task* t);

// Given a request, produce a response.
static HttpResponse ProcessRequest(const HttpRequest& req,
                            const string& base_dir,
                            const list<string>& indices);

// Process a file request.
static HttpResponse ProcessFileRequest(const string& uri,
                                const string& base_dir);

// Process a query request.
static HttpResponse ProcessQueryRequest(const string& uri,
                                 const list<string>& indices);


///////////////////////////////////////////////////////////////////////////////
// HttpServer
///////////////////////////////////////////////////////////////////////////////
bool HttpServer::Run(void) {
  // Create the server listening socket.
  int listen_fd;
  cout << "  creating and binding the listening socket..." << endl;
  if (!socket_.BindAndListen(AF_INET6, &listen_fd)) {
    cerr << endl << "Couldn't bind to the listening socket." << endl;
    return false;
  }

  // Spin, accepting connections and dispatching them.  Use a
  // threadpool to dispatch connections into their own thread.
  cout << "  accepting connections..." << endl << endl;
  ThreadPool tp(kNumThreads);
  while (1) {
    HttpServerTask* hst = new HttpServerTask(HttpServer_ThrFn);
    hst->base_dir = static_file_dir_path_;
    hst->indices = &indices_;
    if (!socket_.Accept(&hst->client_fd,
                    &hst->c_addr,
                    &hst->c_port,
                    &hst->c_dns,
                    &hst->s_addr,
                    &hst->s_dns)) {
      // The accept failed for some reason, so quit out of the server.
      // (Will happen when kill command is used to shut down the server.)
      break;
    }
    // The accept succeeded; dispatch it.
    tp.Dispatch(hst);
  }
  return true;
}

static void HttpServer_ThrFn(ThreadPool::Task* t) {
  // Cast back our HttpServerTask structure with all of our new
  // client's information in it.
  unique_ptr<HttpServerTask> hst(static_cast<HttpServerTask*>(t));
  cout << "  client " << hst->c_dns << ":" << hst->c_port << " "
       << "(IP address " << hst->c_addr << ")" << " connected." << endl;

  // Read in the next request, process it, and write the response.
  bool done = false;
  HttpConnection connection(hst->client_fd);
  while (!done) {
    // Get the next request, break if error
    HttpRequest request;
    if (!connection.GetNextRequest(&request)) {
      break;
    }

    // Process the request and write it
    HttpResponse response = ProcessRequest(request,
                                          hst->base_dir,
                                          *(hst->indices));
    if (!connection.WriteResponse(response)) {
      break;
    }

    // If it received a close connection, set to done
    if (request.GetHeaderValue("connection") == "close") {
      done = true;
    }
  }
}

static HttpResponse ProcessRequest(const HttpRequest& req,
                            const string& base_dir,
                            const list<string>& indices) {
  // Is the user asking for a static file?
  if (req.uri().substr(0, 8) == "/static/") {
    return ProcessFileRequest(req.uri(), base_dir);
  }

  // The user must be asking for a query.
  return ProcessQueryRequest(req.uri(), indices);
}

static HttpResponse ProcessFileRequest(const string& uri,
                                const string& base_dir) {
  // The response we'll build up.
  HttpResponse ret;

  string file_name = "";

  URLParser parser;
  parser.Parse(uri);
  file_name.append(parser.path());
  if (file_name.find("/static/") != file_name.npos) {
    file_name = file_name.substr(8);
  }

  // Read file into body
  std::string body;
  FileReader reader(base_dir, file_name);
  if (reader.ReadFile(&body)) {
    ret.AppendToBody(body);

    // Set the content type based on the file extension
    string extension = file_name.substr(file_name.find("."));
    if (extension == ".html" || extension == ".htm") {
      ret.set_content_type("text/html");
    } else if (extension == ".jpeg" || extension == ".jpg") {
      ret.set_content_type("image/jpeg");
    } else if (extension == ".png") {
      ret.set_content_type("image/png");
    } else if (extension == ".txt") {
      ret.set_content_type("text/plain");
    } else if (extension == ".js") {
      ret.set_content_type("text/js");
    } else if (extension == ".css") {
      ret.set_content_type("text/css");
    } else if (extension == ".xml") {
      ret.set_content_type("text/xml");
    } else if (extension == ".gif") {
      ret.set_content_type("image/gif");
    }

    ret.set_protocol("HTTP/1.1");
    ret.set_message("OK");
    ret.set_response_code(200);

    return ret;
  }

  // If you couldn't find the file, return an HTTP 404 error.
  ret.set_protocol("HTTP/1.1");
  ret.set_response_code(404);
  ret.set_message("Not Found");
  ret.AppendToBody("<html><body>Couldn't find file \""
                   + EscapeHtml(file_name)
                   + "\"</body></html>\n");
  return ret;
}

static HttpResponse ProcessQueryRequest(const string& uri,
                                 const list<string>& indices) {
  // The response we're building up.
  HttpResponse ret;

  // Set up the basic 333gle page
  ret.AppendToBody(kThreegleStr);

  // Get the query terms
  stringstream ss;
  if (uri.find("/query?terms=") != uri.npos) {
    URLParser parser;
    parser.Parse(uri);

    // Get the query words
    string path = parser.path();
    string query = parser.args().at("terms");
    boost::to_lower(query);
    boost::trim(query);

    // Escape the query so cross-site scripting cannot occur
    query = EscapeHtml(query);

    // Split the query and call the Query Processor
    std::vector<string> query_words;
    boost::split(query_words, query, boost::is_any_of(" "),
                                     boost::token_compress_on);

    hw3::QueryProcessor qp(indices);
    vector<hw3::QueryProcessor::QueryResult> res = qp.ProcessQuery(query_words);

    // Parse through the results and add them to the page
    // with hyperlinks
    if (res.size() == 0) {
      ss << "<p><br>\nNo results found for <b>" << query << "</b>\n<p>";
    } else {
      ss << "<p><br>\n" << res.size()
         << " results found for <b>"
         << query << "</b>\n<p>\n";
      ss << "\n<ul>\n";
      string message = "";
      for (auto query_result : res) {
        ss << " <li> <a href=\"";
        if (query_result.document_name.find("http://") ==
            query_result.document_name.npos) {
          ss << "/static/" << query_result.document_name << "\">"
             << query_result.document_name << "</a> ["
             << query_result.rank << "]<br>\n";
        } else {
          ss << query_result.document_name << "\">"
             << query_result.document_name << "</a> ["
             << query_result.rank << "]<br>\n";
        }
      }
      ss << "</ul>\n";
    }
  }
  ss << "</body>\n</html>\n";
  ret.AppendToBody(ss.str());

  ret.set_protocol("HTTP/1.1");
  ret.set_response_code(200);
  ret.set_message("OK");

  return ret;
}

}  // namespace hw4
