#include <string>
#include <iostream>
#include <vector>
#include "WebS.h"
#include <algorithm>
#include "menu.h"
using namespace std;


int main(int argc, char* argv[]) {
	if (argc > 1 && argv[1][0] == '/') {
		if (argv[1][1] == 's') {
			string IP = argv[2];
			int port = stoi(argv[3]);
			WebS siteserv(IP.c_str(), port);
			if (siteserv.init() != 0) {
				return 0;
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
		int port = 80;
		WebS siteserv(IP.c_str(), port);
		if (siteserv.init() != 0) {
			return 0;
		}
		cout << "Server started" << endl;
		siteserv.run();
		system("pause");
	}
	return 0;
}