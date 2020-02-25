# FakeFileGenerator

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/22e764eef2dd4c76b1ecd15eab0763fb)](https://www.codacy.com/manual/Harkame/FakeFileGenerator?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Harkame/FakeFileGenerator&amp;utm_campaign=Badge_Grade)
[![Maintainability](https://api.codeclimate.com/v1/badges/3b9d8f24f967cf2ccee2/maintainability)](https://codeclimate.com/github/Harkame/FakeFileGenerator/maintainability)

A bit strange project that replace all files by another one but keep all metadata

## Requirements

-   Windows
-   g++ 17

## Installation

``` bash

make

```

## Usage

``` bash

ffg.exe file.mp4 Video directory_1 directory_2 ...

```

It will replace all files in directory_1 and directory_2 with that name contains « Video » by file.mp4 content
