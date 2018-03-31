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
	while(!file.eof())
	{
		file.getline(buffer, sizeof(buffer));
		string source(buffer);
		cout << source << "     tokens:";
		Lexer::token_vector tokens = lexer.Analyze(source);
		for (tuple<basic_string<char>, Lexer::token_type> item : tokens)
		{
			cout << '<' << std::get<0>(item) << "," << std::get<1>(item) << ">  ";
		}
		cout << endl;
		
	}
	return 1;
}
