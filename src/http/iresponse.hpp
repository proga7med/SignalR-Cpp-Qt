#ifndef IRESPONSE_HPP
#define IRESPONSE_HPP

#include <QDataStream>

#include <QtPromise>

namespace signalr {
namespace http {

class IResponse {
public:
  virtual QByteArray getStream() const = 0;

  static QtPromise::QPromise<QString> readAsString(std::shared_ptr<IResponse> pResponse) {
    return pResponse->readAsString(pResponse, [&](const QByteArray& data) { return true; });
  }

  static QtPromise::QPromise<QString> readAsString(std::shared_ptr<IResponse> pResponse, std::function<bool(const QByteArray&)> onChunk) {
    if(pResponse == nullptr) throw QException();
    auto buffer = pResponse->getStream();
    QString result;
    return QtPromise::QPromise<QString>([&](const QtPromise::QPromiseResolve<QString>& resolver) {
      onChunk(buffer) == true
              ? result.append(QString::fromUtf8(buffer).data(), buffer.length())
              : result = QString::fromUtf8(buffer);

      resolver(result);
    });
  }

  virtual ~IResponse() = default;
};

}
}

#endif //IRESPONSE_HPP
