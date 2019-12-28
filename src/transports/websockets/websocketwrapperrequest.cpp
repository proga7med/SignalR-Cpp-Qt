#include "websocketwrapperrequest.hpp"

namespace signalr::transports::websockets {

WebSocketWrapperRequest::WebSocketWrapperRequest(std::shared_ptr<QWebSocket> pWebSocket, QNetworkRequest& request, std::shared_ptr<IConnection> pIConnection) {
  m_pWebSocket = std::move(pWebSocket);
  m_pIConnection = std::move(pIConnection);
  m_Request = request;
  prepareRequest();
}

void WebSocketWrapperRequest::setRequestHeaders(QList<QPair<QString, QString> > &headers) {
  for (auto const& header : headers) {
    m_Request.setRawHeader(header.first.toLatin1(), header.second.toLatin1());
  }
}

QString WebSocketWrapperRequest::getAccept() const {
  return QString();
}

void WebSocketWrapperRequest::setAccept(QString accept) {

}

QString WebSocketWrapperRequest::getUserAgent() const {
  return QString();
}

void WebSocketWrapperRequest::setUserAgent(QString userAgent) {

}

void WebSocketWrapperRequest::abort() {

}

QAuthenticator WebSocketWrapperRequest::getCredential() const {
  QAuthenticator credential;
  auto authorization = m_Request.rawHeader("Authorization").split(' ');
  if(authorization.isEmpty()) return  credential;

  auto credentialHeader = QString::fromLocal8Bit(authorization.at(1)).split(':'); //test
  if(credentialHeader.isEmpty()) return credential;

  credential.setUser(credentialHeader[0]);
  credential.setPassword(credentialHeader[1]);
  return credential;
}

void WebSocketWrapperRequest::setCredentials(QAuthenticator credential) {
  QString concatenated = credential.user() + ":" + credential.password();
  QByteArray data = concatenated.toLocal8Bit().toBase64();
  QString headerData = "Basic " + data;
  m_Request.setRawHeader("Authorization", headerData.toLocal8Bit());
}

QList<QNetworkCookie> WebSocketWrapperRequest::getCookieContainer() const {
  return m_Request.header(QNetworkRequest::KnownHeaders::CookieHeader).value<QList<QNetworkCookie>>();
}

void WebSocketWrapperRequest::setCookieContainer(QList<QNetworkCookie> cookieContainer) {
  m_Request.setHeader(QNetworkRequest::KnownHeaders::CookieHeader, QVariant::fromValue<QList<QNetworkCookie>>(cookieContainer));
}

QNetworkProxy WebSocketWrapperRequest::getProxy() const {
  return m_pWebSocket->proxy();
}

void WebSocketWrapperRequest::setProxy(QNetworkProxy proxy) {
  m_pWebSocket->setProxy(proxy);
}

void WebSocketWrapperRequest::setClientCertificate(QSslConfiguration configuration) {
  m_pWebSocket->setSslConfiguration(configuration);
}

void WebSocketWrapperRequest::prepareRequest() {
  if(!m_pIConnection->getCertificate().isNull()) setClientCertificate(m_pIConnection->getCertificate());
  if(!m_pIConnection->getCookieContainer().isEmpty()) setCookieContainer(m_pIConnection->getCookieContainer());
  if(!m_pIConnection->getCredentials().isNull()) setCredentials(m_pIConnection->getCredentials());
  setProxy(m_pIConnection->getProxy());
}

QNetworkRequest WebSocketWrapperRequest::getNetworkRequest() const {
  return m_Request;
}

}
