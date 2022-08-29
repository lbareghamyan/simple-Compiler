#pragma once
#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <list>

using namespace std;

enum BOOL_OP
{
	LessEQ = 1,
	GrEQ = 2,
	Equal = 3,
	NEqual = 4,
	Greater = 5,
	Less = 6,
	AND_OP = 7,
	OR_OP = 8
};


class exp_node {
public:
	float num;
	bool val;
	virtual void print() = 0;
	virtual float evaluate() = 0;
};

class operator_node : public exp_node {
public:
	exp_node* left;
	exp_node* right;
	operator_node(exp_node* L, exp_node* R);
};



class number_node : public exp_node {

public:
	number_node(float value);
	void print();
	float evaluate();
};

class id_node : public exp_node {
protected:
	string id;

public:
	id_node(string value);
	void print();
	float evaluate();
};

class bool_op_node : public operator_node
{
public:
	BOOL_OP opType;
	bool_op_node(BOOL_OP type, exp_node* L, exp_node* R)
		:operator_node(L, R), opType(type) {}
	void print();
	float evaluate();
};
class plus_node : public operator_node {
public:

	plus_node(exp_node* L, exp_node* R);
	void print();
	float evaluate();
};

class times_node : public operator_node {
public:

	times_node(exp_node* L, exp_node* R);
	void print();
	float evaluate();
};

class minus_node : public operator_node {
public:

	minus_node(exp_node* L, exp_node* R);
	void print();
	float evaluate();
};

class divided_node : public operator_node {
public:

	divided_node(exp_node* L, exp_node* R);
	void print();
	float evaluate();
};

class statement {
public:
	virtual void print() {}
	virtual void evaluate() = 0;
};


class else_stmt : public statement
{
	list<statement*>* stmt;
public:
	else_stmt(list<statement*>* elseStmt)
		: stmt(elseStmt) {}

	void print();
	void evaluate();
};

class assignment_stmt : public statement {
protected:
	string id;
	exp_node* exp;
public:
	string getID()
	{
		return id;
	}
	assignment_stmt(string name, exp_node* expression);
	void print();
	void evaluate();
};

class if_stmt : public statement
{
protected:
	exp_node* boolExp;
	list<statement*>* stmt;
	else_stmt* els;

public:
	if_stmt(exp_node* exp, list<statement*>* st, else_stmt* el = nullptr)
		: boolExp(exp), stmt(st), els(el) {}
	void print();
	void evaluate();
};

class while_stmt : public statement
{
	exp_node* boolExp;
	list<statement*>* stmt;
public:
	while_stmt(exp_node* exp, list<statement*>* st)
		: boolExp(exp), stmt(st) {}
	void print();
	void evaluate();
};

class for_stmt : public statement
{
	assignment_stmt* a;
	exp_node* boolExp;
	exp_node* exp;
	list<statement*>* stmt;
public:
	for_stmt(assignment_stmt* as, exp_node* bExp,
		exp_node* ep, list<statement*>* st)
		: a(as), boolExp(bExp), exp(ep), stmt(st) {}
	void print();
	void evaluate();
};


class print_stmt : public statement {
protected:
	string id;
public:
	print_stmt(string id);
	void evaluate();
};

class pgm {
protected:
	list<statement*>* stmts;
public:
	pgm(list<statement*>* stmtlist);
	void evaluate();
	void print();
};

extern map<string, float> idTable;
extern pgm* root;



