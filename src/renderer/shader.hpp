#pragma once
#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>
#include <deque>
#include <functional>

// creates the shader objects
std::vector<vk::ShaderEXT> make_shader_objects(vk::Device logicalDevice, 
    const char* vertexFilename, const char* fragmentFilename, vk::DispatchLoaderDynamic& dl,
    std::deque<std::function<void(vk::Device)>>& deviceDeletionQueue);
