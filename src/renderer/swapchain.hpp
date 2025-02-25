#pragma once
#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>
#include <deque>
#include <functional>

struct SurfaceDetails {
  // # of images and supported sizes
  vk::SurfaceCapabilitiesKHR capabilities;
  // supported pixel formats
  std::vector<vk::SurfaceFormatKHR> formats;
  // available presentation modes
  std::vector<vk::PresentModeKHR> presentModes;
};

class Swapchain {
public:
  void build(
    vk::Device logicalDevice, vk::PhysicalDevice physicalDevice,
    vk::SurfaceKHR surface, uint32_t width, uint32_t height,
    std::deque<std::function<void(vk::Device)>>& deviceDeletionQueue
  );
  // number of images
  uint32_t imageCount;
  // swapchain resource
  vk::SwapchainKHR chain;
  // image format
  vk::SurfaceFormatKHR format;
  // image size
  vk::Extent2D extent;

private:
  SurfaceDetails query_surface_support(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
  vk::Extent2D choose_extent(uint32_t width, uint32_t height, vk::SurfaceCapabilitiesKHR capabilities);
  vk::PresentModeKHR choose_present_mode(std::vector<vk::PresentModeKHR> presentModes);
  vk::SurfaceFormatKHR choose_surface_format(std::vector<vk::SurfaceFormatKHR> formats);
};
