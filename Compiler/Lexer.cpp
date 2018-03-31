
#include "Lexer.h"
#include <iostream>


Lexer::Lexer()
= default;


Lexer::~Lexer()
= default;

void Lexer::error(const string& err)
{
	cout << err;
}

//判断是否为数字 
bool Lexer::is_digit_char(char ch)
{
	return ch >= '0' && ch <= '9';
}
bool Lexer::is_space_char(char ch)
{
	return ch == ' ';
}
//判断是否为定界符等
bool Lexer::is_seprator(char ch)
{
	for (int i = 0; i<9; i++)
	{
		if (ch == seprator_char[i])
			return true;
	}
	return false;
}
//判断是否为字母 
bool Lexer::is_letter_char(char ch)
{
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}
//判断是否为关键字 
Lexer::token_type Lexer::get_token_type(const string &str)
{
	for (token_type type = token_type::Main; type<=token_type::String;)
	{
		if (str == keyword_string[type])
		{
			return type;
		}
		type = static_cast<token_type>(type + 1);
	}
	return token_type::ID;
}


bool Lexer::is_operator_char(char ch)
{
	for (char i : operator_char)
	{
		if(ch == i)
		{
			return true;
		}
	}
	return false;
}

Lexer::token_vector Lexer::Analyze(const string& source)
{
	token_vector tokens;

	state _state = state::empty;
	string _tmp;
	auto&& it = source.begin();
	while (it != source.end())
	{

		switch (_state)
		{
		case state::empty:
			_tmp.assign("");
			if (is_space_char(*it) || *it == '\r'||*it == '\n'|| *it =='\t')
			{
				++it;
				break;
			}
			if (is_letter_char(*it))
			{
				_state = state::receiving_id_or_keyword;
				_tmp.push_back(*it);
				++it;
				break;
			}
			if (is_digit_char(*it))
			{
				_state = state::receiving_num;
				_tmp.push_back(*it);
				++it;
				break;
			}
			if(is_operator_char(*it))
			{
				_state = state::receiving_operator;
				_tmp.push_back(*it);
				++it;
				break;
			}
			if(is_seprator(*it))
			{
				_tmp.push_back(*it);
				token_type type = get_token_type(_tmp);
				tokens.emplace_back(_tmp, type);
				_tmp.clear();
				++it;	
				break;
			}
			error("读取到非法字符:" + *it + _tmp);
			break;
		case state::receiving_id_or_keyword:
			if (is_letter_char(*it))
			{
				_tmp.push_back(*it);
				++it;
			}else
			{
				token_type _type = get_token_type(_tmp);
				if (_type == token_type::ID)
				{
					tokens.emplace_back(_tmp, token_type::ID);
				}
				else
				{
					tokens.emplace_back(_tmp, _type);
					
				}
				_tmp.clear();
				_state = state::empty;
			}
			break;
		case state::receiving_num:
			if (is_digit_char(*it))
			{
				_tmp.push_back(*it);
				++it;
			}else
			{
				tokens.emplace_back(_tmp, token_type::Num);
				_tmp.clear();
				_state = state::empty;
			}
			break;
		case state::receiving_operator:
			if(is_operator_char(*it))
			{
				_tmp.push_back(*it);
			}else
			{
				token_type type = get_token_type(_tmp);
				if(type<token_type::Assign || type> token_type::LessOrEqul)
				{
					error("运算符出错");
					system("pause");
					break;
				}else
				{
					tokens.emplace_back(_tmp, type);
					_tmp.clear();
					_state = state::empty;
				}
				
			}
		}
	}
	return tokens;
}

