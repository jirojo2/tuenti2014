/*
 *  Tuenti Challenge #7
 *	José Ignacio Rojo -josi-
 */

#include "util.h"
#include <unordered_map>
#include <algorithm>
#include <thread>

using namespace std;

class Contact
{
public:
	int uid;
	Contact* to_source;
	Contact* to_target;

	Contact() {}
	~Contact() {}

	Contact(int id) : uid(id), to_source(0), to_target(0)
	{
	}

	bool knows_source()
	{
		return to_source != 0;
	}

	bool knows_target()
	{
		return to_target != 0;
	}
};
typedef unordered_map<int, Contact> ContactList;

class Call
{
public:
	Contact* a;
	Contact* b;
	int idx;

	Call() {}
	~Call() {}
};

class CallLog
{
public:
	vector<Call> log;

	CallLog() {}
	~CallLog() {}

	vector<Call*> find_calls(Contact* c)
	{
		vector<Call*> calls;
		for (auto& call : log)
		{
			if (call.a->uid == c->uid)
				calls.push_back(&call);
			if (call.b->uid == c->uid)
				calls.push_back(&call);
		}
		return calls;
	}

	void add(Call call)
	{
		log.push_back(call);
	}
};

void enroute(Contact* source, Contact* target, Contact* contact, ContactList & contacts, CallLog & callLog)
{
	//cout << "processing contact " << contact->uid << endl;

	// Process source and target contact lists and find a route between them
	vector<Call*> calls = callLog.find_calls(contact);
	for (auto& c : calls)
	{
		bool knew = false;

		Contact* next = 0;
		Contact* prev = 0;

		if(c->a->uid == contact->uid) 
		{
			next = c->b;
			prev = c->a;
			knew = next->knows_source();
			next->to_source = c->a;
		}
		else
		{
			next = c->a;
			prev = c->b;
			knew = next->knows_source();
			next->to_source = c->b;
		}

		if (source != nullptr)
		{
			knew = next->knows_source();
			next->to_source = prev;

			if (next->knows_target())
			{
				// Spread knows_source
				Contact* aux1 = next;
				Contact* aux2 = next->to_target;
				while (aux2 == aux1)
				{
					aux2->to_source = aux1;
					aux1 = aux2;
					aux2 = aux1->to_target;
				}
			}
		}
		else
		{
			knew = next->knows_target();
			next->to_target = prev;

			if (next->knows_source())
			{
				// Spread knows_source
				Contact* aux1 = next;
				Contact* aux2 = next->to_source;
				while (aux2 == aux1)
				{
					aux2->to_target = aux1;
					aux1 = aux2;
					aux2 = aux1->to_source;
				}
			}
		}

		//cout << "found " << calls.size() << " calls for contact " << contact->uid << endl;

		if (knew || next->uid == target->uid)
			continue;

		// recursive -> source
		enroute(source, target, next, contacts, callLog);
	}
}

void launch(Contact* source, Contact* target, Contact* contact, ContactList * contacts, CallLog * callLog)
{
	enroute(source, target, source, *contacts, *callLog);
}

int processData(CallLog & callLog, ContactList & contacts, vector<int> targets)
{
	ifstream is ("phone_call.log", std::ifstream::binary);
	string line;
	int idx = 0;

	do
	{
		getline(is, line);
		
		vector<string> params = split(line, ' ');

		if (params.size() != 2)
			break;

		Call call;

		int aid = stoi(params[0]);
		int bid = stoi(params[1]);
		call.idx = idx++;

		Contact* a;
		Contact* b;

		if (!contacts.count(aid))
			contacts[aid] = Contact(aid);
		a = &contacts[aid];

		if (!contacts.count(bid))
			contacts[bid] = Contact(bid);
		b = &contacts[bid];

		call.a = a;
		call.b = b;

		callLog.add(call);
	}
	while(is.good());

	Contact* source = &contacts[targets[0]];
	Contact* target = &contacts[targets[1]];

	thread first (launch, source, nullptr, source, &contacts, &callLog);
	thread second(launch, nullptr, target, target, &contacts, &callLog);

	cout << "Launched threads" << endl;

	first .join();
	second.join();

	// If there is a route, find the binding call
	if (target->knows_source()) return 0;	
	return -1;
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
	vector<int>  targets(2);
	CallLog callLog;
	ContactList contacts;

	processInput(targets);
	int r = processData(callLog, contacts, targets);

	if (r != -1)
		cout << "Connected at " << r << endl;
	else
		cout << "Not connected" << endl;
	return 0;
}

