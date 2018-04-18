#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "Lexer.h"
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class SemanticAnalyzer
{
public:


	union operand
	{
		int jump_index;
		int symbol_index;
		operand()
		{
			jump_index = NULL;
		}
	};
	enum OperatorType
	{
		JP,//Jump
		JPMore,//JumpNotZero
		JPEqual,
		JPLess,
		JPMoreEqual,
		JPLessEqual,
		Multiply,
		Plus,
		Assign,
		Divide,
		Subtraction 
	};
	class symbol
	{
	public:
		string name;
		enum SymbolType
		{
			Const,
			Variable
		};
		SymbolType type;

		symbol(const string& _name, SymbolType _type) : name(_name),type(_type)
		{
			
		};
	};
	class quadruple
	{
	public:
		quadruple();
		quadruple(operand _fir, operand _sec, OperatorType _type, operand _res)
			:
			first(_fir),
			second(_sec),
			op(_type),
			result(_res)
		{

		}
		operand first;
		operand second;
		OperatorType op;
		operand result;

	};
	typedef vector<symbol> sybolsList;
	typedef unordered_map<string, int> SymbolMap;
	typedef vector<quadruple> QuadrupleList;

	sybolsList symbols;
	SymbolMap symbolsMap;
	QuadrupleList quadruples;

	SemanticAnalyzer();
	~SemanticAnalyzer();
	int index;
	void showQuadruples();
	string& getSymbolName(int index);
	void analyze(const Lexer::TokensVector& tokens);
	
	operand findSymbol(const string& symbol_name,Lexer::TokenType _type = Lexer::ID);
	int addSymbol(const string& symbol_name, symbol::SymbolType _type);
	int errorRowIndex;
	string errorMessage;
private:
	Lexer::TokensVector tokens;
	void checkAndRead(Lexer::TokenType);
	operand analyzeFactor();
	operand analyzeExpression();

	void analyzeBlock();
	int  analyzeStmt();
	void analyzeAssignment();
	void analyzeDecl();
	void analyzeBool();
	void analyzeWhile();
	void analyzeFor();

private:

};

#endif