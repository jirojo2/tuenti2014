/*
 *  Tuenti Challenge #7
 *	José Ignacio Rojo -josi-
 */

#include "util.h"
#include <unordered_map>
#include <algorithm>

using namespace std;

class Call
{
public:
	int a;
	int b;
	int idx;
};

void processData(vector<Call> & callLog)
{
	ifstream is ("phone_call.log", std::ifstream::binary);
	string line;
	int idx = 0;

	do
	{
		getline(is, line);
		
		Call call;

		vector<string> params = split(line, ' ');

		if (params.size() != 2)
			break;

		call.a = stoi(params[0]);
		call.b = stoi(params[1]);
		call.idx = idx++;

		callLog.push_back(call);
	}
	while(is.good());
}

void processInput(vector<int> & targets)
{
	string line;

	int n = 2;
	for (int i = 0; i < n; i++)
	{
		// Get input
		getline(cin, line);
		targets[i] = stoi(line);
	}
}

int route(vector<Call> & callLog, int a, int b, unordered_map<int, int> & ignore, int depth, unordered_map<int, int> path)
{
	unordered_map<int, int> a1;

	ignore[a] = a;

	if (depth > 5000)
		return -1;

	// First grade connections
	
	for (int i = 0; i < callLog.size(); i++)
	{
		Call* call = &callLog[i];
		
		if (call->a == a)
			a1[call->b] = call->idx;
		if (call->b == a)
			a1[call->a] = call->idx;
	}

	#ifdef DEBUG
	cout << "[depth: " << depth << "] " << a << "'s contacts: " << a1.size() << " - ignore count: " << ignore.size() << endl;
	//cout << "B's contacts: " << b1.size() << endl;
	#endif

	// Interconnections
	
	for (auto& ca : a1)
	{
		#ifdef DEBUG
		cout << "  " << a <<  " contact: " << ca.first << " in call #" << ca.second << endl;
		#endif
		
		path[a] = ca.second;

		if (ca.first == b)
		{
			#ifdef DEBUG
			cout << "found at depth " << depth << endl;
			#endif

			int pmax = 0;
			int n = 0;
			for (auto& p : path)
			{
				if (p.second > pmax)
					pmax = p.second;
				#ifdef DEBUG
				cout << "  " << n++ << " - #" << p.second << ": " << p.first << endl;
				#endif
				cout << p.second << endl;
			}
			#ifdef DEBUG
			cout << "#" << pmax << endl;
			#endif
			return max(ca.second, pmax);
		}

		// Fork!
		if (!ignore.count(ca.first))
		{
			int r = route(callLog, ca.first, b, ignore, depth+1, path);
			if (r != -1)
				return r;
		}
	}

	return -1;
}

int main(int argc, char** argv)
{
	vector<Call> callLog;
	vector<int>  targets(2);
	unordered_map<int, int> path;
	unordered_map<int, int> ignore;

	processData(callLog);
	processInput(targets);
	//processContactRelations(contacts, callLog);
	//int r = route(callLog, &contacts[targets[0]], &contacts[targets[1]], 0);
	int r = route(callLog, targets[0], targets[1], ignore, 0, path);

	//cout << callLog.size() << " calls registered." << endl;
	//cout << "Looking for: " << targets[0] << " and " << targets[1] << endl;
	if (r != -1)
		cout << "Connected at " << r << endl;
	else
		cout << "Not connected" << endl;
	return 0;
}

