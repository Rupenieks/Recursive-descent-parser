
#include "Parser.h"
#include <string>


Parser::Parser()
{
}

Parser::Parser(const Parser& orig)
{
}

Parser::~Parser()
{
}

ASTNode* Parser::Start()
{
	ASTNode* packageNode = Package();
	ASTNode* importNode = Import();
	ASTNode* bodyNode = Body();
	std::vector<ASTNode*> arr = { packageNode, importNode, bodyNode };

	return CreateNode(START_, arr);
}

ASTNode* Parser::Package()
{
	NextToken();
	Match("package");
	ASTNode* nameNode = Name();
	ASTNode* stringNode = CreateNode(STRINGLITERAL_);
	std::vector<ASTNode*> arr = { nameNode, stringNode };

	return CreateNode(PACKAGE_, arr);
}

ASTNode* Parser::Name()
{
	NextToken();
	if (current_token.type == STRING_LITERAL)
	{
		ASTNode* stringNode = CreateNode(STRINGLITERAL_);
		std::vector<ASTNode*> arr = { stringNode };
		return CreateNode(NAME_, arr);
	}
	else
	{
		std::string message = "Expected STRING LITERAL, but got: " + SSTR(current_token.type);
		throw ParserException(message, text_index);
	}
}

ASTNode* Parser::Import()
{
	NextToken();
	Match("import");

	ASTNode* nameNode = Name();
	ASTNode* stringNode = CreateNode(STRINGLITERAL_);

	std::vector<ASTNode*> arr = { nameNode, stringNode };

	return CreateNode(IMPORT_, arr);
}



ASTNode* Parser::Body()
{
	NextToken();

	std::vector<ASTNode*> arr;

	if (current_token.symbol == "var")
	{
		ASTNode* varDeclarationNode = VariableDeclaration();
		arr.push_back(varDeclarationNode);
	}
	else if (current_token.symbol == "func")
	{
		ASTNode* funcDeclarationNode = FunctionDeclaration();
		arr.push_back(funcDeclarationNode);
	}
	else
	{
		std::string message = "Expected STRING LITERAL (var/func), but got: " + SSTR(current_token.type);
		throw ParserException(message, text_index);
	}

	NextToken();

	if (current_token.type == END_OF_TEXT)
	{
		std::cout << "end of text reached \n";
		return CreateNode(BODY_, arr);
	}
	
}



ASTNode* Parser::VariableDeclaration()
{
	std::vector<ASTNode*> arr;
	arr.push_back(CreateNode(STRINGLITERAL_));
	//NextToken();
	arr.push_back(Name());
	NextToken();


	if (current_token.type != EQUALS)
	{
		std::string message = "Expected EQUALS, but got: " + SSTR(current_token.type);
		throw ParserException(message, text_index);
	}
	arr.push_back(CreateNode(ASSIGNMENT_));
	NextToken();

	if (current_token.type == STRING_LITERAL)
	{
		arr.push_back(Name());
	}
	else if (current_token.type == NUMBER)
	{
		arr.push_back(CreateNode(NUMBERVALUE_));
	}
	else
	{
		std::string message = "Expected STRING LITERAL or NUMBER, but got: " + SSTR(current_token.type);
		throw ParserException(message, text_index);
	}

	return CreateNode(VARIABLEDECLARATION_, arr);

}

ASTNode* Parser::FunctionDeclaration()
{
	std::vector<ASTNode*> arr;
	arr.push_back(CreateNode(STRINGLITERAL_));

	//NextToken();
	arr.push_back(Name());
	NextToken();


	if (current_token.type != OPEN_PARENTHESIS)
	{
		std::string message = "Expected OPEN PARENTHESIS, but got: " + SSTR(current_token.type);
		throw ParserException(message, text_index);
	}

	NextToken();

	if (current_token.type != CLOSED_PARENTHESIS)
	{
		std::string message = "Expected CLOSED PARENTHESIS, but got: " + SSTR(current_token.type);
		throw ParserException(message, text_index);
	}

	NextToken();

	if (current_token.type != OPEN_CURLY_BRACKET)
	{
		std::string message = "Expected OPEN CURLY BRACKET, but got: " + SSTR(current_token.type);
		throw ParserException(message, text_index);
	}

	NextToken();

	if (current_token.symbol == "var")
	{
		ASTNode* varDeclaration = VariableDeclaration();
		arr.push_back(varDeclaration);
		
	}

	NextToken();

	if (current_token.type != CLOSED_CURLY_BRACKET)
	{
		
		std::string message = "Expected CLOSED CURLY BRACKET, but got: " + SSTR(current_token.type);
		throw ParserException(message, text_index);
		
	}

	

	return CreateNode(FUNCTIONDECLARATION_, arr);
	//NextToken();
}




void Parser::SkipWhitespace()
{
	while (isspace(text[text_index]))
	{
		text_index++;
	}

}

void Parser::NextToken()
{
	SkipWhitespace();
	//debug
	std::cout << "token             ";
	current_token.value = 0;
	current_token.symbol = "";

	if (text[text_index] == 0)
	{
		current_token.type = END_OF_TEXT;
		return;
	}

	switch (text[text_index])
	{
	case '=':
		current_token.type = EQUALS;
		text_index++;
		break;
	case '(':
		current_token.type = OPEN_PARENTHESIS;
		text_index++;
		break;
	case ')':
		current_token.type = CLOSED_PARENTHESIS;
		text_index++;
		break;
	case '{':
		current_token.type = OPEN_CURLY_BRACKET;
		text_index++;
		break;
	case '}':
		current_token.type = CLOSED_CURLY_BRACKET;
		text_index++;
		break;
	case '"':
		current_token.type = DOUBLE_QUOTE;
		text_index++;
		/*Name();
		Match("\"");*/
		break;
	}

	if (isalpha(text[text_index]))
	{
		current_token.type = STRING_LITERAL;
		GetString();
	}

	if (isdigit(text[text_index]))
	{
		current_token.type = NUMBER;
		GetNumber();
	}

	//debug
	std::cout << current_token.type << " " << current_token.value << " " << current_token.symbol << std::endl;
}

void Parser::GetString()
{
	int start_index = text_index;

	while (isalpha(text[text_index]))
	{
		text_index++;
	}

	char string_buffer[24] = { 0 };
	memcpy(string_buffer, &text[start_index], text_index - start_index);

	std::string str(string_buffer);
	current_token.symbol = str;
}

void Parser::GetNumber()
{
	int start_index = text_index;

	while (isdigit(text[text_index]))
	{
		text_index++;
	}

	char string_buffer[24] = { 0 };
	memcpy(string_buffer, &text[start_index], text_index - start_index);

	current_token.value = atoi(string_buffer);
}

void Parser::Match(std::string p_expected)
{
	if (current_token.symbol != p_expected)
	{
		std::string message = "Unexpected token: " + current_token.symbol;
		throw ParserException(message, text_index);
	}
}


ASTNode * Parser::CreateNode(ASTNodeType type)
{
	ASTNode* node = new ASTNode;
	node->Type = type;

	return node;
}


ASTNode * Parser::CreateNode(ASTNodeType type, std::vector<ASTNode*> children)
{
	ASTNode* node = new ASTNode();
	node->children = children;
	node->Type = type;
	return node;
}

ASTNode* Parser::Parse(const char* p_text)
{
	text = p_text;
	text_index = 0;
	try
	{
		return Start();
	}
	catch (ParserException e)
	{
		std::cout << e.what() << std::endl;
		return NULL;
	}
}
