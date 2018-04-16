#ifndef LEXER_H
#define LEXER_H

#include <string>
#include<tuple>
#include <vector>
#include <map>

using namespace std;

const string keywords[] =
{
	"main",
	"if",
	"then",
	"while",
	"do",
	"static",
	"int",
	"double",
	"struct",
	"break",
	"else",
	"long",
	"switch",
	"case",
	"typedef",
	"char",
	"return",
	"const",
	"float",
	"short",
	"continue",
	"for",
	"void",
	"default",
	"sizeof",

	"=",
	"+",
	"-",
	"*",
	"/",
	"==",
	"<",
	">",
	"<=",
	">=",


	";",
	"\"",
	"(",
	")",
	"{",
	"}",
};

class Lexer
{
public:
	Lexer();
	~Lexer();
public:
	enum LexerState
	{
		ReceivingIdOrKeyword,
		Empty,
		ReceivingOperator,
		ReceivingNumber,
	};
	enum TokenType
	{
		Error = -1,

		//¹Ø¼ü×Ö
		Main = 0,
		If,
		Then,
		While,
		Do,
		Static,
		Int,
		Double,
		Struct,
		Break,
		Else,
		Long,
		Switch,
		Case,
		TypeOf,
		Char,
		Return,
		Const,
		Float,
		Short,
		Continue,
		For,
		Void,
		Default,
		SizeOf = 24,

		//ÔËËã·û
		Assign = 25,
		Plus,
		Subtraction,
		Multiply,
		Divide,
		Equal,
		LessThan,
		MoreThan,
		LessOrEqul,
		MoreOrEqul,
		
		//½ç·û
		Semicolon,//·ÖºÅ
		Quotes,//Ë«Òý
		LeftBrackets,//×óÀ¨ºÅ
		RightBrackets,//ÓÒÀ¨ºÅ
		LeftBigBrackets,//×ó´óÀ¨ºÅ
		RightBigBrackets,//ÓÒ´óÀ¨ºÅ
		Douhao,
		
		//±êÊ¶·û£¬Êý×Ö£¬×Ö·û´®
		ID ,
		Num,
		String
	};


	static constexpr char CharOperator[] =
	{

		'+',
		'-',
		'*',
		'/',
		'=',
		'<',
		'>',
	};
	static constexpr char CharSeperator[] =
	{
		'(',
		')',
		'{',
		'}',
		';',
		','
	};
	class LexerException
	{
	public:
		string type;
	};

	static void error(const string &err);
	static TokenType getTokenType(const string &str);
	static bool isOperatorCh(char ch);
	static bool isLetter(char ch);
	static bool isDigit(char ch);
	static bool isSpace(char ch);
	static bool isSeperator(char ch);

	class token
	{
	public:
		string value;
		int row;
		Lexer::TokenType type;
		token(const string& _val, const Lexer::TokenType _type,int _row) : type(_type), value(_val),row(_row)
		{

		}
	};

	typedef vector< token > TokensVector;

	static TokensVector analyze(const string& source);
	
private:


	
};

#endif

