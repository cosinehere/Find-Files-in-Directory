#pragma once

#ifndef _FILEFINDS_HPP_
#define _FILEFINDS_HPP_

#include <cstring>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(linux)
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#else
#error unsupport platform
#endif // defined(_WIN32)

#include <string>
#include <set>
#include <queue>

#include <regex>

namespace filefinds {

using std::queue;
using std::regex;
using std::set;
using std::string;

bool FileFinds(const char *root, set<string> &files, bool recursive,
               const char *filter, bool include) {
    string format = root;
    queue<string> pathque;
    pathque.push(format);

    regex re;
    try {
        re = filter;
    } catch (const std::regex_error &e) {
        return false;
    }

    while (!pathque.empty()) {
        string path = pathque.front();
        pathque.pop();

#ifdef _WIN32
        string pathfind = path + "\\*";

        WIN32_FIND_DATA find;
        HANDLE hfind = FindFirstFile(pathfind.c_str(), &find);
        if (hfind == INVALID_HANDLE_VALUE) {
            continue;
        }

        do {
            if (!strcmp(find.cFileName, ".") || !strcmp(find.cFileName, "..")) {
                continue;
            }

            string fullpath = path + "\\" + find.cFileName;
            bool match = regex_search(fullpath.c_str(), re);

            if (find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (recursive) {
                    pathque.push(fullpath);
                }
            } else {
                if (!(include ^ match)) {
                    files.insert(fullpath);
                }
            }
        } while (FindNextFile(hfind, &find));
        FindClose(hfind);

#else
        DIR *dir;
        dir = opendir(path.c_str());
        if (dir == nullptr) {
            continue;
        }

        struct dirent *file;
        while ((file = readdir(dir)) != nullptr) {
            if (!strcmp(file->d_name, ".") || !strcmp(file->d_name, "..")) {
                continue;
            }

            string fullpath = path + "/" + file->d_name;
            bool match = regex_search(fullpath.c_str(), re);
            struct stat st;
            lstat(fullpath.c_str(), &st);

            if (!S_ISDIR(st.st_mode)) {
                if (!(include ^ match)) {

                    files.insert(fullpath);
                }
            } else if (recursive) {
                pathque.push(fullpath);
            }
        }
#endif
    }

    return true;
}

}; // namespace filefinds

#endif // _FILEFINDS_HPP_

