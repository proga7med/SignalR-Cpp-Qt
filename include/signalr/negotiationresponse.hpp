#ifndef NEGOTIATIONRESPONSE_HPP
#define NEGOTIATIONRESPONSE_HPP

#include <optional>

#include <QString>
#include <QObject>

namespace signalr {

class NegotiationResponse {
  Q_GADGET
public:
  Q_INVOKABLE NegotiationResponse(QObject* parent = nullptr) {}

  Q_PROPERTY(QString ConnectionId READ getConnectionId WRITE setConnectionId MEMBER m_ConnectionId);
  Q_PROPERTY(QString ConnectionToken READ getConnectionToken WRITE setConnectionToken MEMBER m_ConnectionToken);
  Q_PROPERTY(QString Url READ getUrl WRITE setUrl MEMBER m_Url);
  Q_PROPERTY(QString ProtocolVersion READ getProtocolVersion WRITE setProtocolVersion MEMBER m_ProtocolVersion);
  Q_PROPERTY(double DisconnectTimeout READ getDisconnectTimeout WRITE setDisconnectTimeout MEMBER m_DisconnectTimeout);
  Q_PROPERTY(bool TryWebSockets READ getTryWebSockets WRITE setTryWebSockets MEMBER m_TryWebSockets);
  Q_PROPERTY(std::optional<double> KeepAliveTimeout READ getKeepAliveTimeout WRITE setKeepAliveTimeout MEMBER m_KeepAliveTimeout);
  Q_PROPERTY(double TransportConnectTimeout READ getTransportConnectTimeout WRITE setTransportConnectTimeout MEMBER m_TransportConnectTimeout);
  Q_PROPERTY(QString RedirectUrl READ getRedirectUrl WRITE setRedirectUrl MEMBER m_RedirectUrl);
  Q_PROPERTY(QString AccessToken READ getAccessToken WRITE setAccessToken MEMBER m_AccessToken);
  Q_PROPERTY(QString Error READ getError WRITE setError MEMBER m_Error);

public:
  QString getConnectionId() const;
  void setConnectionId(QString connectionId);

  QString getConnectionToken() const;
  void setConnectionToken(QString connectionToken);

  QString getUrl() const;
  void setUrl(QString url);

  QString getProtocolVersion() const;
  void setProtocolVersion(QString protocolVersion);

  double getDisconnectTimeout() const;
  void setDisconnectTimeout(double disconnectTimeout);

  bool getTryWebSockets() const;
  void setTryWebSockets(bool tryWebSockets);

  std::optional<double> getKeepAliveTimeout() const;
  void setKeepAliveTimeout(std::optional<double> keepAliveTimeout);

  double getTransportConnectTimeout() const;
  void setTransportConnectTimeout(double transportConnectTimeout);

  QString getRedirectUrl() const;
  void setRedirectUrl(QString redirectUrl);

  QString getAccessToken() const;
  void setAccessToken(QString accessToken);

  QString getError() const;
  void setError(QString error);

private:
  QString m_ConnectionId;
  QString m_ConnectionToken;
  QString m_Url;
  QString m_ProtocolVersion;
  double m_DisconnectTimeout;
  bool m_TryWebSockets;
  std::optional<double> m_KeepAliveTimeout;
  double m_TransportConnectTimeout;
  QString m_RedirectUrl;
  QString m_AccessToken;
  QString m_Error;
};

}
Q_DECLARE_METATYPE(signalr::NegotiationResponse)

#endif //NEGOTIATIONRESPONSE_HPP
