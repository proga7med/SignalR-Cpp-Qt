#ifndef HUBPROXY_HPP
#define HUBPROXY_HPP

#include "signalr/hubs/ihubproxy.hpp"

namespace signalr {
namespace hubs {

class HubProxy : public IHubProxy {
public:
    HubProxy();
    virtual ~HubProxy() = default;
};


}
}


#endif //HUBPROXY_HPP
