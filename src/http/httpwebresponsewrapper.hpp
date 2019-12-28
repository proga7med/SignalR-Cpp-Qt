#ifndef HTTPWEBRESPONSEWRAPPER_HPP
#define HTTPWEBRESPONSEWRAPPER_HPP

#include <QNetworkReply>

#include "iresponse.hpp"

namespace signalr::http {

class HttpWebResponseWrapper : public IResponse {
  HttpWebResponseWrapper(QNetworkReply* reply);
  virtual QByteArray getStream() const override;

private:
  QNetworkReply* m_pReply;
};

}
#endif //HTTPWEBRESPONSEWRAPPER_HPP
