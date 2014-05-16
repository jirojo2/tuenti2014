/*
 *  Tuenti Challenge #7
 *	José Ignacio Rojo -josi-
 */

#include "util.h"
#include <unordered_map>
#include <algorithm>

using namespace std;

class Router;

struct Packet
{
	int from;
	int dest;
	string data;
	int port;
	int last;
};

struct Link
{
	Router* src;
	Router* dst;
	int idx;
	int first_tick;
	int final_tick;

	bool alive(int tick)
	{
		if (tick < 0) return true;
		return (tick >= first_tick && (final_tick < 0 || tick < final_tick));
	}
};

class Router
{
public:
	int uid;
	vector<Link> ports;
	vector<Packet> outbuffer;
	vector<Packet>  inbuffer;
	int port_to_source;
	int tick;

	Router() {}
	Router(int id) : uid(id), port_to_source(-1)
	{
	}

	void init_port(Router* dst, int idx)
	{
		for (int i = 0; i < ports.size(); i++)
			if (ports[i].dst == dst)
				ports[i].final_tick = idx;
		
		Link link;
		link.src = this;
		link.dst = dst;
		link.idx = idx;
		link.first_tick = idx;
		link.final_tick = -1;

		ports.push_back(link);
	}

	virtual void reset()
	{
		outbuffer.clear();
		 inbuffer.clear();
	}

	void send(int dest, string data)
	{
		for (int i = 0; i < ports.size(); i++)
			write_data(i, uid, dest, data);
	}

	virtual void receive(int from, string data)
	{
		// Override
	}

	void read_data(int port, int from, int dest, string data)
	{
		// Read data, if we are the dest, receive it, else enroute it to dest
		// enrouting: copy to all ports but the source one
		// learn :)
		
		if (dest == uid) {
			receive(from, data);
			return;
		}

		port_to_source = port;

		// TTL
		//if (data-- < 0)
		//	return;
		
		data += to_string(uid) + ",";

		for (int i = 0; i < ports.size(); i++)
		{
			if (i != port && ports[i].alive(tick))
			{
				// skip the incomming port
				write_data(i, from, dest, data);
			}
		}
	}

	void write_data(int port, int from, int dest, string data)
	{
		Packet pkt;
		pkt.last = uid;
		pkt.port = port;
		pkt.from = from;
		pkt.dest = dest;
		pkt.data = data;

		outbuffer.push_back(pkt);
	}

	void update(int tick)
	{
		this->tick = tick;

		// Flush outgoing packets
		for (auto& pkt : outbuffer)
		{
			ports[pkt.port].dst->inbuffer.push_back(pkt);
		}
		outbuffer.clear();

		// Read ingoing packets
		for (auto& pkt : inbuffer)
		{
			for (int i = 0; i < ports.size(); i++)
				if (ports[i].dst->uid == pkt.last)
					pkt.port = i; // find incoming port
			read_data(pkt.port, pkt.from, pkt.dest, pkt.data);
			//cout << " Router " << uid << " reads one pkt from " << pkt.from << " through " << ports[pkt.port].dst->uid << endl;
		}
		inbuffer.clear();
	}

	virtual void talk()
	{
		cout << "I am router " << uid << endl;
	}
};

class TargetRouter : public Router
{
public:
	bool done;
	vector<int> route;

	TargetRouter() : Router(), done(false) {}
	TargetRouter(int id) : Router(id), done(false) {}

	void reset() override
	{
		Router::reset();
		done = false;
		route.clear();
	}

	void receive(int from, string data) override
	{
		data = data + to_string(uid);
		for (string& r : split(data, ','))
			route.push_back(stoi(r));

		//cout << "Router " << uid << ": Received message '" << data << "' from " << from << endl;
		done = true;
	}

	void talk() override
	{
		cout << "I am target router " << uid << endl;
	}
};

struct Call
{
	int idx;
	int a;
	int b;

	Call() {}
	Call(int _idx, int _a, int _b) : idx(_idx), a(_a), b(_b)
	{
	}
};

void readLog(vector<Call> & calls)
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

		int a = stoi(params[0]);
		int b = stoi(params[1]);

		calls.push_back(Call(idx++, a, b));
	}
	while(is.good());
}

void initRouters(vector<Call> & calls, unordered_map<int, Router*> & routers, vector<int> & targets)
{
	// Create the routers
	for (int i = 0; i < calls.size(); i++)
	{
		Call call = calls[i];
		routers[call.a] = new Router(call.a);
		routers[call.b] = new Router(call.b);
	}

	for (int i = 0; i < targets.size(); i++)
	{
		routers.erase(targets[i]);
		routers[targets[i]] = new TargetRouter(targets[i]);
	}

	// Initialize their direct ports
	for (int i = 0; i < calls.size(); i++)
	{
		Call call = calls[i];
		routers[call.a]->init_port(routers[call.b], i);
		routers[call.b]->init_port(routers[call.a], i);
	}
}

void readInput(vector<int> & targets)
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
	vector<Call> calls;
	unordered_map<int, Router*> routers;

	readInput(targets);
	readLog(calls);
	initRouters(calls, routers, targets);

	// Simulate
	
	//for (auto& router: routers)
	//	router.second->talk();
	
	// Get the target routers references
	TargetRouter* source = static_cast<TargetRouter*>(routers[targets[0]]);
	TargetRouter* target = static_cast<TargetRouter*>(routers[targets[1]]);

	// Send message to the other target

	int r = -1;

	int connected = 0;
	for (auto& call : calls)
	{
		if (call.a == source->uid) connected++;
		if (call.b == source->uid) connected++;
		if (call.a == target->uid) connected++;
		if (call.b == target->uid) connected++;

		if (connected < 2) continue;

		//cout << "simulating call idx " << call.idx << endl;

		int ticks = 0;

		//cout << " reseting routers" << endl;
		for (auto& it : routers)
			it.second->reset();

		//cout << " sending packet from source" << endl;
		source->send(target->uid, to_string(source->uid) + ",");

		// Cap simulation duration
		while(ticks++ < 100 && !(target->done))
		{
			for (auto& it : routers)
			{
				Router* router = it.second;
				router->update(call.idx);
			}
		}

		// Find out the index of the latest call of the route
		if (target->done)
		{
			//cout << "found something" << endl;
			Router* router = target;
			while (router != source || ticks-- > 0)
			{
				if (router->port_to_source == -1) break;
				Link* link = &router->ports[router->port_to_source];
				router = link->dst;
				//cout << "link idx: " << link->idx << endl;
			}
			r = call.idx;
			break;
		}

		connected -= 1;
		//cout << "not connected yet at idx " << call.idx << endl;
	}

	if (r != -1)
		cout << "Connected at " << r << endl;
	else
		cout << "Not connected" << endl;
	return 0;
}

