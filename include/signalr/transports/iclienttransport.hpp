#ifndef ICLIENTTRANSPORT_HPP
#define ICLIENTTRANSPORT_HPP

#include <future>
#include <QString>
#include "signalr/cancellationtoken.hpp"

namespace signalr {
namespace transports {

class IConnection;

class IClientTransport {
public:
   virtual std::future<void> start(std::shared_ptr<IConnection> pConnection, const QString& connectionData, const CancellationToken& cancellationToken) = 0;
   virtual std::future<void> send(std::shared_ptr<IConnection> pConnection, const QString& data, const QString& connectionData) = 0;
   virtual void abort(std::shared_ptr<IConnection> pConnection, int timeout, const QString& connectionData) = 0; 
   QString getName() const { return m_Name; }
   bool isSupportKeepAlive() const { return m_SupportKeepAlive; }
   virtual ~IClientTransport() = default;

private:
    QString m_Name;
    bool m_SupportKeepAlive;
};

}
}



#endif //ICLIENTTRANSPORT_HPP
