#pragma once
#ifndef PETRICHOR_RESOURCE_CREATION_COROUTINE_HPP
#define PETRICHOR_RESOURCE_CREATION_COROUTINE_HPP
#include "PetrichorResourceTypes.hpp"
#include <coroutine>

namespace petrichor
{
    
    struct ResourceCreationEvent
    {
        // Debug enum used to track state as we proceed through the process
        enum class State
        {

        };

        ResourceCreationEvent(const ResourceCreationEvent&) = delete;
        ResourceCreationEvent& operator=(const ResourceCreationEvent&) = delete;

        struct Promise;
        using promise_type = Promise;
        using CoroutineHandle = std::coroutine_handle<promise_type>;

        ResourceCreationEvent(const void* _devicePtr, const void* _physicalDevicePtr, ResourceCreationMessage msg);

        struct Awaiter;
        Awaiter operator co_await() noexcept;

        static ResourceSystemReply constructReply(CoroutineHandle handle)
        {
            return ResourceSystemReply(handle.address());
        }

        
        struct Awaiter
        {
            // If it returns true, this indicates the value can be immediately returned
            // This means the resource or whatever we're doing here is performed synchronously.
            bool await_ready() const;
            // If await ready is false, then we suspend the coroutine and execute this code
            bool await_suspend();
        };

        struct InitialSuspendAwaitable;

        struct Promise
        {
            // Exception in coroutine. Handle it as best as we can.
            void unhandled_exception() noexcept;
            
            ResourceSystemReply get_return_object();
            InitialSuspendAwaitable initial_suspend() noexcept;
            

        };

        struct InitialSuspendAwaitable
        {
            // We always suspend, because we use that to schedule the resource operation
            bool await_ready() const noexcept;
            bool await_suspend(CoroutineHandle handle);


        };

        void* devicePtr = nullptr;
        void* physicalDevicePtr = nullptr;
        GpuResourceHandle resourceHandle = INVALID_GPU_RESOURCE_HANDLE;
    };

}

#endif //!PETRICHOR_RESOURCE_CREATION_COROUTINE_HPP
