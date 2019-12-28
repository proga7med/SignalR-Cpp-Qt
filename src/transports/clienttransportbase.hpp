#ifndef CLIENTTRANSPORT_HPP
#define CLIENTTRANSPORT_HPP

#include <QString>
#include <QElapsedTimer>
#include "transporthelper.hpp"
#include "signalr/http/ihttpclient.hpp"
#include "signalr/negotiationresponse.hpp"
#include "signalr/transports/iclienttransport.hpp"

namespace signalr {
namespace transports {

class ClientTransportBase : public IClientTransport {
protected:
    ClientTransportBase(std::shared_ptr<http::IHttpClient> pHttpClient, QString transportName);

public:
    ClientTransportBase(std::shared_ptr<http::IHttpClient> pHttpClient, QString transportName, TransportHelper transportHelper);
    QtPromise::QPromise<NegotiationResponse> negotiate(std::shared_ptr<IConnection> pConnection, const QString &connectionData);

    virtual QtPromise::QPromise<void> start(std::shared_ptr<IConnection> pConnection, const QString& connectionData) override;
    virtual void abort(std::shared_ptr<IConnection> pConnection, const TimeDelta& timeout, const QString& connectionData);

    virtual QtPromise::QPromise<void> send(std::shared_ptr<IConnection> pConnection, const QString& data, const QString& connectionData) = 0;
    virtual void lostConnection(std::shared_ptr<IConnection> pConnection) override;
    virtual QString getName() const override;

protected:
    virtual void onStart(std::shared_ptr<IConnection> pConnection, const QString& connectionData) = 0;
    virtual void onStartFailed() = 0;

    void transportFailed(const QException& ex);

    virtual bool processResponse(std::shared_ptr<IConnection> pConnection, const QString& response);


private:
  QString m_TransportName;
  bool m_Finished;
  std::shared_ptr<http::IHttpClient> m_pHttpClient;
  TransportHelper m_TransportHelper;
  bool m_SupportKeepAlive;
};
}

}

#endif //CLIENTTRANSPORT_HPP
