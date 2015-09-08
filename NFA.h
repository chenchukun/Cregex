#ifndef NFA_H
#define NFA_H
#include "Node.h"
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <stack>
#include <queue>
#include <algorithm>
using namespace std;
class DFA;

class NFA
{
    friend class DFA;
public:
    NFA(const string & re){createNfa(re);}
    NFA() = default;
    void createNfa(const string &re);
    void traverseNfa();
    
private:
    vector<Vnode> nfaTable;
    inline bool isChar(char ch);
};

inline bool NFA::isChar(char ch)
{
    return ch!='(' && ch!=')' && ch!='*' && ch!='|';
}

#endif
