#pragma once
#ifndef PETRICHOR_RESOURCE_CONTEXT_IMPL_HPP
#define PETRICHOR_RESOURCE_CONTEXT_IMPL_HPP
#include "ResourceContext.hpp"
#include "ResourceCreationCoro.hpp"
#include <vector>
#include <unordered_map>
#include <vulkan/vulkan_core.h>
#include <queue>
#include <coroutine>
#include <thread>
#include "vk_mem_alloc.h"
#include "VkDebugUtils.hpp"
#include "mwsrQueue.hpp"

namespace petrichor
{
    
    struct ResourceContextImpl
    {

        void construct(vpr::Device* _device, vpr::PhysicalDevice* _physical_device, bool validation_enabled);
        void destroy();

        ResourceSystemReply createResource(ResourceCreationMessage message);
        void ProcessMessages();

        /*
        GpuResource* createBuffer(
            const VkBufferCreateInfo* info,
            const VkBufferViewCreateInfo* view_info,
            const size_t num_data,
            const GpuResourceData* initial_data,
            const GpuResource::MemoryDomain _memory_domain,
            const GpuResource::CreationFlags _flags,
            void* user_data = nullptr);

        void setBufferData(GpuResource* dest_buffer, const size_t num_data, const GpuResourceData* data);

        GpuResource* createImage(
            const VkImageCreateInfo* info,
            const VkImageViewCreateInfo* view_info,
            const size_t num_data,
            const GpuImageResourceData* initial_data,
            const GpuResource::MemoryDomain _memory_domain,
            const GpuResource::CreationFlags _flags,
            void* user_data = nullptr);

        GpuResource* createImageView(const GpuResource* base_rsrc, const VkImageViewCreateInfo* view_info, void* user_data = nullptr);
        GpuResource* createSampler(const VkSamplerCreateInfo* info, const GpuResource::CreationFlags _flags, void* user_data = nullptr);

        void copyResourceContents(GpuResource* src, GpuResource* dst);
        void setBufferInitialDataHostOnly(GpuResource * resource, const size_t num_data, const GpuResourceData * initial_data, GpuResource::MemoryDomain _memory_domain);
        void setBufferInitialDataUploadBuffer(GpuResource* resource, const size_t num_data, const GpuResourceData* initial_data);
        void setImageInitialData(GpuResource* resource, const size_t num_data, const GpuImageResourceData* initial_data);

        VkFormatFeatureFlags featureFlagsFromUsage(const VkImageUsageFlags flags) const noexcept;
        void writeStatsJsonFile(const char* output_file);
        bool resourceInTransferQueue(GpuResource* rsrc);
        */
    private:
        vpr::VkDebugUtilsFunctions vkDebugFns;
        vpr::Device* logicalDevice = nullptr;
        vpr::PhysicalDevice physicalDevice = nullptr;
        VmaAllocator vmaAllocatorHandle = VK_NULL_HANDLE;
        friend struct ResourceCreationEvent;

        void enqueueEvent(ResourceCreationEvent&& event);
        std::thread::id workQueueThreadID;
        mwsrQueue<ResourceCreationEvent> eventQueue;
    };

}

#endif