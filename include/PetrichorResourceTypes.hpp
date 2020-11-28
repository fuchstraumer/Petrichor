#pragma once
#ifndef PETRICHOR_RESOURCE_TYPES_HPP
#define PETRICHOR_RESOURCE_TYPES_HPP
#include <cstdint>
#include <limits>

namespace petrichor
{

    // This structure is used to describe data (singular/plural) that will be uploaded
    // to a created object. Destination queue family may be left blank, in which case
    // this will be identified by the code and used appropriately (e.g DMA queue to REN)
    struct GpuResourceData
    {
        const void* Data{ nullptr };
        size_t Size{ 0u };
        size_t Alignment{ 0u };
        uint32_t DestinationQueueFamily{ 0u };
    };

    // This structure is a more advanced version of GpuResourceData. It includes vital
    // extra fields required to fully specify what we need to know to handle image/texture
    // data, especially for uploading. 
    struct GpuImageResourceData
    {
        const void* Data{ nullptr };
        size_t Size{ 0u };
        uint32_t Width{ 0u };
        uint32_t Height{ 0u };
        // Used to specify which layer this is, in array textures. If not an array, leave 0
        uint32_t ArrayLayer{ 0u };
        // Specifies number of layers. If no layers, set this to 1.
        uint32_t ArrayLayerCount{ 0u };
        // Specifies which mip this structure has the data for. If no mips, leave 0.
        uint32_t MipLevel{ 0u };
        // Total number of mips. If no mips, just set to 1.
        uint32_t MipLevelCount{ 0u };
        // If you don't know what this is for, don't worry about it. May be used internally.
        uint32_t DestinationQueueFamily{ 0u };
    };

    enum class GpuResourceType : uint8_t
    {
        Invalid = 0,
        Buffer,
        Image,
        Sampler,
        CombinedImageSampler,
        BufferView,
        ImageView
    };

    // Describes memory "domain" or location it will be 
    // allocated and stored in
    enum class GpuResourceMemoryDomain : uint8_t
    {
        Invalid = 0,
        // Must reside in GPU memory proper (may be host visible on integrated GPUs)
        Device,
        // Resides on the CPU side of things. Not cached. Used for primarily for GPU upload
        Host,
        // Resides on host, but cached. Designed for readback
        HostCached,
        // This is so far only available on AMD, but is a pinned PCI mapping that allows
        // rapid (iirc) host-device memory sharing and access
        LinkedDeviceHost,
    };

    // scoped in a structure so we can make this enum work how we need, but without polluting namespace
    struct CreationFlagBits
    {
        enum : uint32_t
        {
            // Create this resource into it's own dedicated memory allocation on the device.
            ResourceCreateDedicatedMemory = 0x00000001,
            // Don't allocate new memory objects to fit this resource, it must find room among others
            ResourceCreateNeverAllocate = 0x00000002,
            // This memory will be mapped throughout it's entire lifetime, meaning we don't need to
            // map/unmap it to write to/from it. Will require a dedicated allocation, most likely
            ResourceCreatePersistentlyMapped = 0x00000004,
            // Passed in user data to creation function (unrelated to returned struct's UserData) 
            // will be interpreted as a \0 terminated C-string: this is then passed to debug info functions
            // if enabled, naming the resource in the API (and in graphics captures with tools like RenderDoc)
            ResourceCreateUserDataAsString = 0x00000020,
            // Use a memory allocation strategy that prioritizes overall memory footprint and consumption
            ResourceCreateMemoryStrategyMinMemory = 0x00010000,
            // This allocation strategy will prioritize time to allocate, but will result in waste
            ResourceCreateMemoryStrategyMinTime = 0x00020000,
            // Minimally fragmented allocation strategy - slower, but probably more efficient
            ResourceCreateMemoryStrategyMinFragmentation = 0x00040000
        };
    };

    // Alias to uint over the flag values to make bitwise operations (combinging them,
    // mostly) far easier.
    using GpuResourceCreationFlags = uint32_t;

    using GpuResourceHandle = uint64_t;
    constexpr static GpuResourceHandle INVALID_GPU_RESOURCE_HANDLE = std::numeric_limits<uint64_t>::max();

    /*
        Passed to our resource context to create a new resource, and contains all the requisite info
    */
    struct ResourceCreationMessage
    {
        // What kind of resource are we creating?
        GpuResourceType Type;
        GpuResourceMemoryDomain MemoryDomain;
        GpuResourceCreationFlags Flags;
        union
        {
            struct
            {
                uint32_t numData = 0u;
                const GpuResourceData* data = nullptr;
            } bufferData;
            struct
            {
                uint32_t numData = 0u;
                const GpuImageResourceData* data = nullptr;
            } imageData;
        } ResourceData;
        // Set to the VkBuffer/Image/SamplerCreateInfo you are using
        const void* Info = nullptr;
        // Set to the VkBufferViewCreateInfo/VkImageViewCreateInfo you are using for this object, if applicable
        const void* ViewInfo = nullptr;
        // Used to hold debug names if relevant flags set, otherwise it's whatever you like and is copied to the resource
        const void* UserData = nullptr;
        // If creating a view based on a pre-existing object, set this to the handle of the parent object
        uint64_t ParentHandle = 0u;
    };

    enum class ResourceModificationOpType : uint8_t
    {
        Invalid = 0,
        SetContents,
        ClearContents,
        CreateCopy,
    };

    struct ResourceModificationMessage
    {

    };

    /*
        Returned from resource creation message submission: can be queried to find status,
        and will return a valid handle to a resource once it is complete.
    */
    struct ResourceSystemReply
    {
        ResourceSystemReply() = default;
        ~ResourceSystemReply() = default;
        ResourceSystemReply(const ResourceSystemReply&) = delete;
        ResourceSystemReply& operator=(const ResourceSystemReply&) = delete;

    private:
        // Handle to the coroutine created for this object
        void* parent;
        void* coroutineHandle;
        friend struct ResourceCreationEvent;
        friend bool ResourceOperationComplete(const ResourceSystemReply&);
        friend GpuResourceHandle GetHandleFromOperation(const ResourceSystemReply&);
    };

    bool ResourceOperationComplete(const ResourceSystemReply& reply);
    GpuResourceHandle GetHandleFromOperation(const ResourceSystemReply& reply);

} // namespace petrichor

#endif //!PETRICHOR_RESOURCE_TYPES_HPP