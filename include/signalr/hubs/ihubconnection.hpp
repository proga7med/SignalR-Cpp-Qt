#ifndef IHUBCONNECTION_HPP
#define IHUBCONNECTION_HPP

#include <memory>
#include <functional>

#include "hubresult.hpp"
#include "signalr/iconnection.hpp"
#include "signalr/connectionstate.hpp"

namespace signalr {
namespace hubs {

class IHubConnection : public virtual IConnection {
  Q_OBJECT
public:

  /**
   * @brief Creates the HubProxy by hub name. if existed, the existed instance would be returned.
   * @param hubName the hub name.
   * @return the created hubProxy.
   */
  virtual std::shared_ptr<IHubProxy> createHubProxy(const QString& hubName) = 0;
  virtual std::shared_ptr<IHubProxy> operator[] (const QString& hubName) = 0;
  virtual QString registerCallback(std::function<void(HubResult)> callback) = 0;
  virtual void removeCallback(const QString &callbackId) = 0;
  virtual ~IHubConnection() = default;
};

} //end hubs
} //end signalr
Q_DECLARE_INTERFACE(signalr::hubs::IHubConnection, "IHubConnection")

#endif //IHUBCONNECTION_HPP
