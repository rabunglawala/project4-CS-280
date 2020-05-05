/*
 * parse.h
 */

#ifndef PARSE_H_
#define PARSE_H_

#include <iostream>
using namespace std;

#include "lex.h"
#include "pt.h"

extern Pt *Prog(istream& in, int& line);
extern Pt *Sl(istream& in, int& line);
extern Pt *Stmt(istream& in, int& line);
extern Pt *PrintStmt(istream& in, int& line);
extern Pt *PrintlnStmt(istream& in, int& line);
extern Pt *RepeatStmt(istream& in, int& line);
extern Pt *Expr(istream& in, int& line);
extern Pt *Sum(istream& in, int& line);
extern Pt *Prod(istream& in, int& line);
extern Pt *Primary(istream& in, int& line);

#endif /* PARSE_H_ */

