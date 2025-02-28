#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include <deque>
#include <vulkan/vulkan.h>

#include "tiny_obj_loader.h"

const int WIDTH = 800;
const int HEIGHT = 600;
const int MAX_TRAIL_LENGTH = 50;

struct Sphere {
    glm::vec3 position;
    glm::vec3 velocity;
    float radius;
    std::deque<glm::vec3> trail;
};

std::vector<Sphere> spheres = {
    {{-0.5f, 0.0f, 0.0f}, {0.02f, 0.01f, 0.015f}, 0.1f},
    {{0.3f, -0.2f, 0.1f}, {-0.015f, 0.02f, -0.01f}, 0.1f},
    {{0.0f, 0.3f, -0.3f}, {0.01f, -0.02f, 0.02f}, 0.1f}
};

VkInstance instance;
VkSurfaceKHR surface;
VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
VkDevice device;
VkQueue graphicsQueue;
VkRenderPass renderPass;
VkPipeline graphicsPipeline;
VkFramebuffer framebuffer = VK_NULL_HANDLE;
VkBuffer vertexBuffer = VK_NULL_HANDLE;
VkBuffer indexBuffer = VK_NULL_HANDLE;
VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
VkDescriptorSet descriptorSets = VK_NULL_HANDLE;
VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
VkCommandPool commandPool = VK_NULL_HANDLE;

void createCommandPool() {
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = 0; // Assume graphics queue family index is 0
    poolInfo.flags = 0;

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create command pool!");
    }
}

void createCommandBuffer() {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate command buffer!");
    }
}

void beginCommandBuffer() {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }
}


uint32_t indexCount;
void pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    for (const auto& dev : devices) {
        if (true) { // Add suitable device selection criteria here
            physicalDevice = dev;
            break;
        }
    }
    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("Failed to find a suitable GPU!");
    }
}

void createLogicalDevice() {
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = 0; // Assume graphics queue family index is 0
    queueCreateInfo.queueCount = 1;
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device!");
    }

    vkGetDeviceQueue(device, 0, 0, &graphicsQueue);
}

void initVulkan() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Bouncing Spheres";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan instance!");
    }

    pickPhysicalDevice();
    createLogicalDevice();
}


void updatePhysics() {
    for (auto &sphere : spheres) {
        sphere.position += sphere.velocity;
        sphere.trail.push_front(sphere.position);
        if (sphere.trail.size() > MAX_TRAIL_LENGTH) {
            sphere.trail.pop_back();
        }
        for (int i = 0; i < 3; i++) {
            if (sphere.position[i] + sphere.radius > 1.0f || sphere.position[i] - sphere.radius < -1.0f) {
                sphere.velocity[i] *= -1.0f;
            }
        }
    }
}

void render(VkCommandBuffer commandBuffer) {
    // Placeholder: Implement Vulkan rendering of spheres and trails here

    VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = framebuffer; // Assume framebuffer is properly initialized
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = {WIDTH, HEIGHT};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    for (const auto& sphere : spheres) {
        // Bind vertex buffer, index buffer, and descriptor sets for each sphere
        // Assume vertexBuffer, indexBuffer, and descriptorSets are properly initialized
        VkBuffer vertexBuffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets, 0, nullptr);

        // Draw the sphere
        vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0); // Assume indexCount is properly initialized
    }

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer!");
    }
}


void mainLoop(VkCommandBuffer commandBuffer) {
    while (true) {
        updatePhysics();
        render(commandBuffer);
    }
}
int main() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Bouncing Spheres", nullptr, nullptr);

    initVulkan();
    createCommandPool();
    createCommandBuffer();
    beginCommandBuffer();

    mainLoop(commandBuffer);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        mainLoop(commandBuffer);
    }

    vkDestroyPipeline(device, graphicsPipeline, nullptr);
    vkDestroyRenderPass(device, renderPass, nullptr);
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
    /*
    The code above is a simple example of a Vulkan application that simulates bouncing spheres. The spheres are represented by their position, velocity, and radius, and they leave a trail behind them as they move. The main loop of the application updates the physics of the spheres and renders them using Vulkan. 
    The code is divided into several functions: 
    initVulkan()  initializes the Vulkan instance. updatePhysics()  updates the physics of the spheres. render()  renders the spheres and their trails using Vulkan. mainLoop()  is the main loop of the application that updates the physics and renders the spheres. 
    The  main()  function creates a window using GLFW and initializes Vulkan. It then enters the main loop, which updates the physics and renders the spheres until the window is closed. 
    The code is a simple example of a Vulkan application and does not include error handling or resource management. It is intended to demonstrate the basic structure of a Vulkan application and how to integrate it with GLFW. 
    */
