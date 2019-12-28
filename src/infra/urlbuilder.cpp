#include "urlbuilder.hpp"
#include <QUuid>

namespace signalr {

QString UrlBuilder::buildNegotiate(std::shared_ptr<IConnection> pConnection, const QString& connectionData) {
  if (pConnection == nullptr) throw QException();
  auto urlStringBuilder = createBaseUrl("negotiate", pConnection, QString(), connectionData);
  appendNoCacheUrlParam(urlStringBuilder);
  return trim(urlStringBuilder);
}

QString UrlBuilder::buildStart(std::shared_ptr<IConnection> pConnection, const QString& transport, const QString& connectionData) {
  if (pConnection == nullptr) throw QException();
  if(transport.isNull() || transport.isEmpty()) throw QException();
  return trim(createBaseUrl("start", pConnection, transport, connectionData));
}

QString UrlBuilder::buildConnect(std::shared_ptr<IConnection> pConnection, const QString& transport, const QString& connectionData) {
  if (pConnection == nullptr) throw QException();
  if(transport.isNull() || transport.isEmpty()) throw QException();
  auto urlStringBuilder = createBaseUrl("connect", pConnection, transport, connectionData);
  appendReceiveParameters(urlStringBuilder, pConnection);
  return trim(urlStringBuilder);
}

QString UrlBuilder::buildReconnect(std::shared_ptr<IConnection> pConnection, const QString& transport, const QString& connectionData) {
  if (pConnection == nullptr) throw QException();
  if(transport.isNull() || transport.isEmpty()) throw QException();
  auto urlStringBuilder = createBaseUrl("reconnect", pConnection, transport, connectionData);
  appendReceiveParameters(urlStringBuilder, pConnection);
  return trim(urlStringBuilder);
}

QString UrlBuilder::buildPoll(std::shared_ptr<IConnection> pConnection, const QString& transport, const QString& connectionData) {
  if (pConnection == nullptr) throw QException();
  if(transport.isNull() || transport.isEmpty()) throw QException();
  auto urlStringBuilder = createBaseUrl("poll", pConnection, transport, connectionData);
  appendReceiveParameters(urlStringBuilder, pConnection);
  return trim(urlStringBuilder);
}

QString UrlBuilder::buildSend(std::shared_ptr<IConnection> pConnection, const QString& transport, const QString& connectionData) {
  if (pConnection == nullptr) throw QException();
  if(transport.isNull() || transport.isEmpty()) throw QException();
  return trim(createBaseUrl("send", pConnection, transport, connectionData));
}

QString UrlBuilder::buildAbort(std::shared_ptr<IConnection> pConnection, const QString& transport, const QString& connectionData) {
  if (pConnection == nullptr) throw QException();
  if(transport.isNull() || transport.isEmpty()) throw QException();
  return trim(createBaseUrl("abort", pConnection, transport, connectionData));
}

QString UrlBuilder::createBaseUrl(const QString& command, std::shared_ptr<IConnection> pConnection, const QString& transport, const QString& connectionData) {
  auto urlStringBuilder = QString()
          .append(pConnection->getUrl())
          .append(command)
          .append("?");

  appendCommonParameters(urlStringBuilder, pConnection, transport, connectionData);
  return urlStringBuilder;
}

void UrlBuilder::appendCommonParameters(QString& urlStringBuilder, std::shared_ptr<IConnection> pConnection, const QString& transport, const QString& connectionData) {
  appendClientProtocol(urlStringBuilder, pConnection);
  appendTransport(urlStringBuilder, transport);
  appendConnectionData(urlStringBuilder, connectionData);
  appendConnectionToken(urlStringBuilder, pConnection);
  appendCustomQueryString(urlStringBuilder, pConnection->getQueryString());
}

void UrlBuilder::appendReceiveParameters(QString& urlStringBuilder, std::shared_ptr<IConnection> pConnection) {
  appendMessageId(urlStringBuilder, pConnection);
  appendGroupsToken(urlStringBuilder, pConnection);
  appendNoCacheUrlParam(urlStringBuilder);
}

QString UrlBuilder::trim(const QString& urlStringBuilder) {
  auto length = urlStringBuilder.length() - 1;
  return QString(urlStringBuilder.data(), length);
}

void UrlBuilder::appendClientProtocol(QString& urlStringBuilder, std::shared_ptr<IConnection> pConnection) {
  urlStringBuilder
          .append("clientProtocol=")
          .append(pConnection->getProtocol().toString())
          .append("&");
}

void UrlBuilder::appendTransport(QString& urlStringBuilder, const QString& transportName) {
  if(transportName.isNull()) return;
  urlStringBuilder
          .append("transport=")
          .append(transportName)
          .append("&");
}

void UrlBuilder::appendConnectionToken(QString& urlStringBuilder, std::shared_ptr<IConnection> pConnection) {
  QString connectionToken = pConnection->getConnectionToken();
  if(connectionToken.isNull()) return;
  urlStringBuilder
          .append("connectionToken=")
          .append(QUrl::toPercentEncoding(connectionToken))
          .append("&");
}

void UrlBuilder::appendMessageId(QString& urlStringBuilder, std::shared_ptr<IConnection> pConnection) {
  QString messageId = pConnection->getMesssageId();
  if(messageId.isNull() || messageId.isEmpty()) return;
  urlStringBuilder
          .append("messageId=")
          .append(QUrl::toPercentEncoding(messageId))
          .append("&");
}

void UrlBuilder::appendGroupsToken(QString& urlStringBuilder, std::shared_ptr<IConnection> pConnection) {
  QString groupsToken = pConnection->getGroupsToken();
  if(groupsToken.isNull()) return;
  urlStringBuilder
          .append("groupsToken=")
          .append(QUrl::toPercentEncoding(groupsToken))
          .append("&");
}

void UrlBuilder::appendConnectionData(QString& urlStringBuilder, const QString& connectionData) {
  if(connectionData.isNull() || connectionData.isEmpty()) return;
  urlStringBuilder
          .append("connectionData=")
          .append(connectionData)
          .append("&");
}

void UrlBuilder::appendCustomQueryString(QString& urlStringBuilder, const QString& queryString) {
  if(queryString.isNull() || queryString.isEmpty()) return;
  auto firstChar = queryString[0];
  firstChar == "?" || firstChar == "&"
          ? urlStringBuilder.append(queryString.mid(1))
          : urlStringBuilder.append(queryString);
  urlStringBuilder.append("&");
}

void UrlBuilder::appendNoCacheUrlParam(QString& urlStringBuilder) {
  urlStringBuilder
      .append("noCache=")
      .append(QUuid::createUuid().toString().mid(1,36))
      .append("&");
}

QUrl UrlBuilder::convertToWebSocketUri(const QString& uriString) {
  QUrl uriBuilder = QUrl(uriString);
  if(uriBuilder.scheme() != "http" && uriBuilder.scheme() != "https") throw QException();
  uriBuilder.setScheme(uriBuilder.scheme() == "https" ? "wss" : "ws");
  return uriBuilder;
}

}
