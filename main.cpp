#include "DFA.h"
#include <fstream>

int main()
{
	string re;
	cout << "input regex:";
	cin >> re;
	DFA dfa(re);
	string text;

	cout << "input text:";
	cin >> text;
	dfa.match(text);
	return 0;
}
