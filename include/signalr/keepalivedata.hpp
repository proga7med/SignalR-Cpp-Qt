#ifndef KEEPALIVEDATA_HPP
#define KEEPALIVEDATA_HPP

#include <time_delta.h>

namespace signalr {

class Connection;
class IConnection;
class KeepAliveData {
public:
  KeepAliveData(TimeDelta timeout);
  KeepAliveData(TimeDelta timeout,  TimeDelta timeoutWarning, TimeDelta checkInterval);
  KeepAliveData(const KeepAliveData& keepAliveData);
  KeepAliveData(KeepAliveData&&) = default;
  KeepAliveData& operator= (const KeepAliveData&);
  KeepAliveData& operator= (KeepAliveData&&) = default;

  TimeDelta getTimeout() const;
  TimeDelta getTimeoutWarning() const;
  TimeDelta getCheckInterval() const;

private:
  KeepAliveData() = default;
  friend class Connection;
  friend class IConnection;

private:
  const double m_KeepAliveWarnAt = 2.0 / 3.0;
  TimeDelta m_Timeout;
  TimeDelta m_TimeoutWarning;
  TimeDelta m_CheckInterval;
};

}

#endif //KEEPALIVEDATA_HPP
