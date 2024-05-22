# IconFinder
Find DLL files with icons.

## Description
This is a C++ program that searches for DLL files in a specified directory and its subdirectories, and checks if they contain at least one icon. The program prints the paths of all DLL files that contain at least one icon.

If you need to extract or view icons from a DLL found with this program, I recommend the free Windows Explorer extension [IconViewer](https://www.botproductions.com/iconview/iconview.html) from  Bot Productions.

## Usage
You can run the program from the command line like this:

```IconFinder -d <directory>```

or

```IconFinder --directory <directory>```

## Building the Project
The project can be built using any C++ compiler that supports the C++17 standard. It has been tested with DevC++.

This repo contains the DevC++ project file, along with the source file needed to compile and run.

To compile the code, you might use the following command if you're using g++:

```bash
g++ -std=c++17 -o IconFinder main.cpp -lwininet
```

Please note that the ```-std=c++17``` flag is necessary to enable C++17 features. The ```-lwininet``` flag is necessary because the program uses Windows API functions that are part of the ```wininet``` library.

## Documentation
The program uses several functions from the Windows API like described bellow, and also uses features from the C++17 standard, such as the ```<filesystem>``` library to traverse directories.

### LoadLibraryEx
A function that loads the specified module into the address space of the calling process. The loaded module may either be a .dll or .exe file. The function returns a handle that can be used to get information about the module.

```c++
HMODULE LoadLibraryEx(LPCTSTR lpFileName, HANDLE hFile, DWORD dwFlags);
```

* ```LPCTSTR lpFileName```: This is the name of the module. This can be either a library module (a .dll file) or an executable module (an .exe file). The name specified is the file name of the module and not the path. In this program, this parameter is passed as ```entry.path().string().c_str()```.

* ```HANDLE hFile```: This parameter is reserved for future use. It should be NULL. In this program, this parameter is passed as ```NULL```.

* ```DWORD dwFlags```: The action to be taken when loading the module. In this program, this parameter is passed as ```LOAD_LIBRARY_AS_DATAFILE```, which means the system maps the file into the calling process’s virtual address space as if it were a data file.

### EnumResourceNames
A function that enumerates through all resources of a specific type within a binary file.

```c++
BOOL EnumResourceNames(HMODULE hModule, LPCTSTR lpszType, ENUMRESNAMEPROC lpEnumFunc, LONG_PTR lParam);
```

* ```HMODULE hModule```: A handle to a module to begin search. The module must contain the resource table to be searched. In this program, this parameter is passed as ```hLibrary```.

* ```LPCTSTR lpszType```: The type of the resource for which the name is being enumerated. In this program, this parameter is passed as ```RT_ICON```, which means we’re looking for icon resources.

* ```ENUMRESNAMEPROC lpEnumFunc```: A pointer to the callback function to be called for each enumerated resource name. In this program, this parameter is passed as ```EnumResNameProc```.

* ```LONG_PTR lParam```: An application-defined parameter passed to the ```EnumResourceNames``` function. This can be used to pass additional data to the callback function. In this program, this parameter is passed as ```0```.

### EnumResNameProc
A callback function that is called by the EnumResourceNames function.

```c++
BOOL CALLBACK EnumResNameProc(HMODULE hModule, LPCTSTR lpszType, LPTSTR lpszName, LONG_PTR lParam);
```

* ```HMODULE hModule```: A handle to the module whose executable file contains the resources for which the names are being enumerated.

* ```LPCTSTR lpszType```: The type of resource for which the name is being enumerated. In this program, it’s ```RT_ICON```, which means we’re looking for icon resources.

* ```LPTSTR lpszName```: The name of the resource. For predefined resource types, the name is the equivalent string of the predefined constant.

* ```LONG_PTR lParam```: An application-defined parameter passed to the ```EnumResourceNames``` function. This can be used to pass additional data to the callback function.

In this program, the callback function always returns ```TRUE```, which causes ```EnumResourceNames``` to stop enumerating resources as soon as it finds the first one. This is because we’re only interested in whether the DLL contains at least one icon, not in all the icons it might contain.

### FreeLibrary
Frees the loaded dynamic-link library (DLL) module.

```c++
BOOL FreeLibrary(HMODULE hLibModule);
```

* ```HMODULE hLibModule```: A handle to the loaded library module. The ```LoadLibrary```, ```LoadLibraryEx```, ```GetModuleHandle```, or ```GetModuleHandleEx``` function returns this handle.

The ```FreeLibrary``` function decrements the reference count of the loaded dynamic-link library (DLL) module. When the reference count reaches zero, the module is unloaded from the address space of the calling process and the handle is no longer valid.

In this program, this function is used to free the library after it's done checking it for icons. The parameter ```hLibModule``` is passed as ```hLibrary```, which is the handle returned by LoadLibraryEx.

### wininet Library
The ```wininet``` library is a Microsoft Windows library that provides high-level network functions to applications. It allows applications to access standard Internet protocols, such as FTP and HTTP. The library encapsulates the protocol-specific details in easy-to-use functions, enabling developers to create Internet-enabled applications without needing to know the details of the protocols themselves.

In this program, the ```wininet``` library is used indirectly through the ```LoadLibraryEx``` and ```EnumResourceNames``` functions, which are part of the Windows API.

## Important Notes
This program is designed to run on Windows, as it uses Windows-specific API functions. Also, please be aware that attempting to load a DLL can potentially execute code, so ensure you trust the DLLs you are scanning. If you want to avoid this risk, consider using a library designed for safe reading of binary files.

## References

* [Microsoft Docs - LoadLibraryEx function](https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-loadlibraryex)

* [Microsoft Docs - EnumResourceNames function](https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-enumresourcenamesa)

* [Microsoft Docs - FreeLibrary function](https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-freelibrary)

* [Microsoft Docs - Windows Internet (WinINet)](https://docs.microsoft.com/en-us/windows/win32/wininet/about-wininet)