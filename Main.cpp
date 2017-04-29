#include <string>
#include <iostream>
#include <vector>
#include <Windows.h>

#include "Helper.h"

using namespace std;

void pwd()
{
	CHAR buffer[_MAX_PATH + 1];
	if (!GetCurrentDirectoryA(_MAX_PATH + 1, buffer))
	{
		cout << "GetCurrentDirectoryA Error: " << GetLastError() << endl;
		system("pause");
		exit(1);
	}
	cout << buffer << endl;
}

void cd(string path)
{
	if (!SetCurrentDirectoryA(path.c_str()))
	{
		cout << "SetCurrentDirectoryA Error: " << GetLastError() << endl;
		system("pause");
		exit(1);
	}
}

void create(string name)
{
	HANDLE h = CreateFile(name.c_str(), GENERIC_READ,NULL,NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,NULL);
	int e = GetLastError();
	if (e != 0 && e != 183) // 183 = File already exists
	{
		cout << "CreateFile Error: " << e << endl;
		system("pause");
		exit(1);
	}
	if (!CloseHandle(h))
	{
		cout << "CloseHandle Error: " << GetLastError() << endl;
		system("pause");
		exit(1);
	}
}

void ls(string name)
{
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	HANDLE f = INVALID_HANDLE_VALUE;
	DWORD e = 0;

	name += "\\*";

	f = FindFirstFile(name.c_str(), &ffd);
	if (f == INVALID_HANDLE_VALUE)
	{
		cout << "FindFirstFile Error: " << GetLastError() << endl;
		system("pause");
		exit(1);
	}

	// List all the files in the directory with some info about them.

	do
		cout << ffd.cFileName << ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? "/" : "") << endl;
	while (FindNextFile(f, &ffd) != 0);

	e = GetLastError();
	if (e != ERROR_NO_MORE_FILES)
	{
		cout << "FindNextFile Error: " << e << endl;
		system("pause");
		exit(1);
	}

	FindClose(f);
}

void secret()
{
	HINSTANCE lib = LoadLibrary("Secret.dll");
	int(*proc)() = NULL;

	if (lib == NULL)
	{
		cout << "LoadLibrary Error: " << GetLastError() << endl;
		system("pause");
		exit(1);
	}
	proc = (int(*)())GetProcAddress(lib, "TheAnswerToLifeTheUniverseAndEverything");
	if (proc != NULL)
		cout << "The answer to life, the universe and everything is " << proc() << endl;
	else
		cout << "GetProcAddress Error: " << GetLastError() << endl;

	if (!FreeLibrary(lib))
	{
		cout << "FreeLibrary Error: " << GetLastError() << endl;
		system("pause");
		exit(1);
	}
}

void run(string file)
{
	STARTUPINFO info = { sizeof(info) };
	PROCESS_INFORMATION processInfo;
	if (!CreateProcess(file.c_str(), "", NULL, NULL, NULL, NULL, NULL, NULL, &info, &processInfo))
	{
		cout << "CreateProcess Error: " << GetLastError() << endl;
		system("pause");
		exit(1);
	}
	WaitForSingleObject(processInfo.hProcess, INFINITE);
	int e = GetLastError();
	if (e)
	{
		cout << "WaitForSingleObject Error: " << e << endl;
		system("pause");
		exit(1);
	}    
	if (!GetExitCodeProcess(processInfo.hProcess, (LPDWORD)&e))
	{
		cout << "GetExitCodeProcess Error: " << e << endl;
		system("pause");
		exit(1);
	}
	cout << "Exit code: " << e << endl;
	if (!CloseHandle(processInfo.hProcess) || !CloseHandle(processInfo.hThread))
	{
		cout << "CloseHandle Error: " << e << endl;
		system("pause");
		exit(1);
	}

}

void main()
{
	vector<string> cmd;
	string buffer;
	while (1)
	{
		cout << "> ";
		getline(cin, buffer);
		cmd = Helper::get_words(buffer);

		if (cmd.size() == 0) continue;
		if (cmd[0] == "pwd") pwd();
		else if (cmd[0] == "cd")
			if (cmd.size() >= 2) cd(cmd[1]);
			else cout << "Usage: cd [path]" << endl;
		else if (cmd[0] == "create")
			if (cmd.size() >= 2) create(cmd[1]);
			else cout << "Usage: create [file name]" << endl;
		else if (cmd[0] == "ls")
			if (cmd.size() >= 2) ls(cmd[1]);
			else ls(".");
		else if (cmd[0] == "secret") secret();
		else if (cmd[0] == "run")
			if (cmd.size() >= 2) run(cmd[1]);
			else cout << "Usage: run [file name]" << endl;
		else if (cmd[0] == "quit") break;
		else cout << "Unknown command!" << endl;
	}
}