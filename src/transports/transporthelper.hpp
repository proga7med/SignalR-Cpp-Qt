#ifndef TRANSPORTHELPER_HPP
#define TRANSPORTHELPER_HPP

#include <future>
#include "signalr/iconnection.hpp"

namespace signalr {
namespace transports {

class TransportHelper
{
public:
    TransportHelper() = default;
    //virtual std::future<NegotiationResponse> getNegotiationResponse(IHttpClient httpClient, IConnection connection, string connectionData)
};

}
}



#endif //TRANSPORTHELPER_HPP
