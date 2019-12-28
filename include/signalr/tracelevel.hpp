#ifndef TRACELEVEL_HPP
#define TRACELEVEL_HPP

namespace signalr {

enum class TraceLevel {
  None = 0,
  Messages = 1,
  Events = 2,
  StateChanges = 4,
  All = Messages | Events | StateChanges
};

}
#endif //TRACELEVEL_HPP
