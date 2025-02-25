#pragma once
#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>
#include "image.hpp"
#include <deque>
#include <functional>

//
// Holds the information and states
// used in one rendering/presentation
// operation
//
class Frame {
  public:
  Frame(vk::Image image, vk::Device logicalDevice, vk::Format swapchainFormat,
        std::deque<std::function<void(vk::Device)>>& deletionQueue);
  // image to render to
  vk::Image image;
  // view of the swapchain image
  vk::ImageView imageView;
};
