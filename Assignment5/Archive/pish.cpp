
#include "hash.h"
#include "Scanner.h"
#include "Parse.h"
#include "assert.h"
#include<string>
#include<stdlib.h>
#include <cstdlib>
#include<cmath>
#include<stdexcept>
#include <iostream>
using namespace std;

#define tabstop 4



class var {
    public:
    string key;
    int value;
    var(string k,int v)
    {
        value=v;
        key=k;
    }
        // * * * add some more attributes and methods here * * * //

};



// * * * declare more things here * * * //


Set<var> memmap;
int run(StmtsNode *stmts);

int conversion (const char ch)
{
    int x;
    if(ch=='0')
        x=0;
    else if(ch=='1')
        x=1;
    else if (ch=='2')
        x=2;
    else if(ch=='3')
        x=3;
    else if(ch=='4')
        x=4;
    else if(ch=='5')
        x=5;
    else if(ch=='6')
        x=6;
    else if(ch=='7')
        x=7;
    else if(ch=='8')
        x=8;
    else
        x=9;
    return x;
}

int evaluate_exp(ExpnNode *ptr){
    if (ptr==NULL)
    {
        return 0;
    }
    
    int left_result= evaluate_exp(ptr->left_operand);
    if(left_result==0 && ptr->tok==asttok)
    {
        return 0;
    }
    if(left_result==0 && ptr->tok==slashtok)
    {
        return 0;
    }
    if(left_result==0 && ptr->tok==andtok)
    {
        return 0;
    }
    if(left_result==1 && ptr->tok==ortok)
    {
        return 1;
    }
    int right_result= evaluate_exp(ptr->right_operand);
    if(ptr->tok==lttok)
    {
        if(left_result<right_result)
            return 1;
        else
            return 0;
    }
    else if(ptr->tok==gttok)
    {
        if(left_result>right_result)
            return 1;
        else
            return 0;
    }
    else if(ptr->tok==eqtok)
    {
        
        if(left_result==right_result)
            return 1;
        else
            return 0;
    }
    else if(ptr->tok==netok)
    {
        if(left_result!=right_result)
            return 1;
        else
            return 0;
    }
    else if(ptr->tok==letok)
    {
        if(left_result<=right_result)
            return 1;
        else
            return 0;
    }
    else if(ptr->tok==getok)
    {
        if(left_result>=right_result)
            return 1;
        else
            return 0;
    }
    else if (ptr->tok==nottok)
    {
        if (right_result==0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if(ptr->tok==asttok)
    {
        return (left_result)*(right_result);
    }
    else if(ptr->tok==andtok)
    {
        if (left_result && right_result)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if(ptr->tok==slashtok)
    {
        if (right_result==0)
        {
            throw runtime_error("Divison by 0 is not allowed");
        }
        if (right_result>=0 && left_result <0 )
        {
            double num= static_cast<double>(left_result)/right_result;
            int x= floor(num);
            return x;
            
        }
        if (left_result>=0 && right_result <0 )
        {
            double num= static_cast<double>(left_result)/right_result;;
            int x= floor(num);
            return x;
        }
        else
        {
            return (left_result)/(right_result);
        }
    }
    if(ptr->tok==mitok)
    {
            return (left_result)-(right_result);
    }
    if(ptr->tok==pltok)
    {
        return (left_result)+(right_result);
    }
    if (ptr->tok==integer)
    {
        int number=0;
        for (int i=0;i<ptr->text.length();i++)
        {
            number=number+conversion(ptr->text[i])*pow(10,ptr->text.length()-i-1);
        }
        return number;
    }
    if (ptr->tok==ident)
    {
        if (memmap.search(ptr->text)==NULL)
        {
            throw runtime_error("not defined variable");
        }
        return memmap.search(ptr->text)->value;
    }
}
int run(StmtsNode *stmts)
{
    if (stmts==NULL)
        return 0;
    if(stmts->stmt->tok==printtok)
    {
        int result=evaluate_exp(stmts->stmt->expn);
        cout<<result<<endl;
    }
    else if(stmts->stmt->tok==asgntok)
    {
        int result= evaluate_exp(stmts->stmt->expn);
        var * temp= new var(stmts->stmt->ident,result);
        memmap.insert(temp);
    }
    else if(stmts->stmt->tok==iftok)
    {
        if(evaluate_exp(stmts->stmt->expn))
        {
            run(stmts->stmt->stmts);
        }
            
    }
    else if(stmts->stmt->tok==whiletok)
    {
        int result=0;
        result=evaluate_exp(stmts->stmt->expn);
        while(result !=0)
        {
            run(stmts->stmt->stmts);
            result=evaluate_exp(stmts->stmt->expn);
        }
    }
    run(stmts->stmts);
    
}



///////////////////////////////////////////////
//----- carried over from testParse.cpp -----//
///////////////////////////////////////////////

// Desc:  These 3 functions return a string which is a pretty-printed
//        version of the subtree rooted there.
string printE(ExpnNode *root);
string printStmts(StmtsNode *root, int ilevel);
string print(StmtsNode *root);


// Desc:  Generate pretty printed pish of the parse tree rooted at root.
string print(StmtsNode *root) {
    return printStmts(root, 0);  // call helper
}


// Desc:  Generate pretty printed pish, by traversing like a linked list.
//        Parameter ilevel is the indentation level.
string printStmts(StmtsNode *root, int ilevel) {
    // generate leading tab
    string spc = "", tab = "";
    for (int i = 0; i < tabstop; i++) spc += " ";
    for (int i = 0; i < ilevel; i++) tab += spc;


    // traverse StmtsNode * like it is a linked list
    StmtsNode *cur = root;
    string accum = "";  // pretty-printed version


    while (cur != NULL) {
        assert(cur->stmt != NULL);

        // print statement
        if (cur->stmt->tok == printtok) {
            accum += tab + toktotext(printtok) + " " + printE(cur->stmt->expn) + toktotext(sctok) + "\n";
        }
        // assignment statement
        else if (cur->stmt->tok == asgntok) {
            accum += tab + cur->stmt->ident + " " + toktotext(asgntok) + " " + printE(cur->stmt->expn) + toktotext(sctok) + "\n";
        }
        // while statement: tab level increases for subStmts
        else if (cur->stmt->tok == whiletok) {
            accum += tab + toktotext(whiletok) + " " + printE(cur->stmt->expn) + " " + toktotext(lctok) + "\n" + printStmts(cur->stmt->stmts, ilevel+1) + tab + toktotext(rctok) + "\n";
        }
        // if/elif/else statement: tab level increases for subStmts
        else if (cur->stmt->tok == iftok) {
            StmtNode *elif = cur->stmt;
            while (elif != NULL) {
                accum += tab + toktotext(elif->tok) + ((elif->tok == elsetok) ? "" : (" " + printE(elif->expn))) + " " + toktotext(lctok) + "\n" + printStmts(elif->stmts, ilevel+1) + tab + toktotext(rctok) + "\n";
                elif = elif->elif;
            }
        }
        // break statement
        else if (cur->stmt->tok == breaktok) {
            accum += tab + toktotext(breaktok) + toktotext(sctok) + "\n";
        }
        else { assert(0); }


        // next node in the chain
        cur = cur->stmts;
    }
    return accum;
}


// Desc:  Generate bracketed infix via postorder traversal
string printE(ExpnNode *root) {
    if (root == NULL) return "";
    if (root->text != "") return root->text;
    if (root->left_operand == NULL) return "(" + toktotext(root->tok) + ((root->tok == nottok) ? " " : "") + printE(root->right_operand) + ")";
    else return "(" + printE(root->left_operand) + " " + toktotext(root->tok) + " " + printE(root->right_operand) + ")";
}


int main () {
    StmtsNode *pTree;
    try {
        pTree = Parse(cin);
    }
    catch(string s) {
        cout << "Parse error: " << s << endl;
        return 0;
    }

    // cout << "Parse Successful!\n";
    run(pTree);
    //cout << print(pTree);
}

