#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <list>
#include "lex-pars.h"

using namespace std;

operator_node::operator_node(exp_node* L, exp_node* R) {
	left = L;
	right = R;
}

number_node::number_node(float value) {
	num = value;
}

void number_node::print() {
	std::cout << num;
}

float number_node::evaluate() {
	std::cout << "number_node: operand = " << num << endl;
	return num;
}

id_node::id_node(string value) : id(value) {}

void id_node::print() {
	std::cout << id;
}

float id_node::evaluate() {
	std::cout << "id_node: " << id << " = " << idTable[id] << endl;
	return idTable[id];
}

plus_node::plus_node(exp_node* L, exp_node* R) : operator_node(L, R) {
}

void plus_node::print() {
	std::cout << "(";
	left->print();
	std::cout << " + ";
	right->print();
	std::cout << ")";
}

float plus_node::evaluate() {
	float left_num, right_num;

	left_num = left->evaluate();
	right_num = right->evaluate();

	num = left_num + right_num;
	std::cout << "plus_node: " << left_num << " + " << right_num << " = " << num << "\n";
	return (num);
}

void bool_op_node::print()
{
	string op = "";
	switch (opType)
	{
	case LessEQ:
		op = "<=";
		break;
	case GrEQ:
		op = ">=";
		break;
	case Equal:
		op = "==";
		break;
	case NEqual:
		op = "!=";
		break;
	case Greater:
		op = ">";
		break;
	case Less:
		op = "<";
		break;
	case AND_OP:
		op = "&&";
		break;
	case OR_OP:
		op = "||";
		break;
	default:
		break;
	}
	left->print();
	std::cout << " " << op << " ";
	right->print();
}

float bool_op_node::evaluate()
{
	float left_num, right_num;
	left_num = left->evaluate();
	right_num = right->evaluate();


	bool l, r; //in case expression was boolean
	if (left_num == 0.0f) l = false;
	else l = true;
	if (right_num == 0.0f) r = false;
	else r = true;

	switch (opType)
	{
	case LessEQ:
		val = left_num <= right_num;
		break;
	case GrEQ:
		val = left_num >= right_num;
		break;
	case Equal:
		val = left_num == right_num;
		break;
	case NEqual:
		val = left_num != right_num;
		break;
	case Greater:
		val = left_num > right_num;
		break;
	case Less:
		val = left_num < right_num;
		break;
	case AND_OP:
		val = l && r;
		break;
	case OR_OP:
		val = l || r;
		break;
	default:
		std::cout << "Unkown operation\n";
		break;
	}

	return val;
}

times_node::times_node(exp_node* L, exp_node* R) : operator_node(L, R) {
}

void times_node::print() {
	std::cout << "(";
	left->print();
	std::cout << " * ";
	right->print();
	std::cout << ")";
}

float times_node::evaluate() {
	float left_num, right_num;

	left_num = left->evaluate();
	right_num = right->evaluate();

	num = left_num * right_num;
	std::cout << "times_node: " << left_num << " * " << right_num << " = " << num << "\n";
	return (num);
}

minus_node::minus_node(exp_node* L, exp_node* R) : operator_node(L, R) {
}

void minus_node::print() {
	std::cout << "(";
	left->print();
	std::cout << "-";
	right->print();
	std::cout << ")";
}

float minus_node::evaluate() {
	float left_num, right_num;

	left_num = left->evaluate();
	right_num = right->evaluate();

	num = left_num - right_num;
	std::cout << "minus_node: " << left_num << "-" << right_num << "=" << num << "\n";

	return (num);
}

assignment_stmt::assignment_stmt(string name, exp_node* expression)
	: id(name), exp(expression) {}

void assignment_stmt::print() {
	std::cout << id << " = ";
	exp->print();
	std::cout << endl;
}

void assignment_stmt::evaluate() {
	float result = exp->evaluate();
	std::cout << "assignment_node: " << id << " = " << result << endl << endl;
	idTable[id] = result;
}

print_stmt::print_stmt(string name) : id(name) { }

