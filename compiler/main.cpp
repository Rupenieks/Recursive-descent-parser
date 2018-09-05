
#include <cstdlib>
#include <fstream>
#include "Parser.h"
#include <Windows.h>

using namespace std;

/*
*
*/
int main(int argc, char** argv) {

	char correctSyntax[300];
	char incorrectSyntax[300];
	ifstream infile{ "text.go" };
	string textInput{ istreambuf_iterator<char>(infile), istreambuf_iterator<char>() };
	strcpy_s(correctSyntax, textInput.c_str());

	

	Parser parser;

	ASTNode* correctast = parser.Parse(correctSyntax);

	if (correctast != NULL)
		std::cout << correctast->MakeString(0) << endl;

	ifstream infile2{ "wrongtextgo.go" };
	string textInput2{ istreambuf_iterator<char>(infile2), istreambuf_iterator<char>() };
	strcpy_s(incorrectSyntax, textInput2.c_str());

	ASTNode* incorrectast = parser.Parse(incorrectSyntax);

	if (incorrectast != NULL)
		std::cout << incorrectast->MakeString(0) << endl;

	system("pause");
	return 0;
}
