#include "Util.h"

namespace splus {
    static auto constexpr npos = std::string::npos;

    bool replace(std::string& text, std::string_view what, std::string_view by) {
        if (auto pos = text.find(what); pos != npos) {
            text.replace(pos, what.size(), by);
            return true;
        }
        return false;
    }

    size_t replaceAll(std::string& text, std::string_view what, std::string_view by) {
        size_t pos = 0, n = 0;
        while ((pos = text.find(what, pos)) != npos) {
            text.replace(pos, what.size(), by);
            ++n; 
            pos += 1; // no cycles please
        }
        return n;
    }

    std::string_view guess_content_type(std::string_view path) {
        if (path.find(".pdf")  != npos) return "application/pdf";
        if (path.find(".PDF")  != npos) return "application/pdf";
        if (path.find(".png")  != npos) return "image/png";
        if (path.find(".jpeg") != npos) return "image/jpeg";
        if (path.find(".txt")  != npos) return "text/plain";
        return "text/html";
    }
}

