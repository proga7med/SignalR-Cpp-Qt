#include "signalr/hubs/subscription.hpp"

namespace signalr {
namespace hubs {

void Subscription::onReceived(const QJsonArray &data) {
  emit received(data);
}

}
}
