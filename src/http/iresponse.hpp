#ifndef IRESPONSE_HPP
#define IRESPONSE_HPP

#include <QDataStream>

namespace signalr {
namespace http {

class IResponse {
public:
  virtual QDataStream getStream() const = 0;
  virtual ~IResponse() = default;
};

}
}

#endif //IRESPONSE_HPP
