# FakeFileGenerator

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/22e764eef2dd4c76b1ecd15eab0763fb)](https://www.codacy.com/manual/Harkame/FakeFileGenerator?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Harkame/FakeFileGenerator&amp;utm_campaign=Badge_Grade)
[![Maintainability](https://api.codeclimate.com/v1/badges/3b9d8f24f967cf2ccee2/maintainability)](https://codeclimate.com/github/Harkame/FakeFileGenerator/maintainability)

A bit strange project that replace files content by specified one but keep metadata (size, creation date, etc)

## Requirements

-   Windows
-   g++ 9.2

## Installation

``` bash

make

```

## Usage

``` bash

ffg.exe <fileToCopy> <fileName> <directoryToProcess> <anotherDirectoryToProcess> ... <yetAnotherDirectoryToProcess>

```

fileToCopy : File whose content will replace target files content

fileName : All files whose name contains \<fileName\> will be modified

directoryToProcess, anotherDirectoryToProcess, yetAnotherDirectoryToProcess : Directories that will be browse recursively

### Example

``` bash

ffg.exe file.mp4 Video C:\\Users\\Harkame\\Desktop D:\\Torrents D:\\ProgramData

```

All files in C:\\Users\\Harkame\\Desktop, D:\\Torrents, D:\\ProgramData (recursively) whose name contains « Video » will get their content replaced by file.mp4 content. All modified files will keep their name, file size, creation date, etc.
