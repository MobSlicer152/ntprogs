# Windows Native API demos
This repository contains some nice demos of using the native API on Windows, including these:

- [process](process/process.c): uses NtCreateUserProcess to run `calc.exe`. Works on Windows 11.
- [template](template/template.c): a template `mainCRTStartup` function

There is also a support library that implements the security cookie functions and eventually other
ones if needed.

## Build instructions
There is a Visual Studio 2022 solution with some hand edited project files. The [mknew.py](mknew.py)
script can make new projects, which can then be added to the solution. You need the WDK because there
are additional exports and some support code in `ntdllp.lib` which is part of the WDK.

## Libraries included
The phnt headers from [Process Hacker](https://github.com/winsiderss/systeminformer) are (obviously)
used for their massive wealth of definitions and declarations.

There are also import libraries generated from Windows libraries such as `win32u.dll`.

