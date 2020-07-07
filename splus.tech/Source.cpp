#include "WebS.h"
#include "menu.h"
#include <iostream>
#include <string>
#include <string_view>

namespace {
    std::string_view IP = "0.0.0.0";
    uint16_t port = 80;
}

int main(int argc, char* argv[]) {
    std::vector<std::string_view> const args(argv, argv+argc);

    if (count(begin(args), end(args), "/?")) {
        help_menu_view(std::cout, args.front());
        return 0;
    }
    if (count(begin(args), end(args), "/m")) {
        man_menu_view(std::cout, args.front());
        return 0;
    }

    auto shift = [&args](auto& it) {
        if (it != end(args))
            std::advance(it, 1);
        if (it == end(args)) 
            throw std::runtime_error("Missing option argument");
        return *it;
    };

    if (auto it = find(begin(args), end(args), "/s"); end(args) != it) {
        IP = shift(it);
        port = static_cast<unsigned short>(atoi(shift(it).data()));
    }

    WebS mainS(IP.data(), port);
    if (auto code = mainS.init()) {
        return code;
    }
    std::clog << "Server started" << std::endl;
	mainS.run();
}
