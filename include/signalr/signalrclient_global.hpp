#ifndef SIGNALRCLIENT_GLOBAL_HPP
#define SIGNALRCLIENT_GLOBAL_HPP

#include <QtCore/qglobal.h>

#ifndef QT_STATIC
    #if defined(SIGNALRCLIENT_LIBRARY)
        #  define SIGNALRCLIENT_EXPORT Q_DECL_EXPORT
    #else
        #  define SIGNALRCLIENT_EXPORT Q_DECL_IMPORT
    #endif
#else
    #define SIGNALRCLIENT_EXPORT
#endif


#endif //SIGNALRCLIENT_GLOBAL_HPP
