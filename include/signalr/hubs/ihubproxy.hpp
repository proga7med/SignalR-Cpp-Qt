#ifndef IHUBPROXY_HPP
#define IHUBPROXY_HPP

#include <memory>

#include <QString>
#include <QException>
#include <QJsonSerializer>

#include "signalr/hubs/subscription.hpp"

namespace signalr {
namespace hubs {

class IHubProxy {
public:

  virtual std::shared_ptr<QJsonSerializer> getJsonSerializer() const = 0;
  virtual std::shared_ptr<Subscription> subscribe(const QString& eventName) = 0;
  virtual ~IHubProxy() = default;

};

}
}

#endif //IHUBPROXY_HPP
