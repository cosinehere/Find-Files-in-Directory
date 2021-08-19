#include <cstdio>

#include "FileFinds.hpp"

int main() {
    std::set<std::string> files;
    filefinds::FileFinds(".", files, true, nullptr, true);

    auto it = files.begin();
    for (; it != files.end(); ++it) {
        printf("%s\n", it->c_str());
    }
    getchar();
    return 0;
}

