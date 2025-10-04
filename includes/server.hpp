#pragma once

class Server {


	public:

	Server();
	~Server();
	Server(const Server& src);

	Server& operator=(const Server& server);

	void addConnection();
	void endConnection();
};
