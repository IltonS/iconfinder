/***************************************************************************************\
 * MIT License
 * 
 * Copyright (c) 2024 Ilton Sequeira
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *  
 \**************************************************************************************/

/*
	Program Name:
	
		InconFinder
	
	Abstract:
		
		This program takes a directory path as param (-d || --directory) and searchs this
		path with subfolders for .dll files that contain icon resources. The output of the
		program will be a list of all the files that were found.
	
	Author:
	
		Ilton Sequeira 16-May-2024
  
 */

#include <iostream>
#include <filesystem>
#include <windows.h>

using namespace std;
using namespace filesystem;

//
// Callback function that is called by the EnumResourceNames function.
// It always returns TRUE, which causes EnumResourceNames to stop enumerating resources as soon as it finds the first one.
//
BOOL CALLBACK EnumResNameProc(HMODULE hModule, LPCSTR lpszType, LPSTR lpszName, LONG_PTR lParam)
{
    return TRUE;
}

//
// This function takes a directory path, and search folder, and subfolder, for .dll files that contain icons
//
void search_dlls(const path& path)
{
    if (!exists(path))
	{
        cout << "Path does not exist\n";
        return;
    }

    for (const auto& entry : recursive_directory_iterator(path))
	{
        if (entry.is_regular_file() && entry.path().extension() == ".dll")
		{
            HMODULE hLibrary = LoadLibraryEx(entry.path().string().c_str(), NULL, LOAD_LIBRARY_AS_DATAFILE);
            
			if (hLibrary != NULL)
			{
                if (EnumResourceNames(hLibrary, RT_ICON, EnumResNameProc, 0))
				{
                    cout << entry.path().string() << " contains at least one icon.\n";
                } else {
                    DWORD dwError = GetLastError();
                    
					if (dwError != ERROR_RESOURCE_TYPE_NOT_FOUND)
					{
                        cout << "Error enumerating resources in " << entry.path().string() << ": " << dwError << '\n';
                    } else {
                        //cout << entry.path().string() << " does not contain any icons.\n";
                    }
                }
                
                FreeLibrary(hLibrary);
            } else {
                DWORD dwError = GetLastError();
                
				cout << "Error loading " << entry.path().string() << ": " << dwError << '\n';
            }
        }
    }
}

//
// Program Entry point
//
int main(int argc, char* argv[])
{
    path path;

    for (int i = 1; i < argc; ++i)
	{
        string arg = argv[i];
        
        if ((arg == "-d") || (arg == "--directory"))
		{
            if (i + 1 < argc) // Make sure we aren't at the end of argv!
			{ 
                path = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
            } else { // Uh-oh, there was no argument to the destination option.
                cerr << "--directory option requires one argument.\n";
                return 1;
            }  
        } 
    }

    if (path.empty()) {
        cout << "Please provide a directory with -d or --directory option\n";
        return 1;
    }

    search_dlls(path);

    return 0;
}