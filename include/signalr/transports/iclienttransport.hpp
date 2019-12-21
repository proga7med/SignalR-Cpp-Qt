#ifndef ICLIENTTRANSPORT_HPP
#define ICLIENTTRANSPORT_HPP

#include <future>
#include <QString>

#include <QtPromise>
#include <time_delta.h>

#include "signalr/cancellationtoken.hpp"
#include "signalr/negotiationresponse.hpp"

namespace signalr {

class IConnection;
namespace transports {

class IClientTransport {
public:
  virtual QtPromise::QPromise<NegotiationResponse> negotiate(std::shared_ptr<IConnection> pConnection, const QString& connectionData) = 0;
  virtual QtPromise::QPromise<void> start(std::shared_ptr<IConnection> pConnection, const QString& connectionData) = 0;
  virtual QtPromise::QPromise<void> send(std::shared_ptr<IConnection> pConnection, const QString& data, const QString& connectionData) = 0;
  virtual void abort(std::shared_ptr<IConnection> pConnection, const TimeDelta& timeout, const QString& connectionData) = 0;
  virtual QString getName() const = 0;
  virtual bool isKeepAliveSupported() const = 0;
  virtual ~IClientTransport() = default;

};

}
}



#endif //ICLIENTTRANSPORT_HPP
