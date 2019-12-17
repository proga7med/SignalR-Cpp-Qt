#ifndef IHUBCONNECTION_HPP
#define IHUBCONNECTION_HPP

#include "hubresult.hpp"
#include "infra/Action.hpp"
#include "signalr/iconnection.hpp"
#include "signalr/connectionstate.hpp"

namespace signalr {
namespace hubs {

class IHubConnection : public IConnection {
public:

    /**
     * @brief Creates the HubProxy by hub name. if existed, the existed instance would be returned.
     * @param hubName the hub name.
     * @return the created hubProxy.
     */
    virtual std::shared_ptr<IHubProxy> createHubProxy(const QString& hubName) = 0;

    virtual QString registerCallback(Action<HubResult> callback) = 0;

    virtual void removeCallback(const QString& callbackId) = 0;

    virtual ~IHubConnection() = default;
};

} //end hubs
} //end signalr

#endif //IHUBCONNECTION_HPP
