/*
 * parse.cpp
 */

#include "parse.h"

namespace Parser
{
bool pushed_back = false;
Tok	pushed_token;
int line =0;

static Tok GetNextToken(istream& in, int& line) 
{
	if( pushed_back ) {
		pushed_back = false;
		return pushed_token;
	}
	return getNextToken(in, line);
}

static void PushBackToken(Tok& t) {
	if( pushed_back ) {
		abort();
	}
	pushed_back = true;
	pushed_token = t;
}

}

static int error_count = 0;

void
ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

Pt *Prog(istream& in, int& line)
{
	Pt *sl = Sl(in, line);

	if( sl == 0 )
		ParseError(line, "No statements in program");

	if( error_count )
		return 0;
	
    return sl;
}

// Sl is a Statement followed by a Statement List
Pt *Sl(istream& in, int& line) 
{
	Pt *s = Stmt(in, line);
	if( s == 0 )
		return 0;

	if( Parser::GetNextToken(in, line) != SC )
    {
       
		ParseError(line, "Missing semicolon");
		delete s;
		return 0;
	}

	return new StmtList(s, Sl(in,line));
}

Pt *Stmt(istream& in, int& line) 
{
	Pt *s=0;
    Tok t = Parser::GetNextToken(in, line);
    switch( t.GetToken() )
    {
    case PRINT:
        s = PrintStmt(in, line);
        break;
    case PRINTLN:
        s = PrintlnStmt(in, line);
        break;
    case REPEAT:
        s = RepeatStmt(in, line);
        break;
    case DONE:
          return 0;        
    default:
        Parser::PushBackToken(t);
        s = Expr(in, line);
        if(s == 0)
        {
            ParseError(line,"Invalid Statement");
            return 0;
        }
        break;
    }
    return s;

}

Pt *PrintStmt(istream& in, int& line) 
{
	Pt *ex = Expr(in, line);
	if( ex == 0 )
    {
		ParseError(line, "Missing expression after print");
		return 0;
	}

	return new Print(line, ex);
}

Pt *PrintlnStmt(istream& in, int& line)
{

	Pt *ex = Expr(in, line);
	if( ex == 0 ) {
		ParseError(line, "Missing expression after println");
		return 0;
	}

	return new Println(line, ex);
}

Pt *RepeatStmt(istream& in, int& line) 
{
    //check for expression
    Pt *e = Expr(in, line);
    if(e == 0)
    {
        ParseError(line, "Missing expression after loop");
        return 0;
    }
    //check if expression followed by BEGIN
    Tok t1 = Parser::GetNextToken(in, line);
    if(t1 != BEGIN)
    {
        ParseError(line, "Missing BEGIN after expression");
        return 0;
    }
    t1 = Parser::GetNextToken(in, line);
    if(t1 == SCONST || t1 == ICONST)
    {
        ParseError(line, "Missing BEGIN after expression");
        return 0;
    }
    else
    {
        Parser::PushBackToken(t1);
    }
    
    //only executes if expr !=0
    Pt *s = Stmt(in, line);
    if(s ==0)
    {
        ParseError(line, "Missing statement list after BEGIN");
        return 0;
    }
    //check if statement followed by END
    t1 = Parser::GetNextToken(in, line);
    if(t1 != END)
    {
        ParseError(line, "Missing END after statement THIS ONE");
        return 0;
    }
    return new Repeat(line, e, s);
}

Pt *Expr(istream& in, int& line) 
{
    //Tok t = Parser::GetNextToken(in, line);
  
    Pt *t1 = Sum(in,line);
	if( t1 == 0 )
    {
		return 0;
	}
    //check if sum is followed by EQ and EQ followd by sum.
	Tok t = Parser::GetNextToken(in, line);
    if( t == EQ )
    {
       Pt *t2 = Expr(in, line);           
	   if( t2 == 0 ) 
       {
		  ParseError(line, "Missing expression after EQ");
		  return 0;
       }
          return new Assign(t.GetLinenum(), t1, t2);
	}
    else
    {
       Parser::PushBackToken(t);
	   return t1;	
    }
	
}

Pt *Sum(istream& in, int& line)
{
	Pt *t1 = Prod(in, line);
	if( t1 == 0 ) 
    {
		return 0;
	}

	while ( true ) 
    {
		Tok t = Parser::GetNextToken(in, line);
		if( t != PLUS && t != MINUS ) {
			Parser::PushBackToken(t);
			return t1;
		}

		Pt *t2 = Prod(in, line);
		if( t2 == 0 ) {
			ParseError(line, "Missing expression after operator");
			return 0;
		}

		if( t == PLUS )
			t1 = new PlusExpr(t.GetLinenum(), t1, t2);
		else
			t1 = new MinusExpr(t.GetLinenum(), t1, t2);
	}
}

Pt *Prod(istream& in, int& line)
{
     Pt *tProd = Primary(in, line);
    if( tProd == 0 )
    {
		return 0;
	}
    //Checks to see eq star/slash
    while ( true ) 
    {
		Tok t = Parser::GetNextToken(in, line);
		if( t != STAR && t != SLASH ) {
			Parser::PushBackToken(t);
			return tProd;
		}
	 	//checks star/slash followed by prod()
		Pt *tProd2 = Primary(in, line);
		if( tProd2 == 0 )
        {
			ParseError(line, "Missing expression after operator");
			return 0;
		}
		//checks to see eq slash or star
		if( t == SLASH )
			tProd = new DivideExpr(t.GetLinenum(), tProd, tProd2);
		else
			tProd = new TimesExpr(t.GetLinenum(), tProd, tProd2);
	} 
    return 0;
}

Pt *Primary(istream& in, int& line) 
{
	Tok t = Parser::GetNextToken(in, line);
   
	if( t == IDENT ) 
    {
		return new Ident(t);
	}
	else if( t == ICONST )
    {
        return new IConst(t);
    }
    else if( t== SCONST )
    {
        return new SConst(t);
    }
    else if( t == LPAREN )
    {
        Pt *ex = Expr(in, line);  
        //CHecks to see () are missing
        if( ex == 0 )
        {
            ParseError(line, "Missing expression after (");
            return 0;
        }
        Tok t1=Parser::GetNextToken(in, line);
        if( t1 != RPAREN )
        {
           ParseError(line, "Missing Right Parentheses");
            return 0; 
        }
        t1=Parser::GetNextToken(in, line);
        if (t1 == EQ)
        {
          ParseError(line, "Invalid Parantheses");
            return 0;   
        }
        else
        {
        Parser::PushBackToken(t1);
        return ex;
        }
    }
    else
    {
        ParseError(line, "Invalid Primary Token");
        return 0;
    }  

}


