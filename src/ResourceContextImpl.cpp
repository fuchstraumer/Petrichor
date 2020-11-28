#include "ResourceContextImpl.hpp"
#include "Instance.hpp"
#include "LogicalDevice.hpp"
#include "PhysicalDevice.hpp"
#include "vkAssert.hpp"

namespace petrichor
{
    void ResourceContextImpl::construct(vpr::Device* _device, vpr::PhysicalDevice* _physical_device, bool validation_enabled)
    {
        workQueueThreadID = std::this_thread::get_id();
        logicalDevice = _device;
        physicalDevice = _physical_device;

        if (validation_enabled)
        {
            vkDebugFns = logicalDevice->DebugUtilsHandler();
        }

        const VkApplicationInfo& applicationInfo = logicalDevice->ParentInstance()->ApplicationInfo();

        VmaAllocatorCreateInfo allocatorCreateInfo;
        memset(&allocatorCreateInfo, 0, sizeof(VmaAllocatorCreateInfo));
        allocatorCreateInfo.flags = applicationInfo.apiVersion >= VK_API_VERSION_1_1 ? VMA_ALLOCATOR_CREATE_KHR_DEDICATED_ALLOCATION_BIT : 0u;
        allocatorCreateInfo.device = logicalDevice->vkHandle();
        allocatorCreateInfo.physicalDevice = physicalDevice->vkHandle();
        allocatorCreateInfo.instance = logicalDevice->ParentInstance()->vkHandle();
        allocatorCreateInfo.vulkanApiVersion = applicationInfo.apiVersion;

        VkResult result = vmaCreateAllocator(&allocatorCreateInfo, &vmaAllocatorHandle);
        VkAssert(result);

    }

    ResourceSystemReply ResourceContextImpl::createResource(ResourceCreationMessage message)
    {

        return ResourceSystemReply();
    }

}
