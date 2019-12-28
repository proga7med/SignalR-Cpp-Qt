#ifndef IREQUEST_HPP
#define IREQUEST_HPP

#include <QString>
#include <QPair>

namespace signalr {
namespace http {

class IRequest {
public:
  virtual void abort() = 0;
  virtual void setRequestHeaders(QList<QPair<QString, QString>>& headers) = 0;
  virtual ~IRequest() = default;

  virtual QString getUserAgent() const = 0;
  virtual void setUserAgent(QString userAgent) = 0;

  virtual QString getAccept() const = 0;
  virtual void setAccept(QString accept) = 0;
};

}
}

#endif //IREQUEST_HPP
