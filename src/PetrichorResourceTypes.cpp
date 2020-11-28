#include "PetrichorResourceTypes.hpp"
#include "ResourceCreationCoro.hpp"

namespace petrichor
{

    bool ResourceOperationComplete(const ResourceSystemReply& reply)
    {
        using coroHandle = std::coroutine_handle<ResourceCreationEvent::promise_type>;
        coroHandle handle = coroHandle::from_address(reply.coroutineHandle);
        // bool conversion first lets us know this is actually referring to a valid handle, so we don't crash
        return handle ? handle.done() : false;
    }

    GpuResourceHandle GetHandleFromOperation(const ResourceSystemReply& reply)
    {
        using coroHandle = std::coroutine_handle<ResourceCreationEvent::promise_type>;
        coroHandle handle = coroHandle::from_address(reply.coroutineHandle);
        if (handle && handle.done())
        {
            auto& handlePromise = handle.promise();

        }
        else
        {
            return INVALID_GPU_RESOURCE_HANDLE;
        }
    }

}
