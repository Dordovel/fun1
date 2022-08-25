#ifndef INVOKE
#define INVOKE

#include <type_traits>
#include <utility>

namespace worker
{
    template<typename Function, typename Object, typename ...Args>
    decltype(auto) invoke(Function Object::* function, Object&& pointer, Args&& ...args)
    {
        return ((*std::forward<Object>(pointer)).*function)(std::forward<Args>(args)...);
    }

    template<typename Function, typename ...Args>
    decltype(auto) invoke(Function&& function, Args&& ...args)
    {
        return std::forward<Function>(function)(std::forward<Args>(args)...);    
    }

    template<typename Function>
    decltype(auto) invoke(Function&& function)
    {
        return std::forward<Function>(function)();    
    }
};

#endif //INVOKE
