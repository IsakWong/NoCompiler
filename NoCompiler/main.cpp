#include "Lexer.h"
#include <iostream>
#include <fstream>
#include "SemanticAnalyzer.h"

using namespace std;
int main()
{
	Lexer lexer;
	char buffer[1024];
	fstream file;
	file.open("source.cpp", ios::in);
	int rows[1000];
	rows[0] = 0;
	SemanticAnalyzer analyzer;
	try
	{
		Lexer::TokensVector tok;
		int index = 1;
		string source;
		while (!file.eof())
		{
			file.getline(buffer, sizeof(buffer));  
			source = move(source + string(buffer));
			index++;
		}
		auto tokens = lexer.analyze(source);
		cout << "tokens:  ";
		for (Lexer::token item : tokens)
		{
			tok.push_back(item);
			cout << '<' << item.value << "," << item.type << ">  ";
			cout << endl;
		}
		analyzer.analyze(tokens);
	}
	catch (exception exp)
	{
		cout << endl;
		int i = 0;
		for (; i<1000; i++)
		{
			if (analyzer.index <= rows[i])
			{
				break;
			}
		}
		cout << "分析在" << i -1<<"行处异常中断" << endl;
	}

	return 1;
}
