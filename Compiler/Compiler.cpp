#include "Lexer.h"
#include <iostream>
#include <fstream>

using namespace std;
int main()
{
	Lexer lexer;
	char buffer[1024];
	fstream file;
	file.open("source.cpp", ios::in);
	try
	{
		while (!file.eof())
		{
			file.getline(buffer, sizeof(buffer));
			string source(buffer);
			cout << source;
			Lexer::token_vector tokens = lexer.Analyze(source);
			cout << "tokens:  ";
			for (tuple<basic_string<char>, Lexer::token_type> item : tokens)
			{
				cout << '<' << std::get<0>(item) << "," << std::get<1>(item) << ">  ";
			}
			cout << endl;
		}
	}
	catch (Lexer::lexer_exception exp)
	{
		cout << endl;
		cout << "词法分析异常中断" << endl;
	}

	return 1;
}
