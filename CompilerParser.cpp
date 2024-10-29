
#include "CompilerParser.h"
#include <iostream>


/**
 * Constructor for the CompilerParser
 * @param tokens A linked list of tokens to be parsed
 */
CompilerParser::CompilerParser(std::list<Token*> tokens) {
    this->tokensList = tokens;
    this->it = this->tokensList.begin();
}

/**
 * Generates a parse tree for a single program
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileProgram() {
    if (this->current()->getValue() != "class") throw ParseException();
    return compileClass();
}

/**
 * Generates a parse tree for a single class
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileClass() {
    ParseTree* tree = new ParseTree("class", "");
    tree->addChild(mustBe("keyword", "class"));
    tree->addChild(mustBe("identifier", ""));
    tree->addChild(mustBe("symbol", "{"));
    while (have("keyword", "static") || have("keyword", "field")) {
        tree->addChild(compileClassVarDec());
    }
    while (have("keyword", "constructor") || have("keyword", "function") || have("keyword", "method")) {
        tree->addChild(compileSubroutine());
    }
    tree->addChild(mustBe("symbol", "}"));
    return tree;
}

/**
 * Generates a parse tree for a static variable declaration or field declaration
 * @return a ParseTree
 */

ParseTree* CompilerParser::compileClassVarDec() {
    ParseTree* tree = new ParseTree("classVarDec", "");
    if (have("keyword", "static") || have("keyword", "field")) {
        tree->addChild(mustBe("keyword", ""));
    } else throw ParseException();
    tree->addChild(mustBe("keyword", ""));
    tree->addChild(mustBe("identifier", ""));
    while (have("symbol", ",")){
        tree->addChild(mustBe("symbol", ","));
        tree->addChild(mustBe("identifier", ""));
    }
    std::cout << current()->getType() << " " << current()->getValue() << "\n";
    tree->addChild(mustBe("symbol", ";"));
    return tree;
}

