#ifndef	__ISSTRING_H__
#define	__ISSTRING_H__

#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <algorithm>
#include <wchar.h>
#include <vector>
using namespace std;

class IsString
{
public:
	static void TrimLeft(string& str , const char* ch)
	{
		if(str.empty()) return;
		const std::string::size_type begin = str.find_first_not_of(ch);
		const std::string::size_type size  = str.size();
			
		if(std::string::npos == begin)
		{
			str.assign("");
		}
		else if( begin != 0)
		{
			str.assign(str.substr(begin, size));
		}	
	}
	
	static void TrimRight(string& str , const char* ch)
	{
		if(str.empty()) return;
		
		const std::string::size_type end   = str.find_last_not_of(ch) + 1;
		const std::string::size_type size  = str.size();
		
		if(0 == end)
		{
			str.assign("");
		}
		else if(end != size)
		{
			string a = str.substr(0 , end);
			str.assign(a.c_str());
		}
	}

	static void TrimWhiteSpace(string& str)
	{
		if(str.empty()) return;
		string::size_type pos = str.find_last_not_of(' ');
		if(pos != string::npos) 
		{
			str.erase(pos + 1);
			pos = str.find_first_not_of(' ');
			if(pos != string::npos) str.erase(0, pos);
		}
		else str.erase(str.begin(), str.end());

		/*
		while((' ' == (str)[0]) || ('\t' == (str)[0]))
		{
			TrimLeft(str , " ");
			TrimLeft(str , "\t");
		}
		
		while((' ' == (str)[str.length() - 1]) || ('\t' == (str)[str.length() - 1]))
		{
			TrimRight(str , " ");
			TrimRight(str , "\t");
		}
		*/
	}

	static void TrimWhiteSpace(wstring& str)
	{
		if(str.empty()) return;
		string::size_type pos = str.find_last_not_of(' ');
		if(pos != string::npos) 
		{
			str.erase(pos + 1);
			pos = str.find_first_not_of(' ');
			if(pos != string::npos) str.erase(0, pos);
		}
		else str.erase(str.begin(), str.end());

		/*
		while((' ' == (str)[0]) || ('\t' == (str)[0]))
		{
			TrimLeft(str , " ");
			TrimLeft(str , "\t");
		}
		
		while((' ' == (str)[str.length() - 1]) || ('\t' == (str)[str.length() - 1]))
		{
			TrimRight(str , " ");
			TrimRight(str , "\t");
		}
		*/
	}

	static void ReplaceOf(string& str , const string target, const string replacement)
	{
		string::size_type pos = 0, found;

		if (!target.empty())
		{
			string::size_type target_size = target.size();
			string::size_type replacement_size = replacement.size();
			while ((found = str.find (target, pos)) != string::npos)
			{
				str.replace (found , target_size , replacement);
				pos = found + replacement_size;
			}
		}
	}

	static void ToUpper(string& str)
	{
		transform(str.begin() , str.end() , str.begin() , ::toupper);
	}

	static void ToLower(string& str)
	{
		transform(str.begin() , str.end() , str.begin() , ::tolower);
	}

	/**
		@brief	

		@author	BHK

		@date	2009.11.09

		@param

		@reutrn
	*/
	static void Format(string& str , const char* sFormat , ...)
	{
		#define MAX_MSG_LENGTH 2048
		char sMessage[MAX_MSG_LENGTH] = {'\0',};
		va_list args;
		
		str = "";
		/* put formatted message in sMessage */
		::memset(sMessage,0,sizeof(sMessage));
		if(sFormat) 
		{
			va_start(args, sFormat);
			_vsnprintf(sMessage, MAX_MSG_LENGTH - 1, sFormat, args);
			sMessage[MAX_MSG_LENGTH-1] = 0;
			va_end(args);

			str = sMessage;
		}
	}

	/**
		@brief	

		@author	BHK

		@date	2009.11.09

		@param

		@reutrn
	*/
	static int CompareNoCase(const string& lhs , const string& rhs)
	{
		return stricmp(lhs.c_str() , rhs.c_str());
	}

#if _MSC_VER >= 1310
	static std::wstring mbs_to_wcs(std::string const& str, std::locale const& loc = std::locale())
	{
		typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
		codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);
		std::mbstate_t state = std::mbstate_t();
		std::vector<wchar_t> buf(str.size() + 1);
		char const* in_next = str.c_str();
		wchar_t* out_next = &buf[0];
		std::codecvt_base::result r = codecvt.in(state, 
			str.c_str(), str.c_str() + str.size(), in_next, 
			&buf[0], &buf[0] + buf.size(), out_next);
		if (r == std::codecvt_base::error)
			throw std::runtime_error("can't convert string to wstring");   
		return std::wstring(&buf[0]);
	}

	static std::string wcs_to_mbs(std::wstring const& str, std::locale const& loc = std::locale())
	{
		typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
		codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);
		std::mbstate_t state = std::mbstate_t();
		std::vector<char> buf((str.size() + 1) * codecvt.max_length());
		wchar_t const* in_next = str.c_str();
		char* out_next = &buf[0];
		std::codecvt_base::result r = codecvt.out(state, 
			str.c_str(), str.c_str() + str.size(), in_next, 
			&buf[0], &buf[0] + buf.size(), out_next);
		if (r == std::codecvt_base::error)
			throw std::runtime_error("can't convert wstring to string");   
		return std::string(&buf[0]);
	}
#endif
};

#endif
