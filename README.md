# IconFinder
Find DLL files with icons.

## About

This program takes a directory path as param (-d || --directory) and searchs this path with subfolders for .dll files that contain icon resources. The output of the program will be a list of all the files that were found.

If you need to extract or view icons from a DLL found with this program, I recommend the free Windows Explorer extension [IconViewer](https://www.botproductions.com/iconview/iconview.html) from  Bot Productions.

## Usage

```C:\> iconfinder.exe -d "C:\windows\System32"```

## Building

This repo contains the DevC++ project file, along with the source files needed to compile and run.

Use the ```-lwininet``` flag to compile it, as it is necessary if you are using g++. This is because the Windows API functions used in this code are part of the ```wininet``` library.

Also remember to compile the program with a C++17 (or later) compatible compiler due to the usage of ```std::filesystem```. If you are using g++ you can compile it with the flag ```-std=c++17```.

## Docs

### EnumResNameProc
This program uses ```EnumResNameProc```, a callback function that is called by the ```EnumResourceNames``` function. Here’s what the parameters mean:

 #### HMODULE hModule
 A handle to the module whose executable file contains the resources for which the names are being enumerated.

#### LPCSTR lpszType
The type of resource for which the name is being enumerated. In this case, it’s RT_ICON, which means we’re looking for icon resources.

#### LPSTR lpszName
The name of the resource. For predefined resource types, the name is the equivalent string of the predefined constant.

#### LONG_PTR lParam
An application-defined parameter passed to the ```EnumResourceNames``` function. This can be used to pass additional data to the callback function.

In this case, the callback function always returns TRUE, which causes ```EnumResourceNames``` to stop enumerating resources as soon as it finds the first one. This is because we’re only checking if the DLL contains at least one icon.

### LoadLibraryEx
```LoadLibraryEx``` is a function that loads the specified module into the address space of the calling process. The loaded module may either be a .dll or .exe file. The function returns a handle that can be used to get information about the module. Here’s what the parameters mean:

#### entry.path().string().c_str()
The name of the module. This can be either a library module (a .dll file) or an executable module (an .exe file). The name specified is the file name of the module.

#### NULL
This parameter can be used to specify a handle to a module to be loaded. This parameter is typically NULL.

#### LOAD_LIBRARY_AS_DATAFILE 
This value is used to load the module as a data file. When this value is used, the system maps the file into the calling process’s virtual address space as if it were a data file. Nothing is done to execute or prepare to execute the mapped file. Therefore, you can use this flag to load a DLL module without running its initialization code, which can be useful when you just want to extract resources from the DLL.