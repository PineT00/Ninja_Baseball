#pragma once

namespace Utils
{
	class MyString
	{
	public :
		static std::string WideStringToString(const std::wstring& wstr);
		static std::wstring StringToWideString(const std::string& str);
	};
}