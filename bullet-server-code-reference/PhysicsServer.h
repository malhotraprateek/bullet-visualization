#pragma once

namespace net11
{
class tcp;
}

class PhysicsServer
{
public:
	PhysicsServer();
	virtual ~PhysicsServer();

	bool update() override;

protected:
	bool mFirstTime = true;
	bool mNetworkActive = true;
	unsigned port = 4321;

	std::shared_ptr<net11::tcp> mpTcp = nullptr;
};