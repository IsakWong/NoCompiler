#include "SemanticAnalyzer.h"
#include <iostream>


SemanticAnalyzer::SemanticAnalyzer()
{
	addSymbol("__temp", symbol::Variable);
}


SemanticAnalyzer::~SemanticAnalyzer()
{
}

void SemanticAnalyzer::checkAndRead(Lexer::TokenType _type)
{
	errorRowIndex = tokens[index].row;
	if (tokens[index++].type == _type)
		return;
	cout << errorRowIndex<<"�з����﷨�������󣡣� ������Ϣ��"<<errorMessage;
	throw exception();
}

SemanticAnalyzer::operand SemanticAnalyzer::analyzeFactor()
{
	operand oprd;
	switch (tokens[index].type)
	{
	case Lexer::ID:
		oprd  = findSymbol(tokens[index].value);
		index++;
		return oprd;
		break;
	case Lexer::Num:
		oprd = findSymbol(tokens[index].value);
		index++;
		return oprd;
		break;
	}
	throw exception();
}

SemanticAnalyzer::operand SemanticAnalyzer::analyzeExpression()
{
	operand first = analyzeFactor();
	operand second;
	OperatorType op;
	operand result = findSymbol("_temp");
	quadruple expression_quad;
	expression_quad.first = first;

	switch (tokens[index].type)
	{
	case Lexer::Plus:
		op = Plus;
		expression_quad.op = op;
		second = analyzeFactor();
		expression_quad.second = second;
		break;
	}
	expression_quad.result = result;
	quadruples.push_back(expression_quad);
	return result;
}

void SemanticAnalyzer::analyze(const Lexer::TokensVector& _tokens)
{
	tokens = _tokens;
	index = 0;
	checkAndRead(Lexer::Int);
	checkAndRead(Lexer::Main);
	checkAndRead(Lexer::LeftBrackets);
	checkAndRead(Lexer::RightBrackets);
	analyzeBlock();

}
SemanticAnalyzer::operand SemanticAnalyzer::findSymbol(const string & symbol_name)
{
	operand op;

	if (symbolsMap.find(symbol_name) != symbolsMap.end())
	{
		op.symbol_index = symbolsMap[symbol_name];
	}
	else
	{
		cout << "ʹ����δ�����ı���" << endl;
		throw exception();

	}
	return op;
}
 int SemanticAnalyzer::addSymbol(const string& symbol_name, symbol::SymbolType _type)
{
	if (symbolsMap.find(symbol_name) != symbolsMap.end())
	{

		cout << "�����ظ�����" << endl;
		throw exception();
	}
	int index = symbols.size();
	symbols.push_back(symbol(symbol_name, _type));
	symbolsMap[symbol_name] = index;
	return index;
}

void SemanticAnalyzer::analyzeBlock()
{
	checkAndRead(Lexer::LeftBigBrackets);
	analyzeStmt();
	checkAndRead(Lexer::RightBigBrackets);

}



int SemanticAnalyzer::analyzeStmt() throw(exception)
{
	switch (tokens[index].type)
	{
	case Lexer::Return:
		return 1;
	case Lexer::RightBigBrackets:
		return 1;
	case Lexer::ID:
		analyzeAssignment();
		index++;
		checkAndRead(Lexer::Semicolon);
	case Lexer::Int:
	case Lexer::Float:
		analyzeDecl();
		index++;
		checkAndRead(Lexer::Semicolon);
		break;
	case Lexer::If:
		index++;

		errorMessage = "If���ȱ��������";
		checkAndRead(Lexer::LeftBrackets);
		analyzeBool();
		index++;
		errorMessage = "If���ȱ��������";
		checkAndRead(Lexer::RightBrackets);
		checkAndRead(Lexer::LeftBigBrackets);
		checkAndRead(Lexer::RightBigBrackets);

		errorMessage = "If�������";
		analyzeStmt();
		index++;
		checkAndRead(Lexer::Lexer::Else);
		checkAndRead(Lexer::LeftBigBrackets);
		analyzeStmt();
		checkAndRead(Lexer::RightBigBrackets);
		break;
	case Lexer::While:
		index++;
		errorMessage = "While���ȱ��������";
		checkAndRead(Lexer::LeftBrackets);
		analyzeBool();
		index++;
		checkAndRead(Lexer::RightBrackets);
		errorMessage = "While���ȱ���������";
		checkAndRead(Lexer::LeftBigBrackets);
		index++;
		analyzeStmt();
		index++;
		errorMessage = "While���ȱ���Ҵ�����";
		checkAndRead(Lexer::RightBigBrackets);
		break;
	}
	analyzeStmt();
	return 1;
}

void SemanticAnalyzer::analyzeAssignment()
{
	errorMessage = "��ֵ������";
	quadruple assign;
	switch (tokens[index].type)
	{
	case Lexer::ID:
		assign.first = findSymbol(tokens[index].value);
		index++; 
		
		checkAndRead(Lexer::Assign);

		operand tmp = analyzeExpression();
		assign.op = OperatorType::Assign;
		assign.result = tmp;

		quadruples.push_back(assign);
		break;
	}
}


void SemanticAnalyzer::analyzeDecl()
{
	errorMessage = "����������";
	switch (tokens[index].type)
	{
	case Lexer::Int:
	case Lexer::Float:
		index++;

		checkAndRead(Lexer::ID);
		addSymbol(tokens[index - 1].value, symbol::SymbolType::Variable);
		
		break;
	}
}

void SemanticAnalyzer::analyzeBool()
{

	errorMessage = "����������";
	analyzeFactor();
	switch (tokens[index].type)
	{
	case Lexer::Equal:
	case Lexer::LessOrEqul:
	case Lexer::LessThan:
	case Lexer::MoreOrEqul:
	case Lexer::MoreThan:
		index++;
		analyzeFactor();
		break;
	}
}

void SemanticAnalyzer::analyzeWhile()
{
}

void SemanticAnalyzer::analyzeFor()
{
}

SemanticAnalyzer::quadruple::quadruple()
{
}
