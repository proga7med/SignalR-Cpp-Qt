#include "signalr/negotiationresponse.hpp"

namespace signalr {

QString NegotiationResponse::getConnectionId() const {
  return m_ConnectionId;
}

void NegotiationResponse::setConnectionId(QString connectionId) {
  m_ConnectionId = std::move(connectionId);
}

QString NegotiationResponse::getConnectionToken() const {
  return m_ConnectionToken;
}

void NegotiationResponse::setConnectionToken(QString connectionToken) {
  m_ConnectionToken = std::move(connectionToken);
}

QString NegotiationResponse::getUrl() const {
  return m_Url;
}

void NegotiationResponse::setUrl(QString url) {
  m_Url = std::move(url);
}

double NegotiationResponse::getDisconnectTimeout() const {
  return m_DisconnectTimeout;
}

void NegotiationResponse::setDisconnectTimeout(double disconnectTimeout) {
  m_DisconnectTimeout = disconnectTimeout;
}

bool NegotiationResponse::getTryWebSocket() const {
  return m_TryWebSockets;
}

void NegotiationResponse::setTryWebSocket(bool tryWebSocket) {
   m_TryWebSockets = tryWebSocket;
}


std::optional<double> NegotiationResponse::getKeepAliveTimeout() const {
   return m_KeepAliveTimeout;
}

void NegotiationResponse::setKeepAliveTimeout(std::optional<double> keepAliveTimeout) {
  m_KeepAliveTimeout = std::move(keepAliveTimeout);
}

double NegotiationResponse::getTransportConnectTimeout() const {
  return m_TransportConnectTimeout;
}

void NegotiationResponse::setTransportConnectTimeout(double transportConnectTimeout) {
  m_TransportConnectTimeout = transportConnectTimeout;
}

QString NegotiationResponse::getRedirectUrl() const {
  return m_RedirectUrl;
}

void NegotiationResponse::setRedirectUrl(QString redirectUrl) {
  m_RedirectUrl = std::move(redirectUrl);
}

QString NegotiationResponse::getAccessToken() const {
  return m_AccessToken;
}

void NegotiationResponse::setAccessToken(QString accessToken) {
  m_AccessToken = std::move(accessToken);
}

QString NegotiationResponse::getError() const {
  return m_Error;
}

void NegotiationResponse::setError(QString error) {
   m_Error = std::move(error);
}

}
