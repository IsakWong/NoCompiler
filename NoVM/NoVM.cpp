// NoVM.cpp: 定义控制台应用程序的入口点。
//

#include "NoVM.h"

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
		cout << "符号表" << endl;
		int i = 0;
		for (auto item : analyzer.symbols)
		{

			cout << i << ":" << item.name << endl;
			++i;
		}
		analyzer.showQuadruples();




	}
	catch (exception exp)
	{
		cout << endl;
	}
}

