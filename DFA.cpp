#include "Node.h"
#include "DFA.h"
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <stack>
#include <queue>
#include <algorithm>

void DFA::createDfa(const string &re)
{
    int start = getStart();
    int end = getEnd();
    //	cout << start << endl;
    set<int> temp;
    temp.insert(start);
    closure(temp, start);
    Q.push_back(temp);
    q.push(temp);
    Vnode vnode;
    vnode.state = 0;
    dfaTable.push_back(vnode);
    
    int cnt = -1;
    
    while( !q.empty() )
    {
        //		int xx = q.size();
        //		int yy = Q.size();
        //		int dd = dfaTable.size();
        
        temp = q.front();	//DFA当前状态集合，根据该集合建立状态转移图
        //	int tt = temp.size();
        ++cnt;                  //表示DFA当前当前状态
        q.pop();
        set<int>::const_iterator sbegin, send;   //DFA状态里的所有NFA状态
        
        set<char>::const_iterator cbegin, cend;  //正则表达式所有字符集合
        cbegin = charSet.cbegin();
        cend = charSet.cend();
        
        //又正则表达式所有字符集合  遍历DFA当前状态里得所有NFA状态
        while(cbegin != cend)
        {
            set<int> add;
            add.clear();
            char ch = *cbegin;
            sbegin = temp.cbegin();
            send = temp.cend();
            
            while(sbegin != send)   //遍历该DFA状态下的所有NFA状态。
            {
                int index = 0;
                int len = nfa.nfaTable[*sbegin].list.size();
                while(index < len)
                {
                    if((nfa.nfaTable[*sbegin].list)[index].ch == ch)
                    {
                        add.insert((nfa.nfaTable[*sbegin].list)[index].next);
                        closure(add, (nfa.nfaTable[*sbegin].list)[index].next);
                    }
                    ++index;
                }
                ++sbegin;
            }
            //			int zz = add.size();
            if(!add.empty())
            {
                int in = isVisit(add);
                if(in == -1)  //该状态集合未出现过
                {
                    Q.push_back(add);
                    q.push(add);
                    
                    set<int>::iterator ret = add.find(end);
                    Vnode vnode;
                    vnode.state = ret==add.end()?-1:1;
                    dfaTable.push_back(vnode);
                    
                    Node node;
                    node.set(ch, dfaTable.size()-1);
                    dfaTable[cnt].list.push_back(node);
                }
                else
                {
                    Node node;
                    node.set(ch, in);
                    dfaTable[cnt].list.push_back(node);
                }
            }
            ++cbegin;
        }
        
    }
}

int DFA::getStart()
{
    int index = 0;
    int len = nfa.nfaTable.size();
    
    while(index < len)
    {
        if(nfa.nfaTable[index].state == 0)
        {
            break;
        }
        ++index;
    }
    return index;
}

int DFA::getEnd()
{
    int index = 0;
    int len = nfa.nfaTable.size();
    
    while(index < len)
    {
        if(nfa.nfaTable[index].state == 1)
        {
            break;
        }
        ++index;
    }
    return index;
}

void DFA::closure(set<int> &s, int start)
{
    int index = 0;
    int len = nfa.nfaTable[start].list.size();
    for(index=0; index<len; ++index)
    {
        if((nfa.nfaTable[start].list)[index].ch == UN)   // 存在ε闭包
        {
            s.insert( (nfa.nfaTable[start].list)[index].next );
            closure(s, (nfa.nfaTable[start].list)[index].next);  //递归检查ε闭包
            
        }
    }
}

void DFA::createCharSet(const string &re)
{
    int index = 0;
    int len = re.length();
    while(index < len)
    {
        if(isChar(re[index]))
        {
            charSet.insert(re[index]);
        }
        ++index;
    }
    //	setTraverse(charSet);
}

int DFA::isVisit(set<int> &s)
{
    vector<set<int>>::iterator mid =  find(Q.begin(), Q.end(), s);
    return mid==Q.end()?-1:mid-Q.begin();
}

void DFA::traverseDfa()
{
    cout << "DFA:" << endl;
    int index1 = 0;
    int len1 = dfaTable.size();
    
    while(index1 < len1)
    {
        int index2 = 0;
        int len2 = dfaTable[index1].list.size();
        
        while(index2 < len2)
        {
            string state = dfaTable[index1].state==0?"    start":dfaTable[(dfaTable[index1].list)[index2].next].state==1?"    end":"";
            cout << index1 << "-- " << (dfaTable[index1].list)[index2].ch << " -->" << (dfaTable[index1].list)[index2].next;
	    cout << "    " << dfaTable[index1].state << "  " << dfaTable[(dfaTable[index1].list)[index2].next].state << endl;
            ++index2;
        }
        ++index1;
    }
}

int DFA::transfer(int start, char ch)
{
    int index = 0;
    int len = dfaTable[start].list.size();
    
    while(index < len)
    {
        if( (dfaTable[start].list)[index].ch == ch )
        {
            return (dfaTable[start].list)[index].next;
        }
        ++index;
    }
    return -1;
}

void DFA::clearStack(stack<Point> & istack)
{
    while(!istack.empty())
    {
        istack.pop();
    }
}



void DFA::match(string &text)
{
    int index = 0;
    int len = text.size();
    int next;
    while(index < len)
    {
        int state = 0;
        stack<Point> istack;
        clearStack(istack);
        int startIndex = index;
        istack.push(Point(index, state));
        next = transfer(state, text[index]);
        
        while(next != -1 && index < len)
        {
            /*
             if(dfaTable[next].state == 1)
             {
             clearStack(istack);
             }
             */
            ++index;
            state = next;
            Point point(index, state);  //分别表示当前text下标和当前DFA状态
            istack.push(point);
            if(index != len)
            {
                next = transfer(state, text[index]);
            }
        }
        
        while(!istack.empty())
        {
            Point top = istack.top();
            if(dfaTable[top.end].state != 1)
            {
                istack.pop();
                index = top.start;
            }
            else
            {
                string res = text.substr(startIndex, top.start-startIndex);
                index = top.start;
		--index;
                clearStack(istack);
                cout << res << endl;
            }
        }
        ++index;
        
    }
}

void DFA::extend(const string &re, string &ere)
{
	int len = re.size();
	int index;
	for(index=0; index<len; ++index)
	{
		if( re[index]==92 && index+1<len && re[index+1]=='d' )
		{
			ere += "0|1|2|3|4|5|6|7|8|9";
			++index;
		}
		else if( re[index]=='+' )
		{
			if(index-1>=0 && re[index-1]!=')')
			{
				ere.push_back(re[index-1]);
				ere.push_back('*');
			}
			else 
			{
				int i;
				int len2 = ere.size();
				for(i=len2-1; re[i]!='('; --i);
				for(; i<len2; ++i)
				{
					ere.push_back(ere[i]);
				}
				ere.push_back('*');
			}
		}
		else
		{
			ere.push_back(re[index]);
		}
	}
}
