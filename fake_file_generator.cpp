#include <fstream>
#include <iostream>
#include <filesystem>
#include <filesystem>
#include <stdio.h>
#include <windows.h>
#include <string>

using namespace std;

namespace fs = filesystem;

std::ifstream::pos_type GetFileSize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

string GetLastErrorAsString()
{
    DWORD errorMessageID = ::GetLastError();
    if(errorMessageID == 0)
        return std::string(); //No error message has been recorded

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    std::string message(messageBuffer, size);

    LocalFree(messageBuffer);

    return message;
}


int main()
{
  for (auto& dirEntry: std::filesystem::recursive_directory_iterator("xcover")) {
    if (!dirEntry.is_regular_file()) {
      //std::cout << "Directory: " << dirEntry.path() << std::endl;
      continue;
    }
    std::filesystem::path file = dirEntry.path();
    //std::cout << "Filename: " << file.filename() << " extension: " << file.extension() << std::endl;

    FILETIME ftCreate;
    FILETIME ftAccess;
    FILETIME ftWrite;

    HANDLE hFile1;
    HANDLE hFile2;

    auto temp = file.c_str();

    wcout << temp << endl;

    char fileName[512];

    wcstombs(fileName, temp, sizeof(fileName));

    LPCSTR tempFileName = (string(fileName) + string(".temp")).c_str();
    LPCSTR copyFileName = "image.jpg";

    cout << "fileName : " << fileName << endl;
    cout << "tempFileName : " << tempFileName << endl;

    fs::remove(tempFileName);
    fs::copy(copyFileName, tempFileName);

    ofstream myfile1;

    long sourceFileSize = GetFileSize(fileName);
    long fileSize = GetFileSize(copyFileName);

    cout << fileName << endl;
    cout << tempFileName << endl;
    cout << fileSize << endl;

    hFile1 = CreateFile(fileName,                // name of the write
                        GENERIC_READ,          // open for writing
                        0,                      // do not share
                        NULL,                   // default security
                        OPEN_EXISTING,             // create new file only
                        FILE_ATTRIBUTE_NORMAL,  // normal file
                        NULL);                  // no attr. template

    if(!GetFileTime(hFile1, &ftCreate, &ftAccess, &ftWrite))
    {
      printf("Something wrong!\n");
      cout << GetLastErrorAsString() << endl;
      return FALSE;
    }

    ofstream myfile;
    myfile.open(tempFileName, ios::binary | std::ios_base::app);

    long diffSize = sourceFileSize - fileSize;

    if (myfile.is_open())
    {
      for(long i = 0; i < diffSize; i++)
      {
        myfile << "0";
      }
      myfile.flush();
      myfile.close();
    }
    else
    {
      std::cerr << "didn't write" << std::endl;
    }

    hFile2 = CreateFile(tempFileName,                // name of the write
                        GENERIC_WRITE,          // open for writing
                        0,                      // do not share
                        NULL,                   // default security
                        OPEN_EXISTING,             // create new file only
                        FILE_ATTRIBUTE_NORMAL,  // normal file
                        NULL);                  // no attr. template


    if(!SetFileTime(hFile2, &ftCreate, &ftAccess, &ftWrite))
    {
      printf("Something wrong!\n");
      cout << GetLastErrorAsString() << endl;
      return FALSE;
    }

    myfile.close();
   }
}
