#include <cstdio>
#include <cstring>

#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#endif  // defined(_WIN32)

#include <string>
#include <set>
#include <queue>

namespace filefinds {

void filefinds(const char* directory, std::set<std::string>& files,
               bool recursive) {
#ifdef _WIN32
#else
    struct stat st;
    lstat(directory, &st);
    if (!S_ISDIR(st.st_mode)) {
        return;
    }

    std::string format = directory;
    std::queue<std::string> pathque;
    pathque.push(format);

    while (!pathque.empty()) {
        std::string path = pathque.front();
        pathque.pop();

        struct dirent* file;
        DIR* dir;
        dir = opendir(path.c_str());
        if (dir == nullptr) {
            continue;
        }

        while ((file = readdir(dir)) != nullptr) {
            if (!strcmp(file->d_name, ".") || !strcmp(file->d_name, "..")) {
                continue;
            }

            std::string fullpath = path + "/" + file->d_name;
            struct stat st;
            lstat(fullpath.c_str(), &st);

            if (!S_ISDIR(st.st_mode)) {
                files.insert(fullpath);
            } else if (recursive) {
                pathque.push(fullpath);
            }
        }
    }

#endif
}

};  // namespace filefinds
