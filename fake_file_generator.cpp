#include <fstream>
#include <iostream>
#include <filesystem>
#include <filesystem>
#include <stdio.h>

using namespace std;

namespace fs = filesystem;

int main()
{
  for (const auto& dirEntry : fs::recursive_directory_iterator("."))
    cout << dirEntry << endl;
  fs::remove("./copy_image.png");
  fs::copy("./image.png", "./copy_image.png");

/*
  FILE* pFile;
  pFile = fopen("copy_image.png", "w");
  for(int i = 0; i < 300000; i++)
  {
  //Some calculations to fill a[]
    //fwrite("0", 1, sizeof(char), pFile);
  }

  fclose(pFile);
  */
  ofstream myfile;
  myfile.open("./copy_image.png", ios::binary | std::ios_base::app);

    if (myfile.is_open())
    {
        for(long i = 0; i < 3000000000; i++)
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
  return 0;
}
