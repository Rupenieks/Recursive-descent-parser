#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()


enum ASTNodeType
{
	START_,
	UNDEFINED_,
	ASSIGNMENT_,
	IMPORT_,
	IMPORTS_,
	PACKAGE_,
	STRINGLITERAL_,
	NUMBERVALUE_,
	BODY_,
	VARIABLEDECLARATION_,
	FUNCTIONDECLARATION_,
	NAME_
};

class ASTNode
{
public:
	ASTNodeType Type;
	int Value;
	std::string Symbol;
	std::vector<ASTNode*> children;

	ASTNode()
	{
		Type = UNDEFINED_;
		Value = 0;
	}

	~ASTNode()
	{
		delete &children;
	}

	std::string MakeString(int p_depth)
	{
		std::string str = "";
		str.append(TypeString());
		int depth = p_depth + 1;
		if (children.begin() != children.end()) {
			str.append(" ->\n");
			for (std::vector<ASTNode*>::iterator it = children.begin(); it != children.end(); ++it)
			{
				for (int i = 0; i < depth; i++) str.append("\t");
				ASTNode* t = *it;
				std::string ts = (*t).MakeString(depth);
				str.append(ts);
			}
		}
		else str.append("\n");
		return str;
	}

	std::string TypeString()
	{
		switch (Type)
		{
		case START_:
			return "Root";
			break;
		case UNDEFINED_:
			return "UNDEFINED";
			break;
		case ASSIGNMENT_:
			return "Assignment";
			break;
		case IMPORT_:
			return "Import";
			break;
		case PACKAGE_:
			return "Package";
			break;
		case STRINGLITERAL_:
			return "StringLiteral";
			break;
		case NUMBERVALUE_:
			return "NumberValue";
			break;
		case BODY_:
			return "Body";
			break;
		case VARIABLEDECLARATION_:
			return "VariableDeclaration";
			break;
		case FUNCTIONDECLARATION_:
			return "FunctionDeclaration";
			break;
		case NAME_:
			return "Name";
			break;
		case IMPORTS_:
			return "Imports";
			break;
		default:
			return "oops";
		}
	}

};







enum TokenType {
	STRING_LITERAL,
	NUMBER,
	EQUALS,
	OPEN_PARENTHESIS,
	CLOSED_PARENTHESIS,
	OPEN_CURLY_BRACKET,
	CLOSED_CURLY_BRACKET,
	DOUBLE_QUOTE,
	END_OF_TEXT
};

struct Token {
	TokenType type;
	int value;
	std::string symbol;
};

class ParserException : public std::exception {
	std::string msg;
public:
	ParserException(const std::string& message, int index)
		: msg(std::string("PARSER ERROR: " + message + " at " + SSTR(index)))
	{}

	virtual const char* what() const throw()
	{
		return msg.c_str();
	}
};

class Parser {
public:
	Parser();
	Parser(const Parser& orig);
	virtual ~Parser();
private:
	Token current_token;
	const char* text;
	size_t text_index;

	ASTNode* Start();
	ASTNode* Package();
	ASTNode* Name();
	ASTNode* Import();
	ASTNode* Body();
	ASTNode* VariableDeclaration();
	ASTNode* FunctionDeclaration();
	ASTNode* CreateNode(ASTNodeType);
	ASTNode* CreateNode(ASTNodeType, std::vector<ASTNode*>);
	void Match(std::string);
	void SkipWhitespace();
	void NextToken();
	void GetString();
	void GetNumber();
public:
	ASTNode * Parse(const char*);
};

