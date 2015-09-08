#include "Node.h"
#include "NFA.h"
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <stack>
#include <queue>
#include <algorithm>

void NFA::createNfa(const string &re)  //Thompson 算法
{
    stack<Point> sChar;
    stack<char> sSym;
    int index = 0;
    int len = re.size();
    
    while(index < len)
    {
        if(isChar(re[index])) //当前为字符，则创建两个状态之间的转移 0-- re --> 1
        {
            Vnode start;
            Vnode end;
            Node temp;
            temp.set(re[index], nfaTable.size()+1);
            start.list.push_back(temp);
            nfaTable.push_back(start);
            nfaTable.push_back(end);
            Point point(nfaTable.size()-2, nfaTable.size()-1);
            sChar.push(point);
            if(index+1 < len && (isChar(re[index+1]) || re[index+1]=='(')) // 此中情况要往符号栈中push连接符号
            {							       // 在这之前要先把符号栈中比连接符号高优先级的符号操作完成
                while(!sSym.empty())
                {
                    char top = sSym.top();
                    if(top == '|')
                    {
                        sSym.pop();
                        Point next = sChar.top();
                        sChar.pop();
                        Point prev = sChar.top();
                        sChar.pop();
                        Vnode start;
                        Vnode end;
                        Node temp;
                        temp.set(UN, prev.start);
                        start.list.push_back(temp);
                        temp.set(UN, next.start);
                        start.list.push_back(temp);
                        nfaTable.push_back(start);
                        nfaTable.push_back(end);
                        temp.set(UN, nfaTable.size()-1);
                        nfaTable[prev.end].list.push_back(temp);
                        temp.set(UN, nfaTable.size()-1);
                        nfaTable[next.end].list.push_back(temp);
                        Point point(nfaTable.size()-2, nfaTable.size()-1);
                        sChar.push(point);
                    }
                    else
                    {
                        break;
                    }
                }
                sSym.push(prod);  //push连接符号
            }
        }
        else if(re[index] == '|')
        {
            if(sSym.empty())   //如果为符号|，且栈为空，则push
            {
                sSym.push('|');
            }
            else if(sSym.top() == '(' || sSym.top() == prod) //如果为符号|，且符号栈顶符号优先级比它低，则push
            {
                sSym.push('|');
            }
            else    //否则要先操作所有符号栈中优先级比符号|高或相等的操作
            {
                char top = sSym.top();
                while(!sSym.empty() && (top = sSym.top()) && top == '|')
                {
                    sSym.pop();
                    Point next = sChar.top();
                    sChar.pop();
                    Point prev = sChar.top();
                    sChar.pop();
                    Vnode start;
                    Vnode end;
                    Node temp;
                    temp.set(UN, prev.start);
                    start.list.push_back(temp);
                    temp.set(UN, next.start);
                    start.list.push_back(temp);
                    nfaTable.push_back(start);
                    nfaTable.push_back(end);
                    temp.set(UN, nfaTable.size()-1);
                    nfaTable[prev.end].list.push_back(temp);
                    temp.set(UN, nfaTable.size()-1);
                    nfaTable[next.end].list.push_back(temp);
                    Point point(nfaTable.size()-2, nfaTable.size()-1);
                    sChar.push(point);
                }
/*
                if(top == '(')
                {
                    sSym.pop();
                }
*/
		sSym.push('|');
            }
        }
        else if(re[index] == ')')  //此时要执行符号栈中所有操作知道出现符号(为止
        {
            bool judge = false;
            if(index+1 < len && (isChar(re[index+1]) || re[index+1]=='('))
            {                               //此时该括号的内容与后面剩余串为连接关系
                judge = true;
            }
            char top = sSym.top();
            while(top != '(')
            {
                sSym.pop();
                if(top == '|')
                {
                    Point next = sChar.top();
                    sChar.pop();
                    Point prev = sChar.top();
                    sChar.pop();
                    Vnode start;
                    Vnode end;
                    Node temp;
                    temp.set(UN, prev.start);
                    start.list.push_back(temp);
                    temp.set(UN, next.start);
                    start.list.push_back(temp);
                    nfaTable.push_back(start);
                    nfaTable.push_back(end);
                    temp.set(UN, nfaTable.size()-1);
                    nfaTable[prev.end].list.push_back(temp);
                    temp.set(UN, nfaTable.size()-1);
                    nfaTable[next.end].list.push_back(temp);
                    Point point(nfaTable.size()-2, nfaTable.size()-1);
                    sChar.push(point);
                }
                else if(top == prod)
                {
                    Point next = sChar.top();
                    sChar.pop();
                    Point prev = sChar.top();
                    sChar.pop();
                    Node temp;
                    temp.set(UN, next.start);
                    nfaTable[prev.end].list.push_back(temp);
                    Point point(prev.start, next.end);
                    sChar.push(point);
                    
                }
                top = sSym.top();
            }
            sSym.pop(); //'('出栈
            if(judge)
            {
                sSym.push(prod);
            }
        }
        else if(re[index] == '(')
        {
            sSym.push('(');
        }
        else if(re[index] == '*')  //此时要立即对字符栈顶元素进行闭包操作。
        {
            Point mid = sChar.top();
            sChar.pop();
            Vnode start;
            Vnode end;
            Node temp;
            temp.set(UN, mid.start);
            start.list.push_back(temp);
            temp.set(UN, nfaTable.size()+1);
            start.list.push_back(temp);
            nfaTable.push_back(start);
            nfaTable.push_back(end);
            nfaTable[mid.end].list.push_back(temp);
            temp.set(UN, mid.start);
            nfaTable[mid.end].list.push_back(temp);
            Point point(nfaTable.size()-2, nfaTable.size()-1);
            sChar.push(point);
            if(index+1 < len && (re[index+1]=='(' || isChar(re[index+1])))
            {                             //此时闭包前的内容与剩余串的为连接关系
                while(!sSym.empty())
                {                     //在push入连接符号前要进行比它优先级高德操作
                    char top = sSym.top();
                    if(top == '|')
                    {
                        sSym.pop();
                        Point next = sChar.top();
                        sChar.pop();
                        Point prev = sChar.top();
                        sChar.pop();
                        Vnode start;
                        Vnode end;
                        Node temp;
                        temp.set(UN, prev.start);
                        start.list.push_back(temp);
                        temp.set(UN, next.start);
                        start.list.push_back(temp);
                        nfaTable.push_back(start);
                        nfaTable.push_back(end);
                        temp.set(UN, nfaTable.size()-1);
                        nfaTable[prev.end].list.push_back(temp);
                        temp.set(UN, nfaTable.size()-1);
                        nfaTable[next.end].list.push_back(temp);
                        Point point(nfaTable.size()-2, nfaTable.size()-1);
                        sChar.push(point);
                    }
                    else
                    {
                        break;
                    }
                }
                sSym.push(prod); 
            }
        }
        ++index;
    }
    //	int xx = sSym.size();
    //	int yy = sChar.size();
    //	cout << xx << " " << yy << endl;
    while(!sSym.empty())   //对字符栈中元素进行符号栈中的相关操作
    {
        char top = sSym.top();
        if(top == '|')
        {
            Point next = sChar.top();
            sChar.pop();
            Point prev = sChar.top();
            sChar.pop();
            Vnode start;
            Vnode end;
            Node temp;
            temp.set(UN, prev.start);
            start.list.push_back(temp);
            temp.set(UN, next.start);
            start.list.push_back(temp);
            nfaTable.push_back(start);
            nfaTable.push_back(end);
            temp.set(UN, nfaTable.size()-1);
            nfaTable[prev.end].list.push_back(temp);
            temp.set(UN, nfaTable.size()-1);
            nfaTable[next.end].list.push_back(temp);
            Point point(nfaTable.size()-2, nfaTable.size()-1);
            sChar.push(point);
        }
        else if(top == prod)
        {
            Point next = sChar.top();
            sChar.pop();
            Point prev = sChar.top();
            sChar.pop();
            Node temp;
            temp.set(UN, next.start);
            nfaTable[prev.end].list.push_back(temp);
            Point point(prev.start, next.end);
            sChar.push(point);
        }
        sSym.pop();
    }
    //	cout << sChar.size() << endl;
    index = 0;
    len = nfaTable.size();
    while(index < len)
    {
        nfaTable[index++].state = -1;
    }
    Point point = sChar.top();
    nfaTable[point.start].state = 0;
    nfaTable[point.end].state = 1;
}

void NFA::traverseNfa()
{
    cout << "NFA:" << endl;
    int index1 = 0;
    int len1 = nfaTable.size();
    
    while(index1 < len1)
    {
        int index2 = 0;
        int len2 = (nfaTable[index1].list).size();
        
        while(index2 < len2)
        {
            string state = nfaTable[index1].state==0?"    start":nfaTable[(nfaTable[index1].list)[index2].next].state==1?"    end":"";
            cout << index1 << "-- " << (nfaTable[index1].list)[index2].ch << " -->"
            << (nfaTable[index1].list)[index2].next;
	    cout << "    " << nfaTable[index1].state << "  " << nfaTable[(nfaTable[index1].list)[index2].next].state << endl;
            ++index2;
        }
        ++index1;
    }
}

