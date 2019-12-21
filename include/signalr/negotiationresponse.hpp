#ifndef NEGOTIATIONRESPONSE_HPP
#define NEGOTIATIONRESPONSE_HPP

#include <optional>
#include <QString>

namespace signalr {

class NegotiationResponse {
public:
    NegotiationResponse() = default;

    QString getConnectionId() const;
    void setConnectionId(QString connectionId);

    QString getConnectionToken() const;
    void setConnectionToken(QString connectionToken);

    QString getUrl() const;
    void setUrl(QString url);

    double getDisconnectTimeout() const;
    void setDisconnectTimeout(double disconnectTimeout);

    bool getTryWebSocket() const;
    void setTryWebSocket(bool tryWebSocket);

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


#endif //NEGOTIATIONRESPONSE_HPP
