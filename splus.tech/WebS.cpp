#ifdef _WIN32
#include "WebS.h"
#include <string>
#include <istream>
#include <sstream>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <vector>
#include <iterator>
#include <algorithm>

void WebS::onMessageReceived(int clientSocket, const char* msg, int length) {
	//client's request string e.g. GET /index.htm HTTP/1.1
	std::istringstream iss(msg);
	std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	/* client GET inf
	for (auto ptr : parsed) {
		std::cout << ptr << std::endl;
	}
	*/
	//default 404 for CL if get not OK, otherwise index
	std::string content = "<h1>404 Not Found</h1>";
	std::string htmlFile = ".//index.htm";
	int errorCode = 404;

	// If srv get GET request and its OK
	if (parsed.size() >= 3 && parsed[0] == "GET") {
		htmlFile = parsed[1];
		// parse GET to separate files
		if (htmlFile == "/" || htmlFile == "/index.htm") {
			htmlFile = "index.htm";
		}
		if (htmlFile == "/backgrund.png") {
			htmlFile = "./img/backgrund.png";
		}
		if (htmlFile == "/ipaddr.txt") {
			htmlFile = "ipaddr.txt";
		}
		//root part with files
		std::ifstream f("./" + htmlFile, std::ios::binary);
		// grab files from root to str (error code ok)
		if (f.good()) {
			std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			content = str;
			errorCode = 200;
		}
		f.close();
	}
	// IN THIS BLOCK YOU MAY PLACE YOU FILES 
	// by *.pdf, or hard  -  by names
	//parse get and write down files to client
	std::ostringstream oss;
	if (htmlFile == "index.htm") {
		oss << "HTTP/1.1 " << errorCode << " OK\r\n";
		oss << "Cache-Control: no-cache, private\r\n";
		oss << "Content-Type: text/html\r\n";
		oss << "Content-Length: " << content.size() << "\r\n";
		oss << "\r\n";
		std::ifstream ip_file("ipaddr.txt");

		content.replace(content.find("<!--_IP_-->"), 11, this->get_cl_ip_addrs());
		oss << content;
	}
	if (htmlFile == ".\\img\\backgrund.png") {
		oss << "HTTP/1.1 " << errorCode << " OK\r\n";
		oss << "Cache-Control: no-cache, private\r\n";
		oss << "Content-Type: image/png\r\n";
		oss << "Content-Length: " << content.size() << "\r\n";
		oss << "\r\n";
		oss << content;
	}
	std::string output = oss.str();
	int size = output.size() + 2;
	sendToClient(clientSocket, output.c_str(), size);
}

//CLCONN
void WebS::onClientConnected(int clientSocket) {

}

//CL DISCONN
void WebS::onClientDisconnected(int clientSocket) {

}
#else
#include "WebS.h"
#include <cstring>
#include <string.h>
#include <string>
#include <istream>
#include <sstream>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <vector>
#include <iterator>
#include <algorithm>
using namespace std;



void WebS::onMessageReceived(int clientSocket, const char* msg, size_t /*length*/) {
	//client's request string e.g. GET /index.htm HTTP/1.1
	std::istringstream iss(msg);
	std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	/*
//client GET inf
for (auto ptr : parsed) {
	std::cout << ptr << std::endl;
}
*/
//default 404 for CL if get not OK, otherwise index
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
		if (htmlFile == "/backgrund.png") {
			htmlFile = "./img/backgrund.png";
		}
		if (htmlFile == "/ipaddr.txt") {
			htmlFile = "./ipaddr.txt";
		}
		if (htmlFile == "/PoltavsckiyNCV.pdf") {
			htmlFile = "./img/PoltavsckiyNCV.pdf";
		}
		//root part with files
		std::ifstream f("./" + htmlFile, std::ios::binary);
		// grab files from root to str (error code ok)
		if (f.good()) {
			std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			content = str;
			errorCode = 200;
		}
		f.close();
	}
	// IN THIS BLOCK YOU MAY PLACE YOU FILES 
	// by *.pdf, or hard  -  by names
	//parse get and write down files to client
	std::ostringstream oss;
	if (htmlFile == "index.htm") {
		oss << "HTTP/1.1 " << errorCode << " OK\r\n";
		oss << "Cache-Control: no-cache, private\r\n";
		oss << "Content-Type: text/html\r\n";
		oss << "Content-Length: " << content.size() << "\r\n";
		oss << "\r\n";
		std::ifstream ip_file("ipaddr.txt");
		std::string IP_;
		IP_.assign(this->get_cl_ip_addrs(), 16);
		content.replace(content.find("<!--_IP_-->"), 11, IP_);
		oss << content;
	}
	if (htmlFile == "./img/backgrund.png") {
		oss << "HTTP/1.1 " << errorCode << " OK\r\n";
		oss << "Cache-Control: no-cache, private\r\n";
		oss << "Content-Type: image/png\r\n";
		oss << "Content-Length: " << content.size() << "\r\n";
		oss << "\r\n";
		oss << content;
	}
	std::string output = oss.str();
	size_t size = output.size() + 2;
	sendToClient(clientSocket, output.c_str(), size);
}

//CLCONN
void WebS::onClientConnected(int /*clientSocket*/) {
}

//CL DISCONN
void WebS::onClientDisconnected(int /*clientSocket*/) {
}
#endif
