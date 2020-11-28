#pragma once
#ifndef PETRICHOR_TEST_SCENE_FRAMEWORK_HPP
#define PETRICHOR_TEST_SCENE_FRAMEWORK_HPP
#include <cstdint>
#include <chrono>
#include <memory>

namespace vpr
{
    class Device;
    class PhysicalDevice;
    class Instance;
    class Swapchain;
    class Semaphore;
}

struct RequiredVprObjects
{
    vpr::Device* device = nullptr;
    vpr::PhysicalDevice* physicalDevice = nullptr;
    vpr::Instance* instance = nullptr;
    vpr::Swapchain* swapchain = nullptr;
};

namespace petrichor
{

    class VulkanScene
    {
    protected:
        VulkanScene();
        virtual ~VulkanScene();
        VulkanScene(const VulkanScene&) = delete;
        VulkanScene& operator=(const VulkanScene&) = delete;
    public:

        virtual void Construct(RequiredVprObjects vpr_objects, void* user_data) = 0;
        virtual void Destroy() = 0;
        virtual void Render(void* user_data);
        size_t CurrentFrameIdx() const;

    protected:

        void createSemaphores();

        virtual void limitFrame();
        virtual void update() = 0;
        virtual void acquireImage();
        virtual void recordCommands() = 0;
        virtual void draw() = 0;
        virtual void present();
        virtual void endFrame();

        std::chrono::system_clock::time_point limiterA;
        std::chrono::system_clock::time_point limiterB;
        uint32_t currentBuffer;
        RequiredVprObjects vprObjects;
        std::unique_ptr<vpr::Semaphore> imageAcquireSemaphore;
        std::unique_ptr<vpr::Semaphore> renderCompleteSemaphore;

    };

}

#endif //!PETRICHOR_TEST_SCENE_FRAMEWORK_HPP
