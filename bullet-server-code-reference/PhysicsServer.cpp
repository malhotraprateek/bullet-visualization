#include "PhysicsServer.h"
#include "PhysicsDebugDrawer.h"
#include <tcp.hpp>
#include <http.hpp>

static net11::scheduler sched;

// Inefficient example of a data sink that reads in data before sending it back
class echosink : public net11::sink
{
	// we need to keep a reference to a connection so we know where to send data back
	net11::tcp::connection *conn;

	// temp storage vector
	std::vector<char> tmp;

public:
	echosink(net11::tcp::connection *in_conn) : conn(in_conn) {}
	~echosink() = default;

	// the drain function is invoked on sinks as soon as data has been made available.
	bool drain(net11::buffer &buf)
	{
		// keep an ok flag as true as long as we haven't encountered
		bool ok = true;

		// now go through the buf as long as we have data to read.
		while (buf.usage())
		{
			// read one byte at a time
			char c = buf.consume();

			// put it in our temp vector
			tmp.push_back(c);
		}

		// send data back if we got any
		if (tmp.size())
		{
			// let's surround our data with [] characters so we can see the packet extent.
			tmp.insert(tmp.begin(), '[');
			tmp.push_back(']');

			// sending data is done by pushing a producer that contains the data we want.
			// (A more advanced producer could be written that reads in data from a file for example)
			conn->producers.push_back(net11::make_data_producer(tmp));

			// reset our tmp buf for next time.
			tmp.clear();
		}

		// as long as there was no parse problems we want to continue receiving data.
		return ok;
	}
};

PhysicsServer::PhysicsServer()
{
}

PhysicsServer::~PhysicsServer()
{
}

bool PhysicsServer::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
	if (mFirstTime)
	{
		if (!mpTcp)
			mpTcp = std::make_shared<net11::tcp>();

		// todo: Create debug drawer object if not already done and assign it to Bullet using btDiscreteDynamicsWorld::setDebugDrawer()
		// todo: Ensure that Bullet calls btDiscreteDynamicsWorld::debugDrawWorld() at every frame

		auto failed = net11::http::start_server(*mpTcp, port, [&](net11::http::connection &c) -> net11::http::action {
			// Start a web socket on port 4321
			auto socket = net11::http::make_websocket(c, 4321, [&](net11::http::websocket &ws, std::vector<char> &msg) -> bool {
				std::string request(msg.begin(), msg.end());
				if (request == "Ready")
				{
					std::string jsonDump = "";

					// todo: Get a reference/pointer to the debug drawer object

					// todo: Call GetJsonData() on it

					// todo: Call dump() on the json data object received and store returned string in jsonDump

					ws.send(jsonDump);
				}
				return true;
			});

			return socket;
		});

		if (failed)
		{
			std::cout << "Could not establish a tcp connection to localhost @ port: " << port << std::endl;
			return false;
		}

		mFirstTime = false;
	}

	if (mNetworkActive)
	{
		if (mpTcp)
		{
			// Keep checking for any new requests made by client
			mNetworkActive = mpTcp->poll();

			if (!mNetworkActive)
			{
				std::cout << "Tcp polling failed!" << std::endl;
				return false;
			}
		}
	}

	return true;
}