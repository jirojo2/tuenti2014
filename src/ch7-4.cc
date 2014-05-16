/*
 *  Tuenti Challenge #7
 *	José Ignacio Rojo -josi-
 */

#include "util.h"
#include <algorithm>

using namespace std;

struct call
{
	int a;
	int b;
	int idx;

	call() {}
	call(int _a, int _b, int _idx) : a(_a), b(_b), idx(_idx) {}
};

void prev_contacts(int n, vector<call> & calls, vector<int> & plist)
{
	// analizar contactos anteriores de n que no esten en plist
	for (int k = calls.size()-1; k > 0; k--)
	{
		int m = -1;
		if (n == calls[k].a) m = calls[k].b;
		if (n == calls[k].b) m = calls[k].a;
		if (m != -1)
		{
			if (find(plist.begin(), plist.end(), m) == plist.end())
			{
				// anadir contacto y reiterar esta ultima parte
				//plist.insert(plist.begin() + 1, m);

				cout << " " << n << " was connected to " << m << endl;

				plist.push_back(m);
				prev_contacts(m, calls, plist);	
			}
			else
			{
				return;
			}
		}
	}
}

int main(int argc, char** argv)
{
	int source;
	int target;

	// Read Input
	string line;

	getline(cin, line);
	source = stoi(line);
	getline(cin, line);
	target = stoi(line);

	// Read log
	ifstream is ("phone_call.log", std::ifstream::binary);
	int idx = 0;
	vector<int> plist;
	vector<call> calls;
	plist.push_back(source);

	do
	{
		getline(is, line);
		
		vector<string> params = split(line, ' ');

		if (params.size() != 2)
			break;

		int a = stoi(params[0]);
		int b = stoi(params[1]);

		calls.push_back(call(a,b,idx));

		for (int i = 0; i < plist.size(); i++) 
		{
			// check by priority who is connected to the line
			int p = plist[i];
			int n = -1;

			if (p == a) n = b;
			if (p == b) n = a;
			if (n != -1)
			{
				plist.erase(plist.begin() + i);
				plist.push_back(n);

				cout << p << " connected to " << n << endl;

				//
				prev_contacts(n, calls, plist);

				if (n == target)
				{
					cout << "Connected at " << idx << endl;
					return 0;
				}

				break;
			}
		}

		idx++;
	}
	while(is.good());

	cout << "Not connected" << endl;
	return 0;
}

