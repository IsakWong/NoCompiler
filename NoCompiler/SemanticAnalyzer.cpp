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
	cout << errorRowIndex<<"ÐÐ·¢ÉúÓï·¨·ÖÎö´íÎó£¡£¡ ´íÎóÐÅÏ¢£º"<<errorMessage;
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
	int i = 0;
	for (auto item :quadruples)
	{
		cout<<i++
			<< ":(";
		switch (item.op)
		{
		case OperatorType::Multiply:
			cout << "*";
			cout << ","
				<< getSymbolName(item.first.symbol_index) << ","
				<< getSymbolName(item.second.symbol_index) << ","
				<< getSymbolName(item.result.symbol_index)
				<< ")" << endl;
			break;
		case OperatorType::Divide:
			cout << "/";
			cout << ","
				<< getSymbolName(item.first.symbol_index) << ","
				<< getSymbolName(item.second.symbol_index) << ","
				<< getSymbolName(item.result.symbol_index)
				<< ")" << endl;
			break;
		case OperatorType::JP:
			cout << "JP";
			cout << ","
				<< getSymbolName(item.first.symbol_index) << ","
				<< getSymbolName(item.second.symbol_index) << ","
				<< item.result.jump_index
				<< ")" << endl;
			break;
		case OperatorType::JPEqual:
			cout << "JP=";
			cout << ","
				<< getSymbolName(item.first.symbol_index) << ","
				<< getSymbolName(item.second.symbol_index) << ","
				<< item.result.jump_index
				<< ")" << endl;
			break;
		case OperatorType::JPLess:
			cout << ","
				<< getSymbolName(item.first.symbol_index) << ","
				<< getSymbolName(item.second.symbol_index) << ","
				<< item.result.jump_index
				<< ")" << endl;
			cout << "JP<";
			break;
		case OperatorType::JPLessEqual:
			cout << ","
				<< getSymbolName(item.first.symbol_index) << ","
				<< getSymbolName(item.second.symbol_index) << ","
				<< item.result.jump_index
				<< ")" << endl;
			cout << "JP<=";
			break;
		case OperatorType::Subtraction:
			cout << "JP-";
			cout << ","
				<< getSymbolName(item.first.symbol_index) << ","
				<< getSymbolName(item.second.symbol_index) << ","
				<< getSymbolName(item.result.symbol_index)
				<< ")" << endl;
			break;
		case OperatorType::Assign:
			cout << "=";
			cout << ","
				<< getSymbolName(item.first.symbol_index) << ","
				<< getSymbolName(item.second.symbol_index) << ","
				<< getSymbolName(item.result.symbol_index)
				<< ")" << endl;
			break;
		case OperatorType::JPMore:
			cout << "JP>";
			cout << ","
				<< getSymbolName(item.first.symbol_index) << ","
				<< getSymbolName(item.second.symbol_index) << ","
				<< item.result.jump_index
				<< ")" << endl;
			break;
		case OperatorType::Plus:
			cout << "+";
			cout << ","
				<< getSymbolName(item.first.symbol_index) << ","
				<< getSymbolName(item.second.symbol_index) << ","
				<< getSymbolName(item.result.symbol_index)
				<< ")" << endl;
			break;
		case OperatorType::JPMoreEqual:
			cout << "JP>=";
			cout << ","
				<< getSymbolName(item.first.symbol_index) << ","
				<< getSymbolName(item.second.symbol_index) << ","
				<< item.result.jump_index
				<< ")" << endl;
			break;

		}
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
			cout << "Ê¹ÓÃÁËÎ´ÉùÃ÷µÄ±äÁ¿" << endl;
			throw exception();
		}

	}
	return op;
}
 int SemanticAnalyzer::addSymbol(const string& symbol_name, symbol::SymbolType _type)
{
	if (symbolsMap.find(symbol_name) != symbolsMap.end())
	{

		cout << "±äÁ¿ÖØ¸´ÉùÃ÷" << endl;
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
	{
		index++;
		quadruple boolQuad;
		quadruples.push_back(boolQuad);

		int boolQuadRef = quadruples.size() - 1;

		
		errorMessage = "IfÓï¾äÈ±ÉÙ×óÀ¨ºÅ";
		checkAndRead(Lexer::LeftBrackets);
		analyzeBool();
		quadruple elseJumpQuad;
		elseJumpQuad.op = JP;
		quadruples.push_back(elseJumpQuad);
		int elseJumpQuadRef = quadruples.size() - 1;
		quadruples[boolQuadRef].result.jump_index = quadruples.size();



		errorMessage = "IfÓï¾äÈ±ÉÙÓÒÀ¨ºÅ";
		checkAndRead(Lexer::RightBrackets);
		checkAndRead(Lexer::LeftBigBrackets);
		errorMessage = "IfÓï¾ä¿é´íÎó";
		analyzeStmt();
			
		quadruple thenJumpQuad;
		thenJumpQuad.op = JP;
		quadruples.push_back(thenJumpQuad);
		int thenJumpQuadRef = quadruples.size() - 1;
		quadruples[elseJumpQuadRef].result.jump_index = quadruples.size();

		checkAndRead(Lexer::RightBigBrackets);
		checkAndRead(Lexer::Lexer::Else);
		checkAndRead(Lexer::LeftBigBrackets);
		analyzeStmt();
		quadruples[thenJumpQuadRef].result.jump_index = quadruples.size();

		checkAndRead(Lexer::RightBigBrackets);
		break;
	}
	case Lexer::While:
		index++;
		errorMessage = "WhileÓï¾äÈ±ÉÙ×óÀ¨ºÅ";
		checkAndRead(Lexer::LeftBrackets);
		analyzeBool();
		index++;
		checkAndRead(Lexer::RightBrackets);
		errorMessage = "WhileÓï¾äÈ±ÉÙ×ó´óÀ¨ºÅ";
		checkAndRead(Lexer::LeftBigBrackets);
		index++;
		analyzeStmt();
		errorMessage = "WhileÓï¾äÈ±ÉÙÓÒ´óÀ¨ºÅ";
		checkAndRead(Lexer::RightBigBrackets);
		break;
	}
	analyzeStmt();
	return 1;
}

void SemanticAnalyzer::analyzeAssignment()
{
	errorMessage = "¸³ÖµÓï¾ä´íÎó";
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
	errorMessage = "ÉùÃ÷Óï¾ä´íÎó";
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

	quadruple& boolQuad = quadruples.back();
		
	errorMessage = "²¼¶ûÓï¾ä´íÎó";
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
