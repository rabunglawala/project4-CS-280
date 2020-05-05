/*
 * pt.h
 */

#ifndef PT_H_
#define PT_H_

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "val.h"
#include "parse.h"
#include "lex.h"

using std::vector;
using std::map;
using std::string;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

enum Type{ERROR,INT,STR,ID};

class Pt {
protected:
	int	linenum;
	Pt	*left;
	Pt	*right;

public:
	Pt(int linenum, Pt *l = 0, Pt *r = 0)
		: linenum(linenum), left(l), right(r) {}

	virtual ~Pt() {
		delete left;
		delete right;
	}

	int GetLineNumber() const { return linenum; }
    virtual bool isPlus() const { return false; }
    virtual bool isEq() const { return false; }
	virtual bool isIdent() const { return false; }
	virtual bool isVar() const { return false; }
	virtual string getId() const { return ""; }
    
    virtual Value Eval(map<string,Value> & Symbol)=0;
    
    int MaxDepth() const
    {
		int lc = 0;
        int rc = 0;
		if( left ) lc += left->MaxDepth();
		if( right ) rc += right->MaxDepth();
		/* Saying that the left side has to be bigger than the right*/
        if (lc > rc)
            return (lc + 1);
        else 
            return (rc + 1);
        return 0;
	}
    int plusCount()const
    {
        int plus=0;
        if( left ) plus += left->plusCount();
		if( right) plus += right->plusCount();
        if(isPlus())
        {
            plus++;
        }
        return plus;
    }
    int EqCount()const
    {
        int EQ=0;
        if( left ) EQ += left->EqCount();
		if( right) EQ += right->EqCount();
        if(isEq())
        {
            EQ++;
        }
        return EQ;
    }   

};
class StmtList : public Pt {
public:
	StmtList(Pt *l, Pt *r) : Pt(0, l, r) {}
    Value Eval(map<string,Value> &Symbol)
    {
        if(left)
        {
            Value L=left->Eval(Symbol);
            if(L.IsErr())
            {
                cout<<linenum << ": "<<L<<endl;
                exit(0);
            }
        }
        if(right)
        {
            right->Eval(Symbol);
        }
        return Value();
    }
};

class Print : public Pt {
public:
	Print(int line, Pt *e) : Pt(line, e) {}
    Value Eval(map<string,Value> &Symbol) 
    {
        Value L=left->Eval(Symbol);
        if(!L.IsErr())
        {
            cout<<L;
        }
        return Value(L);
    }
};

class Println : public Pt {
// Edited:
public:
	Println(int line, Pt *e) : Pt(line, e) {}
    Value Eval(map<string,Value> &Symbol) 
    {
        Value L=left->Eval(Symbol);
        if(!L.IsErr())
        {
            cout<<L<<endl;
        }
        return Value(L);
    }
};

class Repeat : public Pt {
// Edited:
    public:
	Repeat(int line, Pt *e,Pt *stmt) : Pt(line, e,stmt) {}
	 Value Eval(map<string,Value> &Symbol) 
     {
         Value L=left->Eval(Symbol);
         if(L.GetType() != VINT)
         {
             cout<<"RUNTIME ERROR at "<<linenum+1<<": "<<"control expression not an integer"<<endl;
             return 0;
         }
         else
         {
             Value R;
             for(int i=0;i<L.GetInt();i++)
             {
                 R=right->Eval(Symbol);
                 linenum--;
             }
             return 0;
         }
     }
};

class Assign : public Pt {
 // Edited:
    public:
	Assign(int line, Pt *lhs, Pt *rhs) : Pt(line, lhs, rhs) {}
	virtual bool isEq() const { return true; }
    
     Value Eval(map<string,Value> &Symbol) 
     {
         if(left->isIdent())
         {
             string id = left->getId();
             Value R = right->Eval(Symbol);
             Symbol[id]=R;
             return R;
         }
         else
         {
             throw "RUNTIME ERROR: LEFT SIDE IS NOT AN ID";
             exit(0);
         }      
         
     }
};

class PlusExpr : public Pt {
public:
// Edited:
    PlusExpr(int line, Pt *l, Pt *r) : Pt(line,l,r) {}    
    virtual bool isPlus() const { return true; }
    
    Value Eval(map<string,Value> & Symbol) 
    {
        try
        {
            return left->Eval(Symbol) + right->Eval(Symbol);
        }
            catch (const char* msg)
            {
                cout<<"RUNTIME ERROR at "<<linenum +1<<": Type Mismatch on Addition"<<endl;
            exit(0);
        }
    }
    

};

class MinusExpr : public Pt {
// Edited:
    public:
    MinusExpr(int line, Pt *l, Pt *r) : Pt(line,l,r) {}
     Value Eval(map<string,Value> & Symbol) 
    {
        try
        {
            return left->Eval(Symbol) - right->Eval(Symbol);
        }
            catch (const char* msg)
            {
                cout<<"RUNTIME ERROR at "<<linenum +1<<": Type Mismatch on Substraction"<<endl;
            exit(0);
        }
    }
    

    
    //

};

class TimesExpr : public Pt {
// Edited:
    public:
	TimesExpr(int line, Pt *l, Pt *r) : Pt(line,l,r) {}
     Value Eval(map<string,Value> & Symbol) 
    {
        try
        {
            return left->Eval(Symbol) * right->Eval(Symbol);
        }
            catch (const char* msg)
            {
                cout<<"RUNTIME ERROR at "<<linenum +1<<": Type error on multiplication"<<endl;
            exit(0);
        }
    }
    

};

class DivideExpr : public Pt {
// FIXME add code
    public:
	DivideExpr(int line, Pt *l, Pt *r) : Pt(line,l,r) {}
     Value Eval(map<string,Value> & Symbol) 
    {
        try
        {
            return left->Eval(Symbol) / right->Eval(Symbol);
        }
            catch (const char* msg)
            {
                cout<<"RUNTIME ERROR at "<<linenum +1<<": Type Mismatch on Division"<<endl;
            exit(0);
        }
    }
    
    


};

class IConst : public Pt {
	int val;

public:
	IConst(Tok& t) : Pt(t.GetLinenum()) {
		val = stoi(t.GetLexeme());
	}
    Value Eval(map<string,Value> &Symbol) 
    {
        return Value(val);
    }
};

class SConst : public Pt {
	string val;

public:
	SConst(Tok& t) : Pt(t.GetLinenum())
    {
		val = t.GetLexeme();
	}
    virtual Type getType() const{ return STR;}
    Value Eval(map<string,Value> &Symbol) 
    {
        return Value(val);
    }
};

class Ident : public Pt {
// Edited:
    string id;
    int l;

public:
	Ident(Tok& t) : Pt(t.GetLinenum()), id(t.GetLexeme()){}

	 virtual bool isIdent() const { return true; }
     virtual Type getType() const { return ID;}
     virtual string getId() const { return id;}
     Value Eval (map<string,Value> &Symbol) 
     {
         if(Symbol.find(id) != Symbol.end())
         {
             return Symbol[id];
         }
         else
         {
            
             cout<<"RUNTIME ERROR at "<<linenum+1<<": Variable used but not assigned"<<endl;
             exit(0);
         }
     }
   
};

#endif /* PT_H_ */

