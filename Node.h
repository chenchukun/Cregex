#ifndef NODE_H
#define NODE_H
#include <vector>
using namespace std;
const char UN = '~';
const char prod = '.';

typedef struct Node  //NFA状态转换节点
{
    char ch;   //转换字符
    int next;  //转换的状态
    void set(char a, int b){ch = a; next = b;}
}Node;

typedef struct Vnode  //NFA节点
{
    int state;  //保存节点状态，0为开始节点，1为结束节点，-1为普通节点
    vector<Node> list;  //该节点的状态转换节点
}Vnode;

typedef struct Point
{
    int start;
    int end;
    Point(int a, int b):start(a), end(b){}
    Point() = default;
}Point;

#endif