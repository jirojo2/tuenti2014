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

class Tree
{
public:
	int source;
	int target;
	unordered_map<int, int> active_calls;
	unordered_map<int, int> back_calls;
	int connection;
	int depth;

	Tree(int source = 0, int target = 0)
	{
		this->connection = -1;
		this->source = source;
		this->target = target;
		this->depth = 0;

		active_calls[source] = -1;
		  back_calls[target] = -1;
	}

	// Call this method twice per call, swapping a and b
	void process_call(int idx, int a, int b) 
	{
		unordered_map<int, int>::const_iterator searcha = active_calls.find(a);
		unordered_map<int, int>::const_iterator searchb = active_calls.find(b);
		unordered_map<int, int>::const_iterator searchc =   back_calls.find(a);
		unordered_map<int, int>::const_iterator searchd =   back_calls.find(b);
		int status = 0;
		
		// If a or b are the target, and the other is not in the active_calls, we need to fork the process

		// Look for a in active calls
		if (searcha != active_calls.end())
		{
			// Remove a from active, add b with the new call index
			active_calls.erase(searcha);
			active_calls[b] = idx;

			status += 1;
		}
		if (searchb != active_calls.end())
		{
			// Remove b from active, add a with new call index
			active_calls.erase(searchb);
			active_calls[a] = idx;

			status += 2;
		}
		if (searchc != back_calls.end())
		{
			back_calls.erase(searchc);
			back_calls[b] = idx;

			status += 4;
		}
		if (searchd != back_calls.end())
		{
			back_calls.erase(searchd);
			back_calls[a] = idx;

			status += 8;
		}

		if (status != 0)
			depth++;

		if (status > 4 && status != 8)
			connection = idx;
	}

	bool done()
	{
		return connection != -1;
	}
};

void processData(vector<Call> & callLog, Tree & tree)
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
		tree.process_call(call.idx, call.a, call.b);
		if (tree.done())
			break;
	}
	while(is.good());

	int n = 0;
	while(!tree.done())
	{
		for (auto& call : callLog)
		{
			tree.process_call(call.idx, call.a, call.b);
			if (tree.done()) break;
		}
		if (n++ % 500 == 0) cout << "Loop #" << n-1 << " - depth: " << tree.depth << endl;
	}
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

int main(int argc, char** argv)
{
	vector<Call> callLog;
	vector<int>  targets(2);
	unordered_map<int, int> path;
	unordered_map<int, int> ignore;

	processInput(targets);
	
	// Initialize call tree
	Tree tree = Tree(targets[0], targets[1]);

	processData(callLog, tree);

	if (tree.connection != -1)
		cout << "Connected at " << tree.connection << endl;
	else
		cout << "Not connected" << endl;
	return 0;
}

