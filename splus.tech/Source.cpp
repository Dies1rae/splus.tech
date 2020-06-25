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
			WebS TiKKColorServer(IP.c_str(), port);
			if (TiKKColorServer.init() != 0) {
				return 0;
			}
			cout << "Server started" << endl;
			TiKKColorServer.run();
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
		cerr << "Error RTFM!" << endl;
		return 0;
	}
	return 0;
}