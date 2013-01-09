#include <stdio.h>
#include <ctype.h>
#include <windows.h>
#include <shlwapi.h>

void rolling_caps(wchar_t *pathname)
{
    wchar_t *fname = pathname;
    for (wchar_t *p = pathname; *p != 0; p++) {
        if(*p == '/' || *p == '\\') {
            fname = p + 1;
        }
    }

    for (int caps = 0; *fname != 0; fname++, caps = !caps) {
        if(caps != 0) {
            *fname = towupper(*fname);
        }
        else {
            *fname = towlower(*fname);
        }
    }
}

void enum_rolling_caps(const wchar_t *dirname)
{
    WIN32_FIND_DATAW wfd; wchar_t path[MAX_PATH], path2[MAX_PATH];
    PathCombineW(path, dirname, L"*.*");
    HANDLE search_handle = FindFirstFileW(path, &wfd);
    if(search_handle != INVALID_HANDLE_VALUE) {
        do {
            if(wcscmp(wfd.cFileName, L".") && wcscmp(wfd.cFileName, L"..")) {
                if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    PathCombineW(path, dirname, wfd.cFileName);
                    enum_rolling_caps(path);
                }
                else {
                    PathCombineW(path, dirname, wfd.cFileName);
                    wcscpy(path2, path);
                    wcscat(path2, L"_");

                    MoveFileW(path, path2);
                    rolling_caps(path);
                    MoveFileW(path2, path);
                }
            }
        } while (FindNextFileW(search_handle, &wfd));
        FindClose(search_handle);
    }
}

int main(int argc, char *argv[])
{
    enum_rolling_caps(L"C:\\");
}
