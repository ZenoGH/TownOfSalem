#pragma once
std::string get_random_line(std::string file_name);
std::wstring get_random_line_rc(WORD file_name);
void LoadFileInResource(int, int, DWORD&, const wchar_t*&);
const wchar_t* GetResource(WORD);
std::string wstringToString(const std::wstring&);