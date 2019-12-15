#ifndef CLIENTTRANSPORT_HPP
#define CLIENTTRANSPORT_HPP

#include <QString>
#include "signalr/transports/iclienttransport.hpp"

namespace signalr {
namespace transports {

class ClientTransportBase : public IClientTransport {
public:
    ClientTransportBase() {}

    virtual std::future<void> start(std::shared_ptr<IConnection> pConnection, const QString& connectionData) override;
    virtual void stop(std::shared_ptr<IConnection> pConnection, int timeout, const QString& connectionData) override;

    virtual void onStart(std::shared_ptr<IConnection> pConnection, const QString& connectionData) = 0;

private:
    QString m_TransportName;
    bool m_Finished;
};
}

}




#endif //CLIENTTRANSPORT_HPP
