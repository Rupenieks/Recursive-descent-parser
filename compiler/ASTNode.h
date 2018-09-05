#pragma once

class ASTNode
{
public:
	ASTNodeType Type;
	int Value;
	std::string Symbol;
	ASTNode* Left;
	ASTNode* Right;

	ASTNode()
	{
		Type = Undefined;
		Value = 0;
		Left = NULL;
		Right = NULL;
	}

	~ASTNode()
	{
		delete Left;
		delete Right;
	}

};