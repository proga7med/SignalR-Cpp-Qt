#include "signalr/keepalivedata.hpp"

namespace signalr {

KeepAliveData::KeepAliveData(TimeDelta timeout) {
  m_Timeout = std::move(timeout);
}

KeepAliveData::KeepAliveData(TimeDelta timeout, TimeDelta timeoutWarning, TimeDelta checkInterval) {
  m_Timeout = std::move(timeout);
  m_TimeoutWarning = std::move(timeoutWarning);
  m_CheckInterval = std::move(checkInterval);
}

KeepAliveData::KeepAliveData(const KeepAliveData& keepAliveData) {
  m_Timeout = keepAliveData.m_Timeout;
  m_CheckInterval = keepAliveData.m_CheckInterval;
  m_TimeoutWarning = keepAliveData.m_TimeoutWarning;
}

KeepAliveData& KeepAliveData::operator= (const KeepAliveData& keepAliveData) {
  m_Timeout = keepAliveData.m_Timeout;
  m_CheckInterval = keepAliveData.m_CheckInterval;
  m_TimeoutWarning = keepAliveData.m_TimeoutWarning;
  return *this;
}

TimeDelta KeepAliveData::getTimeout() const {
  return m_Timeout;
}

TimeDelta KeepAliveData::getTimeoutWarning() const {
  return m_TimeoutWarning;
}

TimeDelta KeepAliveData::getCheckInterval() const {
  return m_CheckInterval;
}

}
