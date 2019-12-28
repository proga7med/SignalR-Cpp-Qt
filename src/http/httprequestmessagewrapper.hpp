#ifndef HTTPREQUESTMESSAGEWRAPPER_HPP
#define HTTPREQUESTMESSAGEWRAPPER_HPP

#include <QObject>
#include <QNetworkRequest>

#include "http/irequest.hpp"

namespace signalr {
namespace http {

class HttpRequestMessageWrapper : public QObject, public IRequest {
	Q_OBJECT
public:
  HttpRequestMessageWrapper(QNetworkRequest& networkRequest);

  virtual QString getUserAgent() const override;
  virtual void setUserAgent(QString userAgent) override;

  virtual QString getAccept() const override;
  virtual void setAccept(QString accept) override;

  virtual void abort() override;
  virtual void setRequestHeaders(QList<QPair<QString, QString>> &headers) override;

  virtual ~HttpRequestMessageWrapper() = default;

signals:
  void cancel();

private:
  QNetworkRequest m_NetworkRequest;
  QString m_UserAgent;
  QString m_Accept;
};

}
}

#endif // ! HTTPREQUESTMESSAGEWRAPPER_HPP
