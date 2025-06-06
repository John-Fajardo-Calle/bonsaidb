#ifndef BONSAIDB_UTILS_HPP
#define BONSAIDB_UTILS_HPP

#include <vector>
#include <string>

namespace Utils {

    inline std::vector<std::string> split(const std::string& text, char delim) {
        std::vector<std::string> result;
        std::string tmp;
        for (char c : text) {
            if (c == delim) {
                if (!tmp.empty()) result.push_back(tmp);
                tmp.clear();
            } else {
                tmp.push_back(c);
            }
        }
        if (!tmp.empty()) result.push_back(tmp);
        return result;
    }

} // namespace Utils

#endif // BONSAIDB_UTILS_HPP