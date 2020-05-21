#include "pch.h"
#include "CharString.h"

CharString::CharString()
{
	init(L"");
}

CharString::CharString(const char c)
{
	wchar_t s[] = { (wchar_t)c,'\0' };
	init(s);
}

CharString::CharString(const wchar_t c)
{
	wchar_t s[] = { c,'\0' };
	init(s);
}

CharString::CharString(const char * str)
{
	wchar_t * temp;
	int len = MultiByteToWideChar(CP_ACP, 0, str, strlen(str), NULL, 0);
	temp = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, str, strlen(str), temp, len);
	temp[len] = '\0';
	init(temp);
	delete[] temp;
}

CharString::CharString(const string & s)
{
	wchar_t * temp;
	int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), strlen(s.c_str()), NULL, 0);
	temp = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), strlen(s.c_str()), temp, len);
	temp[len] = '\0';
	init(temp);
	delete[] temp;
}

CharString::CharString(const wstring & s)
{
	init(s.c_str());
}

CharString::CharString(const wchar_t * str)
{
	init(str);
}

CharString::CharString(const CharString & cs)
{
	init(cs.data);
}

int CharString::getSize()
{
	return length;
}

int CharString::indexOf(const CharString & s)
{
	return indexOf(s.data,s.length);
}

int CharString::indexOf(const wchar_t * s)
{
	int len = wcslen(s);
	return indexOf(s, len);
}

int CharString::indexOf(const wchar_t * s, int slen)
{
	int* Next = new int[slen];
	Next[0] = -1;
	int j1 = 0;
	int k = -1;
	while (j1 < slen - 1)
	{
		if (k == -1 || s[j1] == s[k])
		{
			j1++;
			k++;
			Next[j1] = (s[j1] != s[k] ? k : Next[k]);
		}
		else
			k = Next[k];
	}
	int i = 0, j = 0;
	while (i < length&&j < slen)
	{
		if (j == -1 || data[i] == s[j])
		{
			i++;
			j++;
		}
		else
		{
			j = Next[j];
		}		
	}
	if (j == slen)
	{
		delete[] Next;
		return i - j;
	}
	else
	{
		delete[] Next;
		return -1;
	}
}

int CharString::toint()
{
	int num = 0;
	for (int i = 0; i < wcslen(this->data); i++)
	{
		num = num * 10 + (this->data[i] - L'0');
	}
	return num;
}

CharString CharString::substring(int pos, int len)
{
	CharString substr;
	if (pos >= 0 && pos < length)
	{
		if (len + pos > length)
			len = length - pos;

		wchar_t* str = new wchar_t[len + 1];

		wcsncpy(str, data + pos, len);
		str[len] = '\0';
		substr = str;
		delete[] str;
	}
	return substr;
}

CharString & CharString::operator=(const CharString & s)
{
	return (*this = s.data);
}

CharString & CharString::operator=(const wchar_t * s)
{
	if (data != s)
	{
		delete[] data;
		init(s);
	}
	return *this;
}

void CharString::concat(const CharString & str)
{
	wchar_t* p = new wchar_t[length + wcslen(str.data) + 1];
	if (str.data != NULL)
	{
		wcscpy(p, data);
		wcscat(p, str.data);
		length = wcslen(p);
		delete[] data;
		init(p);
		delete[] p;
	}
}

void CharString::concat(const wchar_t * str)
{
	wchar_t* p = new wchar_t[length + wcslen(str) + 1];
	if (str != NULL)
	{
		wcscpy(p, data);
		wcscat(p, str);
		length = wcslen(p);
		delete[] data;
		init(p);
		delete[] p;
	}
}

void CharString::removeBlank()
{
	CharString s = "";
	for (int i = 0; i < length; i++)
	{
		wchar_t c = this->data[i];
		if (!isBlank(c))
		{
			s += c;
		}
	}
	this->init(s.data);
}

wchar_t & CharString::operator[](int index)
{
	return data[index];
}

wchar_t CharString::operator[](int index) const
{
	return data[index];
}

CharString & CharString::operator+=(const CharString & s)
{
	concat(s);
	return *this;
}

CharString & CharString::operator+=(const wchar_t * str)
{
	concat(str);
	return *this;
}

bool CharString::operator==(const CharString & s)
{
	if (length != s.length)
		return false;
	wchar_t *p1 = data; 
	wchar_t *p2 = s.data;
	for (int i = 0; i < length; i++)
	{
		if ((*p1) != (*p2))
			return false;
		p1++;
		p2++;
	}
	return true;
}

bool CharString::operator!=(const CharString & s)
{
	return !(*this==s);
}

bool CharString::operator<(const CharString & s)
{
	if (wcscmp(this->data, s.data) < 0)
		return true;
	else
		return false;
}

bool CharString::operator>(const CharString & s)
{
	if (wcscmp(this->data, s.data) > 0)
		return true;
	else
		return false;
}

wchar_t* CharString::getData()
{
	return this->data;
}


void CharString::init(const wchar_t * s)
{
	if (s == NULL)
		data = NULL;
	length = wcslen(s);
	data = new wchar_t[length + 1];
	wcscpy(data, s);
}


