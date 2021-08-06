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

void FileFinds(const char* directory, std::set<std::string>& files,
               bool recursive) {
    std::string format = directory;
    std::queue<std::string> pathque;
    pathque.push(format);

    while (!pathque.empty()) {
        std::string path = pathque.front();
        pathque.pop();

#ifdef _WIN32
        std::string pathfind = path + "\\*";

        WIN32_FIND_DATA find;
        HANDLE hfind = FindFirstFile(pathfind.c_str(), &find);
        if (hfind == INVALID_HANDLE_VALUE) {
            continue;
        }

        do {
            if (!strcmp(find.cFileName, ".") || !strcmp(find.cFileName, "..")) {
                continue;
            }

            std::string fullpath = path + "\\" + find.cFileName;

            if (find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (recursive) {
                    pathque.push(fullpath);
                }
            } else {
                files.insert(fullpath);
            }
        } while (FindNextFile(hfind, &find));
        FindClose(hfind);

#else
        DIR* dir;
        dir = opendir(path.c_str());
        if (dir == nullptr) {
            continue;
        }

        struct dirent* file;
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
#endif
    }
}

};  // namespace filefinds
