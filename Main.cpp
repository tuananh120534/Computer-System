#include <iostream>
#include <list>

#include "CompilerParser.h"
#include "Token.h"


using namespace std;

int main(int argc, char *argv[]) {
    /* Tokens for:
     *     class MyClass {
     *
     *     }
     */
    list<Token*> tokens;
    // compileProgram
    // tokens.push_back(new Token("keyword", "class"));
    // tokens.push_back(new Token("identifier", "Main"));
    // tokens.push_back(new Token("symbol", "{"));
    // tokens.push_back(new Token("symbol", "}"));

    // compileClass
    // tokens.push_back(new Token("keyword", "class"));
    // tokens.push_back(new Token("identifier", "Main"));
    // tokens.push_back(new Token("symbol", "{"));
    // tokens.push_back(new Token("keyword", "static"));
    // tokens.push_back(new Token("keyword", "int"));
    // tokens.push_back(new Token("identifier", "a"));
    // tokens.push_back(new Token("symbol", ";"));
    // tokens.push_back(new Token("symbol", "}"));



    // compileClassVarDec
    // tokens.push_back(new Token("keyword", "static"));
    // tokens.push_back(new Token("keyword", "int"));
    // tokens.push_back(new Token("identifier", "a"));
    // tokens.push_back(new Token("symbol", ";"));

    // compileParameterList
    // tokens.push_back(new Token("keyword", "int"));
    // tokens.push_back(new Token("identifier", "a"));
    // tokens.push_back(new Token("symbol", ","));
    // tokens.push_back(new Token("keyword", "char"));
    // tokens.push_back(new Token("identifier", "b"));
    // tokens.push_back(new Token("symbol", ","));
    // tokens.push_back(new Token("identifier", "MyClass"));
    // tokens.push_back(new Token("identifier", "c"));

    //compileVarDec
    // tokens.push_back(new Token("keyword", "var"));
    // tokens.push_back(new Token("keyword", "int"));
    // tokens.push_back(new Token("identifier", "a"));
    // tokens.push_back(new Token("symbol", ";"));

    //compileLet
    // tokens.push_back(new Token("keyword", "let"));
    // tokens.push_back(new Token("identifier", "a"));
    // tokens.push_back(new Token("symbol", "="));
    // tokens.push_back(new Token("keyword", "skip"));
    // tokens.push_back(new Token("symbol", ";"));

    // compileStatements
    // tokens.push_back(new Token("keyword", "let"));
    // tokens.push_back(new Token("identifier", "a"));
    // tokens.push_back(new Token("symbol", "="));
    // tokens.push_back(new Token("keyword", "skip"));
    // tokens.push_back(new Token("symbol", ";"));
    // tokens.push_back(new Token("keyword", "do"));
    // tokens.push_back(new Token("keyword", "skip"));
    // tokens.push_back(new Token("symbol", ";"));
    // tokens.push_back(new Token("keyword", "return"));
    // tokens.push_back(new Token("symbol", ";"));

    // compileExpressionList
    tokens.push_back(new Token("integer", ""));
    tokens.push_back(new Token("identifier", "a"));
    tokens.push_back(new Token("symbol", "="));
    tokens.push_back(new Token("keyword", "skip"));
    tokens.push_back(new Token("symbol", ";"));
    tokens.push_back(new Token("keyword", "do"));
    tokens.push_back(new Token("keyword", "skip"));
    tokens.push_back(new Token("symbol", ";"));
    tokens.push_back(new Token("keyword", "return"));
    tokens.push_back(new Token("symbol", ";"));

    
    




    try {
        CompilerParser parser(tokens);
        // ParseTree* result = parser.compileParameterList();
        ParseTree* result = parser.compileStatements();
        if (result != NULL){
            cout << result->tostring() << endl;
        }
    } catch (ParseException& e) {
        cout << "Error Parsing!" << endl;
    }
}