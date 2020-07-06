#include "Util.h"

namespace splus {
    bool replace(std::string& text, std::string_view what, std::string_view by) {
        if (auto pos = text.find(what); pos != std::string::npos) {
            text.replace(pos, what.size(), by);
            return true;
        }
        return false;
    }

    size_t replaceAll(std::string& text, std::string_view what, std::string_view by) {
        size_t pos = 0, n = 0;
        while ((pos = text.find(what, pos)) != std::string::npos) {
            text.replace(pos, what.size(), by);
            ++n; 
            pos += 1; // no cycles please
        }
        return n;
    }
}

