#pragma once
#ifndef PETRICHOR_RESOURCE_CONTEXT_HPP
#define PETRICHOR_RESOURCE_CONTEXT_HPP
#include "PetrichorAPI.hpp"
#include "PetrichorResourceTypes.hpp"

struct VkBufferCreateInfo;
struct VkBufferViewCreateInfo;
struct VkImageCreateInfo;
struct VkImageViewCreateInfo;
struct VkSamplerCreateInfo;
struct VmaAllocationInfo;

namespace vpr
{
    class Device;
    class PhysicalDevice;
}

namespace petrichor
{

    struct ResourceContextImpl;

    class PETRICHOR_API ResourceContext
    {
        ResourceContext(const ResourceContext&) = delete;
        ResourceContext& operator=(const ResourceContext&) = delete;

        ResourceContext();
        ~ResourceContext();

    public:

        static ResourceContext& Get(vpr::Device* device, vpr::PhysicalDevice* physicalDevice);
        // Call at start of frame
        void Update();
        void Destroy();
        void Construct(vpr::Device* device, vpr::PhysicalDevice* physicalDevice);

        ResourceSystemReply CreateResource(ResourceCreationMessage message);
        void DestroyResource(GpuResourceHandle handle);

        /*
        void SetBufferData(
            GpuResource* dest_buffer,
            const size_t num_data,
            const GpuResourceData* data);

        void FillBuffer(
            GpuResource* dest_buffer,
            const uint32_t value,
            const size_t offset,
            const size_t fill_size);


        void SetImageData(
            GpuResource* image,
            const size_t num_data,
            const GpuImageResourceData* data);
        GpuResource* CreateResourceCopy(GpuResource* src);
        void CopyResource(GpuResource* src, GpuResource** dest);
        void CopyResourceContents(GpuResource* src, GpuResource* dest);
        void DestroyResource(GpuResource* resource);
        bool ResourceInTransferQueue(GpuResource* rsrc);
        void* MapResourceMemory(GpuResource* resource, size_t size, size_t offset);
        void UnmapResourceMemory(GpuResource* resource, size_t size, size_t offset);
        */

        void WriteMemoryStatsFile(const char* output_file);


    private:
        ResourceContextImpl* impl = nullptr;
    };

} // namespace petrichor

#endif //!PETRICHOR_RESOURCE_CONTEXT_HPP
