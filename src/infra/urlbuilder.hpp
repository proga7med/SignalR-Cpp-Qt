#ifndef URLBUILDER_HPP
#define URLBUILDER_HPP

#include <memory>

#include <QUrl>
#include <QString>

#include "signalr/iconnection.hpp"

namespace signalr {

class UrlBuilder {
public:
  static QString buildNegotiate(std::shared_ptr<IConnection> pConnection, const QString& connectionData);
  static QString buildStart(std::shared_ptr<IConnection> pConnection, const QString& transport, const QString& connectionData);
  static QString buildConnect(std::shared_ptr<IConnection> pConnection, const QString& transport, const QString& connectionData);
  static QString buildReconnect(std::shared_ptr<IConnection> pConnection, const QString& transport, const QString& connectionData);
  static QString buildPoll(std::shared_ptr<IConnection> pConnection, const QString& transport, const QString& connectionData);
  static QString buildSend(std::shared_ptr<IConnection> pConnection, const QString& transport, const QString& connectionData);
  static QString buildAbort(std::shared_ptr<IConnection> pConnection, const QString& transport, const QString& connectionData);
  static QString createBaseUrl(const QString& command, std::shared_ptr<IConnection> pConnection, const QString& transport, const QString& connectionData);
  static void appendCommonParameters(QString& urlStringBuilder, std::shared_ptr<IConnection> pConnection, const QString& transport, const QString& connectionData);
  static void appendReceiveParameters(QString& urlStringBuilder, std::shared_ptr<IConnection> pConnection);
  static QString trim(const QString& urlStringBuilder);
  static void appendClientProtocol(QString& urlStringBuilder, std::shared_ptr<IConnection> pConnection);
  static void appendTransport(QString& urlStringBuilder, const QString& transportName);
  static void appendConnectionToken(QString& urlStringBuilder, std::shared_ptr<IConnection> pConnection);
  static void appendMessageId(QString& urlStringBuilder, std::shared_ptr<IConnection> pConnection);
  static void appendGroupsToken(QString& urlStringBuilder, std::shared_ptr<IConnection> pConnection);
  static void appendConnectionData(QString& urlStringBuilder, const QString& connectionData);
  static void appendCustomQueryString(QString& urlStringBuilder, const QString& queryString);
  static void appendNoCacheUrlParam(QString& urlStringBuilder);
  static QUrl convertToWebSocketUri(const QString& uriString);
};

}

#endif //URLBUILDER_HPP
