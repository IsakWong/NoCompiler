#ifndef LEXER_H
#define LEXER_H

#include <string>
#include<tuple>
#include <vector>
#include <map>

using namespace std;


const string keyword_string[] =
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
	enum state
	{
		receiving_id_or_keyword,
		empty,
		receiving_operator,
		receiving_num,
	};
	enum token_type
	{
		Error = -1,

		//�ؼ���
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

		//�����
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
		
		//���
		Semicolon,//�ֺ�
		Quotes,//˫��
		LeftBrackets,//������
		RightBrackets,//������
		LeftBigBrackets,//�������
		RightBigBrackets,//�Ҵ�����
		
		//��ʶ�������֣��ַ���
		ID ,
		Num,
		String
	};
	map<string,token_type> ss={
	}

	const string keyword_string[] =
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
	static constexpr char operator_char[] =
	{

		'+',
		'-',
		'*',
		'/',
		'=',
		'<',
		'>',
	};
	static constexpr char seprator_char[] =
	{
		'(',
		')',
		'{',
		'}',
		';',
	};


	static void error(const string &err);
	static token_type get_token_type(const string &str);
	static bool is_operator_char(char ch);
	static bool is_letter_char(char ch);
	static bool is_digit_char(char ch);
	static bool is_space_char(char ch);
	static bool is_seprator(char ch);

	typedef vector< tuple<string, token_type> > token_vector;

	static token_vector Analyze(const string& source);
	
private:


	
};
#endif