/**
 * Generates a parse tree for a method, function, or constructor
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutine() {
    ParseTree* tree = new ParseTree("subroutine", "");
    if (have("keyword", "constructor") || have("keyword", "function") || have("keyword", "method")) {
        tree->addChild(mustBe("keyword", ""));
    } else throw ParseException();
    if (have("keyword", "void")){
        tree->addChild(mustBe("keyword", "void"));
    } else if (have("keyword", "int") || have("keyword", "char") || have("keyword", "boolean")){
        tree->addChild(mustBe("keyword", ""));
    } else if (have("identifier", "")) {
        tree->addChild(mustBe("identifier", ""));
    } else throw ParseException();
    tree->addChild(mustBe("identifier", ""));
    tree->addChild(mustBe("symbol", "("));
    tree->addChild(compileParameterList());
    tree->addChild(mustBe("symbol", ")"));
    tree->addChild(compileSubroutineBody());
    return tree;
}

/**
 * Generates a parse tree for a subroutine's parameters
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileParameterList() {
    ParseTree* tree = new ParseTree("parameterList", "");
    if (!have("keyword", "int") && !have("keyword", "char") && !have("keyword", "boolean") && !have("identifier", "")) {
        return tree; 
    }
    if (have("keyword", "int") || have("keyword", "char") || have("keyword", "boolean")){
        tree->addChild(mustBe("keyword", ""));
    } else tree->addChild(mustBe("identifier", ""));
    tree->addChild(mustBe("identifier", ""));
    while (have("symbol", ",")) {
        tree->addChild(mustBe("symbol", ","));
        if (have("keyword", "int") || have("keyword", "char") || have("keyword", "boolean")) {
            tree->addChild(mustBe("keyword", ""));
        } 
        else if (have("identifier", "")) {
            tree->addChild(mustBe("identifier", "")); 
        }
        else throw ParseException(); 
        tree->addChild(mustBe("identifier", ""));
    }
    return tree;
}

/**
 * Generates a parse tree for a subroutine's body
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutineBody() {
    ParseTree* tree = new ParseTree("subroutineBody", "");
    tree->addChild(mustBe("symbol", "{"));
    while (have("keyword", "var")) {
        tree->addChild(compileVarDec());
    }
    tree->addChild(compileStatements());
    tree->addChild(mustBe("symbol", "}"));
    return tree;
}

/**
 * Generates a parse tree for a subroutine variable declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileVarDec() {
    ParseTree* tree = new ParseTree("varDec", "");
    tree->addChild(mustBe("keyword", "var"));
     if (have("keyword", "int") || have("keyword", "char") || have("keyword", "boolean")){
        tree->addChild(mustBe("keyword", ""));
    } else tree->addChild(mustBe("identifier", ""));
    tree->addChild(mustBe("identifier", ""));
    while (have("symbol", ",")) {
        tree->addChild(mustBe("symbol", ","));
        tree->addChild(mustBe("identifier", ""));
    }
    tree->addChild(mustBe("symbol", ";"));
    return tree;
}

/**
 * Generates a parse tree for a series of statements
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileStatements() {
    ParseTree* tree = new ParseTree("statements", "");
    while (have("keyword", "let") || have("keyword", "if") || have("keyword", "while") || have("keyword", "do") || have("keyword", "return")) {
        if (have("keyword", "let")) {
            tree->addChild(compileLet());
        } else if (have("keyword", "if")) {
            tree->addChild(compileIf());
        } else if (have("keyword", "while")) {
            tree->addChild(compileWhile());
        } else if (have("keyword", "do")) {
            tree->addChild(compileDo());
        } else if (have("keyword", "return")) {
            tree->addChild(compileReturn());
        }
    }
    return tree;
}

/**
 * Generates a parse tree for a let statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileLet() {
    ParseTree* tree = new ParseTree("letStatement", "");
    tree->addChild(mustBe("keyword", "let"));
    tree->addChild(mustBe("identifier", ""));
    if (have("symbol", "[")) {
        tree->addChild(mustBe("symbol", "["));
        tree->addChild(compileExpression());
        tree->addChild(mustBe("symbol", "]"));
    } 
    tree->addChild(mustBe("symbol", "="));
    tree->addChild(compileExpression());
    tree->addChild(mustBe("symbol", ";"));
    return tree;
}

/**
 * Generates a parse tree for an if statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileIf() {
     ParseTree* tree = new ParseTree("ifStatement", "");
    tree->addChild(mustBe("keyword", "if"));
    tree->addChild(mustBe("symbol", "("));
    tree->addChild(compileExpression());
    tree->addChild(mustBe("symbol", ")"));
    tree->addChild(mustBe("symbol", "{"));
    tree->addChild(compileStatements());
    tree->addChild(mustBe("symbol", "}"));
    if (have("keyword", "else")) {
        tree->addChild(mustBe("keyword", "else"));
        tree->addChild(mustBe("symbol", "{"));
        tree->addChild(compileStatements());
        tree->addChild(mustBe("symbol", "}"));
    }
    return tree;
}

/**
 * Generates a parse tree for a while statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileWhile() {
    ParseTree* tree = new ParseTree("whileStatement", "");
    tree->addChild(mustBe("keyword", "while"));
    tree->addChild(mustBe("symbol", "("));
    tree->addChild(compileExpression());
    tree->addChild(mustBe("symbol", ")"));
    tree->addChild(mustBe("symbol", "{"));
    tree->addChild(compileStatements());
    tree->addChild(mustBe("symbol", "}"));
    return tree;
}

/**
 * Generates a parse tree for a do statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileDo() {
    ParseTree* tree = new ParseTree("doStatement", "");
    tree->addChild(mustBe("keyword", "do"));
    tree->addChild(compileExpression());
    tree->addChild(mustBe("symbol", ";"));
    return tree;
}

/**
 * Generates a parse tree for a return statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileReturn() {
    ParseTree* tree = new ParseTree("returnStatement", "");
    tree->addChild(mustBe("keyword", "return"));
    if (!have("symbol", ";")) {
        tree->addChild(compileExpression());
    }
    tree->addChild(mustBe("symbol", ";"));
    return tree;
}

/**
 * Generates a parse tree for an expression
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpression() {
    ParseTree* tree = new ParseTree("expression", "");
    if (have("keyword", "skip")) tree->addChild(mustBe("keyword", ""));
    tree->addChild(compileTerm());
    while (have("symbol", "+") || have("symbol", "-") || have("symbol", "*") || have("symbol", "/") || have("symbol", "&") || have("symbol", "|") || have("symbol", "<") || have("symbol", ">") || have("symbol", "=")) {
        tree->addChild(mustBe("symbol", ""));
        tree->addChild(compileTerm());
    }
    return tree;
}

/**
 * Generates a parse tree for an expression term
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileTerm() {
    ParseTree* tree = new ParseTree("term", "");
    if (have("integerConstant", "")) {
        tree->addChild(mustBe("integerConstant", ""));
    } else if (have("stringConstant", "")) {
        tree->addChild(mustBe("stringConstant", ""));
    } else if (have("keyword", "true") || have("keyword", "false") || have("keyword", "null") || have("keyword", "this")) {
        tree->addChild(mustBe("keyword", ""));
    } else if (have("identifier", "")) {
        tree->addChild(mustBe("identifier", ""));
        if (have("symbol", "[")) {
            tree->addChild(mustBe("symbol", "["));
            tree->addChild(compileExpression());
            tree->addChild(mustBe("symbol", "]"));
        } else if (have("symbol", "(")) {
            tree->addChild(mustBe("symbol", "("));
            tree->addChild(compileExpressionList());
            tree->addChild(mustBe("symbol", ")"));
        } else if (have("symbol", ".")) {
            tree->addChild(mustBe("symbol", "."));
            tree->addChild(mustBe("identifier", ""));
            tree->addChild(mustBe("symbol", "("));
            tree->addChild(compileExpressionList());
            tree->addChild(mustBe("symbol", ")"));
        }
    } else if (have("symbol", "(")) {
        tree->addChild(mustBe("symbol", "("));
        tree->addChild(compileExpression());
        tree->addChild(mustBe("symbol", ")"));
    } else if (have("symbol", "-") || have("symbol", "~")) {
        tree->addChild(mustBe("symbol", ""));
        tree->addChild(compileTerm());
    }
    return tree;
    // return NULL;
}

/**
 * Generates a parse tree for an expression list
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpressionList() {
    ParseTree* tree = new ParseTree("expressionList", "");
    tree->addChild(compileExpression());
    while (have("symbol", ",")) {
        tree->addChild(mustBe("symbol", ","));
        tree->addChild(compileExpression());
    }
    return tree;
}

/**
 * Advance to the next token
 */
