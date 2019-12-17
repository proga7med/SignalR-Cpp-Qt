#ifndef ACTION_HPP
#define ACTION_HPP

template<class ...Ts>
using Action = void(*)(Ts...);

#endif //ACTION_HPP
