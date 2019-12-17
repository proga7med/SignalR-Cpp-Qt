#include "clienttransportbase.hpp"
#include <QException>

namespace signalr {
namespace transports {

ClientTransportBase::ClientTransportBase(std::shared_ptr<http::IHttpClient> pHttpClient, QString transportName)
    : ClientTransportBase(std::move(pHttpClient), std::move(transportName), TransportHelper()){
}

ClientTransportBase::ClientTransportBase(std::shared_ptr<http::IHttpClient> pHttpClient, QString transportName, TransportHelper transportHelper) {
  m_pHttpClient = std::move(pHttpClient);
  m_TransportName = std::move(transportName);
  m_TransportHelper = std::move(transportHelper);
}

std::future<NegotiationResponse> ClientTransportBase::negotiate(std::shared_ptr<IConnection> pConnection, QString connectionData) {
  if(m_Finished) throw QException();
  throw QException(); //return TransportHelper::getNegotiationResponse(pHttpClient, pConnection, connectionData);
}

std::future<void> ClientTransportBase::start(std::shared_ptr<IConnection> pConnection, const QString& connectionData, const CancellationToken& cancellationToken) {
  if(pConnection == nullptr) throw QException();

  //QObject::connect()

  onStart(pConnection, connectionData, cancellationToken);
  throw QException(); //return
}

void ClientTransportBase::transportFailed(const QException& ex) {
  //
}

void ClientTransportBase::abort(std::shared_ptr<IConnection> pConnection, const QElapsedTimer& timeout, const QString& connectionData) {
    m_Finished = true;
    //emit abort
}

bool ClientTransportBase::processResponse(std::shared_ptr<IConnection> pConnection, QString response) {
  if(pConnection == nullptr) throw QException();

  //pConnection->markLastMessage();
  if(response.isNull() || response.isEmpty()) return false;
  auto shouldReconnect = false;

  try {
      //auto result = pConnection->deserializeObject<JsonObject>();
      //TODO:
  }
  catch(const QException& ex) {
      //pConnection->onError(ex);
  }
}


} //end transports
} //end signalr