void CompilerParser::next(){
    if (this->it != this->tokensList.end()){
        ++it;
    }
}

/**
 * Return the current token
 * @return the Token
 */
Token* CompilerParser::current(){
    if (this->it != this->tokensList.end()) {
        return *this->it;
    }
    return nullptr;
}

/**
 * Check if the current token matches the expected type and value.
 * @return true if a match, false otherwise
 */
bool CompilerParser::have(std::string expectedType, std::string expectedValue){
    Token* token = current();
    if (token != nullptr){
        // if (token->getType() == expectedType) return true;
        // if (token->getType() == expectedType && (expectedValue.empty())) return true;
        if (token->getType() == expectedType && (expectedValue.empty() || token->getValue() == expectedValue)) return true;
    }
    return false;
}

/**
 * Check if the current token matches the expected type and value.
 * If so, advance to the next token, returning the current token, otherwise throw a ParseException.
 * @return the current token before advancing
 */
Token* CompilerParser::mustBe(std::string expectedType, std::string expectedValue){
    if (have(expectedType, expectedValue)){
        Token *a = this->current();
        next();
        return a;
    }
    else throw ParseException(); 
}

/**
 * Definition of a ParseException
 * You can use this ParseException with `throw ParseException();`
 */
const char* ParseException::what() {
    return "An Exception occurred while parsing!";
}