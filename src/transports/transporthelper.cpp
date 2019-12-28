#include "transporthelper.hpp"

#include <QJsonObject>
#include <QJsonDocument>

#include "infra/urlbuilder.hpp"
#include "signalr/http/ihttpclient.hpp"

namespace signalr::transports {

QtPromise::QPromise<NegotiationResponse> TransportHelper::getNegotiationResponse(std::shared_ptr<http::IHttpClient> pHttpClient, std::shared_ptr<IConnection> pIConnection, const QString& connectionData) {
  if(pHttpClient == nullptr) throw QException();
  if(pIConnection == nullptr) throw QException();

  auto negotiateUrl = UrlBuilder::buildNegotiate(pIConnection, connectionData);
  pHttpClient->initialize(pIConnection);

  return pHttpClient->get(negotiateUrl, [=](std::shared_ptr<http::IRequest> request) { pIConnection->prepareRequest(request); }, false)
                    .then([=](std::shared_ptr<http::IResponse> response) { return response->readAsString(response); })
                    .then([=](const QString& raw) {
                        if(raw.isNull() || raw.isEmpty()) throw QException();
                        auto qobj = QJsonDocument::fromJson(raw.toLatin1()).object();
                        if(qobj.contains("availableTransports")) throw QException();
                        NegotiationResponse response = pIConnection->getJsonSerializer()->deserialize<NegotiationResponse>(qobj);
                        return response;
                     });
}

QtPromise::QPromise<QString> TransportHelper::getStartResponse(std::shared_ptr<http::IHttpClient> pHttpClient, std::shared_ptr<IConnection> pIConnection, const QString& connectionData, const QString& transport) {
  if(pHttpClient == nullptr) throw QException();
  if(pIConnection == nullptr) throw QException();

  auto startUrl = UrlBuilder::buildStart(pIConnection, transport, connectionData);
  return pHttpClient->get(startUrl, [&](std::shared_ptr<http::IRequest> request) { pIConnection->prepareRequest(request); }, false)
         .then([&](std::shared_ptr<http::IResponse> response) { return response->readAsString(response); });
}

bool TransportHelper::verifyLastActive(std::shared_ptr<IConnection> pIConnection) {
  if(pIConnection == nullptr) throw QException();
  if(QDateTime::currentDateTimeUtc() - pIConnection->getLastActiveAt() >= pIConnection->getReconnectWindow()) {
     //trace.
     pIConnection->stop(QException());
     return false;
  }
  return true;
}

}
