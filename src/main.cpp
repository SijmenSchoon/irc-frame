#include "socket.h"
#include "irc.h"

class MyEventHandler : public EventHandler {
public:
	void onMessageReceived(IRCUser from, std::string channel, std::string message) {
		printf("<%s:%s> %s", channel.c_str(), from.Nickname.c_str(), message.c_str());
	};

	void onNumericReceived(int numeric, std::string message) {
		printf("{%3d} \"%s\"\n", numeric, message.c_str());
	};

	bool onPingReceived(std::string code) {
		printf("[PING] %s\n", code.c_str());
		return true;
	};
};

int main() {
	IRCConnection irc("efnet.xs4all.nl", 6667, "irc-frame");
	irc.eventHandler = new MyEventHandler();
	irc.JoinThread();
}
