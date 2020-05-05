#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <string>
using namespace std;

enum ValType { VINT, VSTR, VERR };

class Value {
    ValType T;
int I;
    string S;
    
public:
    Value() : T(VERR), I(0) {}
    Value(int vi) : T(VINT), I(vi) {}
    Value(string vs) : T(VSTR), I(0), S(vs) {}
    
    ValType GetType() const { return T; }
    bool IsErr() const { return T == VERR; }
    bool IsInt() const { return T == VINT; }
    bool IsStr() const { return T == VSTR; }
    
    int GetInt() const { if( IsInt() ) return I; throw "RUNTIME ERROR: Value not an integer"; }
    string GetStr() const { if( IsStr() ) return S; throw "RUNTIME ERROR: Value not a string"; }
        
    // add op to this
    Value operator+(const Value& op) const
    {
        if(op.GetType() == VINT && this->GetType() == VINT)
        {
           return Value(op.GetInt() + this->GetInt());
        }
        else if(op.GetType() == VSTR && this->GetType() == VSTR)
        {
            return Value(this->GetStr() + op.GetStr());
        } 
        else if(this->GetType() == VINT && op.GetType() == VSTR) 
        {
            return Value(to_string(this->GetInt()) + op.GetStr());
        }
        else if(this->GetType() == VSTR && op.GetType() == VINT)
        {
			return Value(this->GetStr() + to_string(op.GetInt()));
        } 
        else
        {
            throw "Addition Error";
        }
    };
    
    // subtract op from this
    Value operator-(const Value& op) const
    {
        if(this->GetType() == VINT && op.GetType() == VINT)
        {
            return Value(this->GetInt() - op.GetInt());
        } 
        else if(this->GetType() == VSTR && op.GetType() == VSTR) 
        {
            if(this->GetStr().find(op.GetStr()) != string::npos)
            {
                string s1 = this->GetStr();
                string s2 = op.GetStr();
            	 return Value(s1.substr(0, s1.find(s2)) + s1.substr(s1.find(s2) +s2.length()));
            } 
            else 
            {
                return Value(this->GetStr());
            }
        }
        else
        {
            throw "Substraction Error";
        }
    }
    
    // multiply this by op
    Value operator*(const Value& op) const 
    {
        if(this->GetType() == VINT && op.GetType() == VINT)
        {
            return this->GetInt() * op.GetInt();
        } 
        else if(this->GetType() == VSTR && op.GetType() == VINT && op.GetInt() >= 0) 
        {
            string msg = "";
            for(int i = 0; i < op.GetInt(); i++)
            {
                msg += this->GetStr();
            }
            return Value(msg);
        } 
        else if(this->GetType() == VINT && op.GetType() == VSTR && this->GetInt() >= 0) 
        {
            string msg = "";
            for(int i = 0; i < this->GetInt(); i++)
            {
                msg += op.GetStr();
            }
            return Value(msg);
        }
        else 
        {
            throw "Multiplication Error";
        }
    }
    
    // divide this by op
    Value operator/(const Value& op) const 
    {
        if(this->GetType() == VINT && op.GetType() == VINT)
        {
            if(op.GetInt() == 0)
            {
                throw "Error: can't Divide by 0";
            }
            else
            {
                return Value(this->GetInt() / op.GetInt());
            }
        }
        else
        {
            throw "Division Error";
        }
    }
    
    friend ostream& operator<<(ostream& out, const Value& op) {
        if( op.IsInt() ) out << op.I;
        else if( op.IsStr() ) out << op.S;
        else out << "ERROR";
        return out;
    }
};

#endif
