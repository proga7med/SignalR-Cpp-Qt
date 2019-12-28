#include "httpwebresponsewrapper.hpp"

namespace signalr::http {

HttpWebResponseWrapper::HttpWebResponseWrapper(QNetworkReply* reply) {
  m_pReply = reply;
}

QByteArray HttpWebResponseWrapper::getStream() const {
  return m_pReply->readAll();
}

}
