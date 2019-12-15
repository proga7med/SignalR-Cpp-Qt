#ifndef ICLIENTTRANSPORT_HPP
#define ICLIENTTRANSPORT_HPP

#include <future>
#include <QString>

namespace signalr {
namespace transports {

class IConnection;
class IClientTransport {
public:
   virtual std::future<void> start(std::shared_ptr<IConnection> pConnection, const QString& connectionData) = 0;
   virtual void stop(std::shared_ptr<IConnection> pConnection, int timeout, const QString& connectionData) = 0;
   virtual ~IClientTransport() = default;
};

}
}



#endif //ICLIENTTRANSPORT_HPP
