#include "Util.h" // for guess_content_type, replaceAll
#include "WebS.h"
#include <ext/alloc_traits.h> // for __alloc_traits<>::value_type
#include <istream>            // for operator<<, basic_ostream, ostringstream
#include <iterator>           // for istreambuf_iterator, istream_iterator
#include <string>             // for string, char_traits, operator==, alloc...
#include <vector>             // for vector
#include <sstream>            // for istringstream
#include <fstream>            // for ifstream

void WebS::onMessageReceived(client& c, std::string_view msg) {
    // client's request string e.g. GET /index.htm HTTP/1.1
    std::istringstream iss;
    iss.str(std::string(msg));
    std::vector<std::string> parsed(std::istream_iterator<std::string>(iss), {});
    // default 404 for CL if get not OK, otherwise index
    std::string content = "<h1>404 Not Found</h1>";
    std::string htmlFile = "./index.htm";
    int errorCode = 404;

    // If srv get GET request and its OK
    if (parsed.size() >= 3 && parsed[0] == "GET") {
        htmlFile = parsed[1];
        // parse GET to separate files

        if (htmlFile == "/" || htmlFile == "/index.htm") {
            htmlFile = "index.htm";
        }
        if (htmlFile.size() && htmlFile.front() == '/')
            htmlFile.insert(0, 1, '.');
        if (htmlFile.find("..") != std::string::npos || htmlFile.find(":") != std::string::npos) {
            errorCode = 401;
            content = "<h1>Not allowed</h1>";
        }
        // root part with files
        std::ifstream f(htmlFile, std::ios::binary);
        // grab files from root to str (error code ok)
        if (f.good()) {
            std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
            content = str;
            errorCode = 200;
        }
        f.close();
    } else {
        errorCode = 400;
        content = "<h1>Bad request</h1>";
    }

    // IN THIS BLOCK YOU MAY PLACE YOU FILES
    // by *.pdf, or hard  -  by names
    // parse get and write down files to client
    splus::replaceAll(content, "<!--_IP_-->", c.peer_ip_address);
    std::string const output = [&] {
        std::ostringstream oss;
        oss << "HTTP/1.1 " << errorCode << " OK\r\n";
        oss << "Content-Type: " << splus::guess_content_type(htmlFile) << "\r\n";
        oss << "Cache-Control: no-cache, private\r\n";
        oss << "Content-Length: " << content.size() << "\r\n";
        oss << "\r\n";
        oss << content;
        return oss.str();
    }();

    sendToClient(c, output);
}

void WebS::onClientConnected(client& /*c*/) {}
void WebS::onClientDisconnected(client& /*c*/) {}
