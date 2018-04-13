#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "Lexer.h"
#include <string>
#include <vector>
using namespace std;

class SemanticAnalyzer
{
public:
	class symbol
	{
		string name;	
	};
	typedef vector<symbol> symbol_list;


	union operand
	{
		int jump_index;
		int symbol_index;
	};
	enum operator_type
	{
		JP,
		JNZ,
		Multiply,
		Plus,
		Assign
	};
	class quadruple
	{
		operand first;
		operand second;
		operator_type op;
		operand result;
	};
	SemanticAnalyzer();
	~SemanticAnalyzer();
	static void analyze(const Lexer::token_vector& vector);

private:
	
};

#endif