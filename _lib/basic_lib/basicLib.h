#ifndef BASIC_LIB_H__
#define BASIC_LIB_H__

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

#include "vector2.h"

static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

static std::string generateToken(const int length) {
	std::string token;

	for (int i = 0; i < length; i++)
		token += alphanum[rand() % (sizeof(alphanum) - 1)];
	return token;
}

static std::vector<std::string> Split(char* buffer, int byteLen)
{
	std::vector<std::string> elements;
	bool reachedSubItem = false;

	if (buffer[0] == '{' && buffer[byteLen - 1] == '}')
	{
		int start = 1;
		int end = 0;
		do
		{
			if (buffer[start] == '{')
				reachedSubItem = true;
			if (buffer[end] == ';' && !reachedSubItem || end == byteLen - 1)
			{
				std::string temp = "";
				const int tempLen = end - start;
				for (int i = 0; i < tempLen; i++)
					temp += buffer[i + start];
				elements.push_back(temp);
				end++;
				start = end;
			}
			end++;
		} while (end <= byteLen);
	}
	return elements;
}

static std::vector<std::string> Split(std::string& s, int byteLen)
{
	std::vector<std::string> elements;
	bool reachedSubItem = false;

	if (s[0] == '{' && s[byteLen - 1] == '}')
	{
		int start = 1;
		int end = 0;
		do
		{
			if (s[start] == '{')
				reachedSubItem = true;
			if (s[end] == ';' && !reachedSubItem || end == byteLen - 1)
			{
				std::string temp = "";
				const int tempLen = end - start;
				for (int i = 0; i < tempLen; i++)
					temp += s[i + start];
				elements.push_back(temp);
				end++;
				start = end;
			}
			end++;
		} while (end <= byteLen);
	}
	return elements;
}

static bool ToInt(const std::string& s, int& n)
{
	char *end;
	errno = 0;

	n = atoi(s.c_str());
	return true;
	return false;
}

static bool ToPosition(const std::string& s, Position& position)
{
	int byteLen = (int)s.length();
	bool foundY = false;
	if (s[0] == '(' && s[s.length() - 1] == ')')
	{
		int start = 1;
		int end = 0;
		int pos = 0;
		do
		{
			if (s[end] == ':' || end == byteLen - 1)
			{
				std::string temp = "";
				const int tempLen = end - start;
				for (int i = 0; i < tempLen; i++)
					temp += s[i + start];
				if (foundY)
				{
					if (!ToInt(temp, pos))
						return NULL;
					position.x = (float)pos;
					return true;
				}
				else
				{
					if (!ToInt(temp, pos))
						return NULL;
					position.y = (float)pos;
					foundY = true;
				}
				end++;
				start = end;
			}
			end++;
		} while (end <= byteLen);
	}
	return false;
}

static int ToAbsoluteInt(const int& number)
{
	return (number < 0 ? number * -1 : number);
}

static float ToAbsoluteFloat(const float& number)
{
	return (number < 0 ? number * -1 : number);
}

static int RoundToInt(float number)
{
	float temp = (float)(int)number;
	return (number - temp) > 0.5f ? (int)temp + 1 : (int)temp;
}

#endif
