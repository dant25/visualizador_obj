#ifndef UTILITY_H
#define UTILITY_H

#define CAP3D_PRECISION 2

#define INTEGRATION_ID 1


#if (CAP3D_PRECISION == 1)
    typedef float Real;
#else
    typedef double Real;
#endif

#include <string>
#include <sstream>

template <class T>
inline std::string toString( const T& type)
{
    std::stringstream s;
    s << type;
    return s.str();
}

#endif
