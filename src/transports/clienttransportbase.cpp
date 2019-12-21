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

QtPromise::QPromise<NegotiationResponse> ClientTransportBase::negotiate(std::shared_ptr<IConnection> pConnection, const QString &connectionData) {
  if(m_Finished) throw QException();
  throw QException(); //return TransportHelper::getNegotiationResponse(pHttpClient, pConnection, connectionData);
}

QtPromise::QPromise<void> ClientTransportBase::start(std::shared_ptr<IConnection> pConnection, const QString& connectionData) {
  if(pConnection == nullptr) throw QException();

  //QObject::connect()

  onStart(pConnection, connectionData);
  throw QException(); //return
}

void ClientTransportBase::transportFailed(const QException& ex) {
  //
}

void ClientTransportBase::abort(std::shared_ptr<IConnection> pConnection, const TimeDelta& timeout, const QString& connectionData) {
    m_Finished = true;
    //emit abort
}

bool ClientTransportBase::processResponse(std::shared_ptr<IConnection> pConnection, const QString& response) {
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
  return shouldReconnect;
}

QString ClientTransportBase::getName() const {
  return m_TransportName;
}


} //end transports
} //end signalr
