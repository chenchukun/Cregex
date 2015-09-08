#ifndef DFA_H
#define DFA_H
#include "Node.h"
#include "NFA.h"
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <stack>
#include <queue>
#include <algorithm>
using namespace std;

class DFA
{
public:
    DFA(const string & re){extend(re, ere); cout << ere << endl; nfa.createNfa(ere); createCharSet(ere); createDfa(ere);}
    DFA() = default;
    void createDfa(const string &re);
    void traverseDfa();
    void match(string &text);
    
private:
    vector<Vnode> dfaTable;  // DFA有向图邻接矩阵

    vector<set<int>> Q;    //保存创建DFA过程的状态集合
    
    queue<set<int>> q;     //保存创建DFA过程中状态集合的队列
    
    set<char> charSet;   //保存输入的正则表达式字符集
    
    NFA nfa;

    string ere;
    
    void createCharSet(const string &re);
    int getStart();
    int getEnd();
    void closure(set<int> &s, int start);
    int isVisit(set<int> &s);
    inline bool isChar(char ch);
    int transfer(int start, char ch);
    void clearStack(stack<Point> & istack);
    void extend(const string &re, string &ere);
 
};

inline bool DFA::isChar(char ch)
{
    return ch!='(' && ch!=')' && ch!='*' && ch!='|';
}

#endif
