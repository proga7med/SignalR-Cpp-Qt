#ifndef CLIENTTRANSPORT_HPP
#define CLIENTTRANSPORT_HPP

#include <QString>
#include <QElapsedTimer>
#include "transporthelper.hpp"
#include "http/ihttpclient.hpp"
#include "negotiationresponse.hpp"
#include "signalr/transports/iclienttransport.hpp"

namespace signalr {
namespace transports {

class ClientTransportBase : public IClientTransport {
protected:
    ClientTransportBase(std::shared_ptr<http::IHttpClient> pHttpClient, QString transportName);

public:
    ClientTransportBase(std::shared_ptr<http::IHttpClient> pHttpClient, QString transportName, TransportHelper transportHelper);
    std::future<NegotiationResponse> negotiate(std::shared_ptr<IConnection> pConnection, QString connectionData);

    virtual std::future<void> start(std::shared_ptr<IConnection> pConnection, const QString& connectionData, const CancellationToken& cancellationToken) override;
    virtual void abort(std::shared_ptr<IConnection> pConnection, const QElapsedTimer& timeout, const QString& connectionData);

    virtual std::future<void> send(std::shared_ptr<IConnection> pConnection, const QString& data, const QString& connectionData) = 0;
    virtual void lostConnection(std::shared_ptr<IConnection> pConnection) = 0;
protected:
    virtual void onStart(std::shared_ptr<IConnection> pConnection, const QString& connectionData, const CancellationToken& cancellationToken) = 0;
    virtual void onStartFailed() = 0;

    void transportFailed(const QException& ex);

    virtual bool processResponse(std::shared_ptr<IConnection> pConnection, QString response);

private:
    QString m_TransportName;
    bool m_Finished;
    std::shared_ptr<http::IHttpClient> m_pHttpClient;
    TransportHelper m_TransportHelper;
};
}

}




#endif //CLIENTTRANSPORT_HPP
