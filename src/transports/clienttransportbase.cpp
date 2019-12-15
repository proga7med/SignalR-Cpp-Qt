#include "clienttransportbase.hpp"
#include <QException>

namespace signalr {
namespace transports {


std::future<void> ClientTransportBase::start(std::shared_ptr<IConnection> pConnection, const QString& connectionData) {
  throw QException();
}

void ClientTransportBase::stop(std::shared_ptr<IConnection> pConnection, int timeout, const QString& connectionData) {

}


} //end transports
} //end signalr
