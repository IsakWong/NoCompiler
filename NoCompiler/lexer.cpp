
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
bool Lexer::isDigit(const char ch)
{
	return ch >= '0' && ch <= '9';
}
bool Lexer::isSpace(const char ch)
{
	return ch == ' ';
}
//判断是否为定界符等
bool Lexer::isSeperator(const char ch)
{
	for (int i = 0; i < 9; i++)
	{
		if (ch == CharSeperator[i])
			return true;
	}
	return false;
}
//判断是否为字母 
bool Lexer::isLetter(const char ch)
{
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}
//判断是否为关键字 
Lexer::TokenType Lexer::getTokenType(const string &str)
{
	for (TokenType type = TokenType::Main; type <= TokenType::String;)
	{
		if (str == keywords[type])
		{
			return type;
		}
		type = static_cast<TokenType>(type + 1);
	}
	return TokenType::ID;
}


bool Lexer::isOperatorCh(const char ch)
{
	for (char i : CharOperator)
	{
		if (ch == i)
		{
			return true;
		}
	}
	return false;
}

Lexer::TokensVector Lexer::analyze(const string& source) throw(Lexer::LexerException)
{
	TokensVector tokens;

	int row = 0;

	LexerState _state = LexerState::Empty;
	string _tmp;
	auto&& it = source.begin();
	while (it != source.end())
	{

		switch (_state)
		{
		case LexerState::Empty:
		{
			_tmp.assign("");
			if (*it == '\r'  )
			{
				row++; 
				++it;
				if (*it == '\n')
					++it;
				break;
			}
			if (isSpace(*it)  || *it == '\t')
			{
				++it;
				break;
			}
			if (isLetter(*it))
			{
				_state = LexerState::ReceivingIdOrKeyword;
				_tmp.push_back(*it);
				++it;
				break;
			}
			if (isDigit(*it))
			{
				_state = LexerState::ReceivingNumber;
				_tmp.push_back(*it);
				++it;
				break;
			}
			if (isOperatorCh(*it))
			{
				_state = LexerState::ReceivingOperator;
				_tmp.push_back(*it);
				++it;
				break;
			}
			if (isSeperator(*it))
			{
				_tmp.push_back(*it);
				TokenType type = getTokenType(_tmp);
				tokens.emplace_back(_tmp, type,row);
				_tmp.clear();
				++it;
				break;
			}
			LexerException exp;
			cout << "错误！未定义的符号 " << *it << endl;
			throw exp;
			break;
		}
		case LexerState::ReceivingIdOrKeyword:
			if (isLetter(*it))
			{
				_tmp.push_back(*it);
				++it;
			}
			else
			{
				TokenType _type = getTokenType(_tmp);
				if (_type == TokenType::ID)
				{
					tokens.emplace_back(_tmp, TokenType::ID,row);
				}
				else
				{
					tokens.emplace_back(_tmp, _type,row);

				}
				_tmp.clear();
				_state = LexerState::Empty;
			}
			break;
		case LexerState::ReceivingNumber:
			if (isDigit(*it))
			{
				_tmp.push_back(*it);
				++it;
			}
			else
			{
				tokens.emplace_back(_tmp, TokenType::Num,row);
				_tmp.clear();
				_state = LexerState::Empty;
			}
			break;
		case LexerState::ReceivingOperator:
			if (isOperatorCh(*it))
			{
				_tmp.push_back(*it);
			}
			else
			{
				TokenType type = getTokenType(_tmp);
				if (type<TokenType::Assign || type> TokenType::LessOrEqul)
				{
					error("运算符出错");
					system("pause");
					break;
				}
				else
				{
					tokens.emplace_back(_tmp, type,row);
					_tmp.clear();
					_state = LexerState::Empty;
				}

			}
		}
	}
	return tokens;
}

