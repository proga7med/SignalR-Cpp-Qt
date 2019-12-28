#include "clienttransportbase.hpp"
#include <QException>

#include "http/defaulthttpclient.hpp"

namespace signalr {
namespace transports {

ClientTransportBase::ClientTransportBase(std::shared_ptr<http::IHttpClient> pHttpClient, QString transportName)
    : ClientTransportBase(std::move(pHttpClient), std::move(transportName), TransportHelper()){
}

ClientTransportBase::ClientTransportBase(std::shared_ptr<http::IHttpClient> pHttpClient, QString transportName, TransportHelper transportHelper) {
  m_Finished = false;
  m_pHttpClient = std::move(pHttpClient);
  m_TransportName = std::move(transportName);
  m_TransportHelper = std::move(transportHelper);
}

QtPromise::QPromise<NegotiationResponse> ClientTransportBase::negotiate(std::shared_ptr<IConnection> pConnection, const QString &connectionData) {
  if(m_Finished) throw QException();
  return m_TransportHelper.getNegotiationResponse(m_pHttpClient, pConnection, connectionData);
}

QtPromise::QPromise<void> ClientTransportBase::start(std::shared_ptr<IConnection> pConnection, const QString& connectionData) {
  if(pConnection == nullptr) throw QException();

  onStart(pConnection, connectionData);
  QtPromise::QPromise<void> task = QtPromise::QPromise<void>([=](const QtPromise::QPromiseResolve<void>& resolve, const QtPromise::QPromiseReject<void>& reject){
    resolve();
  }).delay(pConnection->getTotalTransportConnectTimeout().InMilliseconds())
    .then([=](){
  }).fail([=](){
      throw QException();
  });

  return task;
}

void ClientTransportBase::transportFailed(const QException& ex) {
  //
}

void ClientTransportBase::abort(std::shared_ptr<IConnection> pConnection, const TimeDelta& timeout, const QString& connectionData) {
  m_Finished = true;
  //TODO: Abort.
}

bool ClientTransportBase::processResponse(std::shared_ptr<IConnection> pConnection, const QString& response) {
  if(pConnection == nullptr) throw QException();

  pConnection->markLastMessage();
  if(response.isNull() || response.isEmpty()) return false;
  auto shouldReconnect = false;

  try {
      QJsonObject result = QJsonDocument::fromJson(response.toLatin1()).object();
      if(result.isEmpty()) return false;

      if(!result["I"].isNull()) {
        pConnection->onReceived(result);
        return false;
      }

      if(!result["E"].isNull()) {
        pConnection->onError(QException()); //HubException.
        pConnection->disconnect();
        return false;
      }

      shouldReconnect = result["T"] == 1;

      auto groupsToken = result["G"];
      if(!groupsToken.isNull()) {
        pConnection->setGroupsToken(groupsToken.toString());
      }

      auto messages = result["M"].toArray();
      if(!messages.isEmpty()) {
        pConnection->setMessageId(result["C"].toString());

        for(auto const& message : messages) {
          pConnection->onReceived(message);
        }

        if(result["S"] == 1) {
          m_TransportHelper.getStartResponse(m_pHttpClient, pConnection, QString(), m_TransportName);
        }
      }
  }
  catch(const QJsonDeserializationException& ex) {
     qDebug() << ex.what();
  }
  catch(const QException& ex) {
     pConnection->onError(ex);
  }
  return shouldReconnect;
}

QString ClientTransportBase::getName() const {
  return m_TransportName;
}

void ClientTransportBase::lostConnection(std::shared_ptr<IConnection> pConnection) {

}


} //end transports
} //end signalr
