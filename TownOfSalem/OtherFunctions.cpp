#include <string>
#include <fstream>
#include <windows.h>
#include <sstream>
#include <string>
#include <codecvt>
#include <iostream>
#include "resource.h"
#include "OtherFunctions.h"

std::wstring get_random_line_rc(WORD file_name) {
	//wchar_t content = GetResource(file_name);

	std::wstring input(GetResource(file_name));
	std::wstringstream ss(input);
	std::wstring line;
	int randomint = rand() % 5000+1;
	while (std::getline(ss, line, L'\n')) {
		if (randomint <= 1) {
			break;
		}
		else {
			randomint--;
		}
	}
	line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
	return line;
}

std::string get_random_line(std::string file_name) {
	std::ifstream file(file_name);
	std::string line;
	file.seekg(0, std::ios::end);
	int length = file.tellg();
	file.seekg(0, std::ios::beg);
	for (int i = 1; i <= rand() % 3; i++)
		std::getline(file, line);
	//file_name.clear();
	//file_name.seekg(0, std::ios::beg);
	return line;
}



std::string wstringToString(const std::wstring& input) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes(input);
}

void LoadFileInResource(int name, int type, DWORD& size, const wchar_t*& data) // *& is passing the pointer by reference and not by val.
{
	HMODULE handle = ::GetModuleHandleW(NULL);
	HRSRC rc = ::FindResourceW(handle, MAKEINTRESOURCEW(name), MAKEINTRESOURCEW(type));
	HGLOBAL rcData = ::LoadResource(handle, rc);
	size = ::SizeofResource(handle, rc);
	data = static_cast<const wchar_t*>(::LockResource(rcData));
	//LockResource does not actually lock memory; it is just used to obtain a pointer to the memory containing the resource data. 
}

const wchar_t* GetResource(WORD file_name)
{
	DWORD size = 0;
	const wchar_t* data = NULL;
	LoadFileInResource(file_name, TXT, size, data);
	/* Access bytes in data - here's a simple example involving text output*/
	// The text stored in the resource might not be NULL terminated.
	wchar_t* buffer = new wchar_t[size + 1];
	::memcpy(buffer, data, size);
	buffer[size] = 0; // NULL terminator
	delete[] buffer;
	return data;
}

