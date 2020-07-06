#ifdef _WIN32
#include <string>
#include <iostream>
#include <vector>
#include "WebS.h"
#include <algorithm>
#include "menu.h"
using namespace std;
#else
#include "WebS.h"
#include <iostream>
#include <string>
using namespace std;
#endif

#ifdef _WIN32
int main(int argc, char* argv[]) {
	if (argc > 1 && argv[1][0] == '/') {
		if (argv[1][1] == 's') {
			string IP = argv[2];
			uint16_t port = stoi(argv[3]);
			WebS siteserv(IP.c_str(), port);
			if (siteserv.init() != 0) {
				return 1;
			}
			cout << "Server started" << endl;
			siteserv.run();
			system("pause");
		}
		if (argv[1][1] == '?') {
			help_menu_view();
		}
		if (argv[1][1] == 'm') {
			man_menu_view();
		}
	}
	else {
		string IP = "0.0.0.0";
		uint16_t port = 80;
		WebS siteserv(IP.c_str(), port);
		if (siteserv.init() != 0) {
			return 1;
		}
		cout << "Server started" << endl;
		siteserv.run();
		system("pause");
	}
	mainS.run();
}
#else
int main() {
    string _ip = "0.0.0.0";
    uint16_t _port = 80;
    WebS mainS(_ip.c_str(), _port);
    cout << "Server started" << endl;
    if (mainS.init() != 0) {
        cout << "Some error with server INIT() " << mainS.init() << endl;
        return 1;
    }
    mainS.run();
}
#endif
