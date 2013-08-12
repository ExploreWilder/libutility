#ifndef shared_utility_runnable_hpp_included_
#define shared_utility_runnable_hpp_included_

#include <boost/noncopyable.hpp>

namespace utility {

class Runable : boost::noncopyable {
public:
    virtual ~Runable() {}
    virtual bool isRunning() = 0;
    virtual void stop() = 0;

    inline operator bool() { return isRunning(); }
};

} // namespace utility

#endif // shared_utility_runnable_hpp_included_
