#include "SemanticAnalyzer.h"
#include <iostream>


SemanticAnalyzer::SemanticAnalyzer()
{
	addSymbol("NULL", symbol::Variable);
	addSymbol("_temp", symbol::Variable);
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
		oprd = findSymbol(tokens[index].value,Lexer::Num);
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

	switch (tokens[index].type)
	{
	case Lexer::Subtraction:
		expression_quad.first = first;
		op = Subtraction;
		expression_quad.op = op;
		index++;
		expression_quad.second = analyzeExpression();
		expression_quad.result = result;
		quadruples.push_back(expression_quad);
		return result;
		break;
	case Lexer::Divide:
		expression_quad.first = first;
		op =Divide;
		expression_quad.op = op;
		index++;
		expression_quad.second = analyzeExpression();
		expression_quad.result = result;
		quadruples.push_back(expression_quad);
		return result;
		break;
	case Lexer::Multiply:
		expression_quad.first = first;
		op = Multiply;
		expression_quad.op = op;

		index++;
		expression_quad.second = analyzeExpression();
		expression_quad.result = result;
		quadruples.push_back(expression_quad);
		return result;
		break;
	case Lexer::Plus:
	{
		
		expression_quad.first = first;
		op = Plus;
		expression_quad.op = op;
		index++;
		expression_quad.second = analyzeExpression();
		expression_quad.result = result;
		quadruples.push_back(expression_quad);
		return result;
		break;
	}
	case Lexer::Semicolon:
		return first;

		break;
	}
}

void SemanticAnalyzer::showQuadruples()
{
	for (auto item :quadruples)
	{
		cout
			<< "(";
		switch (item.op)
		{
		case OperatorType::Multiply:
			cout << "*";
			break;
		case OperatorType::Divide:
			cout << "/";
			break;
		case OperatorType::JP:
			cout << "JP";
			break;
		case OperatorType::JPEqual:
			cout << "JP=";
			break;
		case OperatorType::JPLess:
			cout << "JP<";
			break;
		case OperatorType::JPLessEqual:
			cout << "JP<=";
			break;
		case OperatorType::Subtraction:
			cout << "JP-";
			break;
		case OperatorType::Assign:
			cout << "=";
			break;
		case OperatorType::JPMore:
			cout << "JP>";
			break;
		case OperatorType::Plus:
			cout << "+";
			break;
		case OperatorType::JPMoreEqual:
			cout << "JP>=";
			break;

		}
		cout<< ","
			<< getSymbolName(item.first.symbol_index) << ","
			<< getSymbolName(item.second.symbol_index) << ","
			<< getSymbolName(item.result.symbol_index)
			<< ")" << endl;
	}
}

string& SemanticAnalyzer::getSymbolName(int index)
{
	return symbols[index].name;
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
SemanticAnalyzer::operand SemanticAnalyzer::findSymbol(const string & symbol_name,Lexer::TokenType _type)
{
	operand op;

	if (symbolsMap.find(symbol_name) != symbolsMap.end())
	{
		op.symbol_index = symbolsMap[symbol_name];
	}
	else
	{
		if(_type == Lexer::Num)
		{
			op.symbol_index = addSymbol(symbol_name, symbol::Const);
			return op;
			
		}else
		{
			cout << "ʹ����δ�����ı���" << endl;
			throw exception();
		}

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
		checkAndRead(Lexer::Semicolon);
		break;
	case Lexer::Int:
	case Lexer::Float:
		analyzeDecl();
		checkAndRead(Lexer::Semicolon);
		break;
	case Lexer::If:
		index++;

		errorMessage = "If���ȱ��������";
		checkAndRead(Lexer::LeftBrackets);
		analyzeBool();
		errorMessage = "If���ȱ��������";
		checkAndRead(Lexer::RightBrackets);
		checkAndRead(Lexer::LeftBigBrackets);

		errorMessage = "If�������";
		analyzeStmt();
		checkAndRead(Lexer::RightBigBrackets);
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

	quadruple boolQuad;
	errorMessage = "����������";
	boolQuad.first = analyzeFactor();
	switch (tokens[index].type)
	{
	case Lexer::Equal:
		boolQuad.op = OperatorType::JPEqual;
		index++;
		boolQuad.second = analyzeFactor();
		break;
	case Lexer::LessOrEqul:
		boolQuad.op = OperatorType::JPLessEqual;
		index++;
		boolQuad.second =  analyzeFactor();
		break;
	case Lexer::LessThan:

		boolQuad.op = OperatorType::JPLess;
		index++;
		boolQuad.second = analyzeFactor();
		break;
	case Lexer::MoreOrEqul:

		boolQuad.op = OperatorType::JPMoreEqual;
		index++;
		boolQuad.second = analyzeFactor();
		break;
	case Lexer::MoreThan:

		boolQuad.op = OperatorType::JPMore;
		index++;
		boolQuad.second = analyzeFactor();
		break;
	}
	quadruples.push_back(boolQuad);
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