void print_stmt::evaluate() {
	std::cout << "print_node: " << id << " = " << idTable[id] << endl << endl;
}

pgm::pgm(list<statement*>* stmtList) : stmts(stmtList) {}

void pgm::evaluate() {
	list<statement*>::iterator stmtIter;
	for (stmtIter = stmts->begin(); stmtIter != stmts->end();
		stmtIter++) {
		(*stmtIter)->print();
		(*stmtIter)->evaluate();
	}
}

void pgm::print() {
	list<statement*>::iterator stmtIter;
	std::cout << "\nGoing to print AST\n";
	for (stmtIter = stmts->begin(); stmtIter != stmts->end();
		stmtIter++) {
		(*stmtIter)->print();
	}
	std::cout << "\nAST printed \n";
}

map<string, float> idTable;

void if_stmt::print()
{
	std::cout << "if (";
	boolExp->print();
	std::cout << ") {\n";
	list<statement*>::iterator stmtIter;
	for (stmtIter = stmt->begin(); stmtIter != stmt->end();
		stmtIter++)
	{
		(*stmtIter)->print();
	}
	std::cout << "} " << std::endl;
	if (els != nullptr)
		els->print();
}

void if_stmt::evaluate()
{
	if (boolExp == nullptr)
	{
		std::cout << "IF statement must have a condition!" << std::endl;
		return;
	}
	boolExp->evaluate();
	if (boolExp->val)
	{
		list<statement*>::iterator stmtIter;
		for (stmtIter = stmt->begin(); stmtIter != stmt->end();
			stmtIter++)
		{
			(*stmtIter)->evaluate();
		}
	}
	else if (els != nullptr)
	{
		els->evaluate();
	}
}

void else_stmt::print()
{
	std::cout << "else {\n";
	if (!stmt->empty())
	{
		list<statement*>::iterator stmtIter;
		for (stmtIter = stmt->begin(); stmtIter != stmt->end();
			stmtIter++)
		{
			(*stmtIter)->print();
			std::cout << std::endl;
		}
	}
	std::cout << "} " << std::endl;
}

void else_stmt::evaluate()
{
	if (!stmt->empty())
	{
		list<statement*>::iterator stmtIter;
		for (stmtIter = stmt->begin(); stmtIter != stmt->end();
			stmtIter++)
		{
			(*stmtIter)->print();
		}
	}
}

void while_stmt::print()
{
	std::cout << "while (";
	boolExp->print();
	std::cout << ") {\n";
	list<statement*>::iterator stmtIter;
	for (stmtIter = stmt->begin(); stmtIter != stmt->end();
		stmtIter++)
	{
		(*stmtIter)->print();
	}
	std::cout << "} " << std::endl;
}

void while_stmt::evaluate()
{
	if (boolExp == nullptr)
	{
		std::cout << "WHILE statement must have a condition!" << std::endl;
		return;
	}
	while (boolExp->val)
	{
		list<statement*>::iterator stmtIter;
		for (stmtIter = stmt->begin(); stmtIter != stmt->end();
			stmtIter++)
		{
			(*stmtIter)->evaluate();
		}
		boolExp->evaluate();
	}
}

void for_stmt::print()
{
	std::cout << "for (";
	a->print();
	std::cout << "; ";
	boolExp->print();
	std::cout << "; ";
	exp->print();
	std::cout << "; ";
	std::cout << ") {\n";
	list<statement*>::iterator stmtIter;
	for (stmtIter = stmt->begin(); stmtIter != stmt->end();
		stmtIter++)
	{
		(*stmtIter)->print();
	}
	std::cout << "} " << std::endl;
}

void for_stmt::evaluate()
{
	int i = 0;
	list<statement*>::iterator stmtIter;
	a->evaluate();
	i = idTable[a->getID()];
	do
	{
		boolExp->evaluate();
		if (!boolExp->val)
			break;

		for (stmtIter = stmt->begin(); stmtIter != stmt->end();
			stmtIter++)
		{
			(*stmtIter)->evaluate();
		}

		exp->evaluate();
		i = (int)exp->num;

	} while (1);
}
