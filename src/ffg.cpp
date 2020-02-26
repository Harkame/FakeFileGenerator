#include <fstream>
#include <iostream>
#include <filesystem>
#include <filesystem>
#include <stdio.h>
#include <windows.h>
#include <string>
#include <set>

using namespace std;

namespace fs = filesystem;

ifstream::pos_type GetFileSize(const char* filename)
{
  ifstream in(filename, ifstream::ate | ifstream::binary);
  return in.tellg();
}

string GetLastErrorAsString()
{
  DWORD errorMessageID = ::GetLastError();
  if(errorMessageID == 0)
  return string(); //No error message has been recorded

  LPSTR messageBuffer = nullptr;
  size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
     NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

  string message(messageBuffer, size);

  LocalFree(messageBuffer);

  return message;
}

/*
ffg.exe file_to_copy file_name_to_replace directory_1 directory_2 ...
*/
int main(int argc, char** argv)
{
  if(argc < 4)
  {
    cerr << "Missing parameters" << endl;
    return 1;
  }

  set<string> checkedElements;

  string filePattern = string(argv[2]);
  while(1)
  {
    for(int index = 3; index < argc; index++)
    {
      char* directory = argv[index];

      if(!fs::exists(directory))
        continue;

      for (auto& dirEntry: fs::recursive_directory_iterator(directory))
      {
        if (!dirEntry.is_regular_file())
          continue;

        fs::path file = dirEntry.path();

        FILETIME ftCreate;
        FILETIME ftAccess;
        FILETIME ftWrite;

        HANDLE hFile1;
        HANDLE hFile2;

        char fileName[512];

        wcstombs(fileName, file.c_str(), sizeof(fileName));

        string currentFileStr = string(fileName);

        if (checkedElements.find(currentFileStr) != checkedElements.end())
        {
          cout << "Already trated : " << currentFileStr << endl;
          continue;
        }

        if (currentFileStr.find(filePattern) == string::npos)
          continue;

        checkedElements.insert(fileName);

        char tempFileName[512];
        strcpy(tempFileName, fileName);
        strcat(tempFileName, string(".temp").c_str());

        LPCSTR copyFileName = argv[1];

        fs::remove(tempFileName);
        fs::copy(copyFileName, tempFileName);

        ofstream myfile1;

        long sourceFileSize = GetFileSize(fileName);
        long fileSize = GetFileSize(copyFileName);

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
        myfile.open(tempFileName, ios::binary | ios_base::app);

        long diffSize = sourceFileSize - fileSize;

        if (myfile.is_open())
        {
          for(long i = 0; i < diffSize; i++)
            myfile << "0";

          myfile.flush();
          myfile.close();
        }
        else
        {
          cerr << "didn't write" << endl;
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
          cout << GetLastErrorAsString() << endl;
          return 1;
        }
      }
    }

    Sleep(10000);
  }
}
