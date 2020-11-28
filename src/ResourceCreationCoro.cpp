#include "ResourceCreationCoro.hpp"
#include <thread>

namespace petrichor
{

    ResourceSystemReply ResourceCreationEvent::Promise::get_return_object()
    {
        std::coroutine_handle<Promise> handle = std::coroutine_handle<Promise>::from_promise(*this);
        return ResourceCreationEvent::constructReply(handle);
    }


    bool ResourceCreationEvent::InitialSuspendAwaitable::await_ready() const noexcept
    {
        return false;
    }

    bool ResourceCreationEvent::InitialSuspendAwaitable::await_suspend(CoroutineHandle handle)
    {
        auto thisThreadID = std::this_thread::get_id();

        return false;
    }

}
