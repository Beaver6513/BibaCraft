#include "swapchain.hpp"
#include "../logging/logger.hpp"

void Swapchain::build(
    vk::Device logicalDevice, vk::PhysicalDevice physicalDevice,
    vk::SurfaceKHR surface, uint32_t width, uint32_t height,
    std::deque<std::function<void(vk::Device)>>& deviceDeletionQueue
  ) {
  Logger* logger = Logger::get_logger();

  SurfaceDetails support = query_surface_support(physicalDevice, surface);

  format = choose_surface_format(support.formats);
  vk::PresentModeKHR presentMode = choose_present_mode(support.presentModes);
  extent = choose_extent(width, height, support.capabilities);
  if (support.capabilities.maxImageCount == 0) imageCount = support.capabilities.minImageCount;
  else imageCount = std::min(support.capabilities.maxImageCount, support.capabilities.minImageCount + 1);

  vk::SwapchainCreateInfoKHR swapchainCreateInfo = vk::SwapchainCreateInfoKHR(vk::SwapchainCreateFlagsKHR(), 
                              surface, imageCount, format.format, format.colorSpace, 
                             extent, 1, vk::ImageUsageFlagBits::eColorAttachment, vk::SharingMode::eExclusive);
  swapchainCreateInfo.preTransform = support.capabilities.currentTransform;
  swapchainCreateInfo.presentMode = presentMode;
  swapchainCreateInfo.clipped = VK_TRUE;
  swapchainCreateInfo.oldSwapchain = vk::SwapchainKHR(nullptr);

  auto result = logicalDevice.createSwapchainKHR(swapchainCreateInfo);
  if (result.result == vk::Result::eSuccess) {
    chain = result.value;
    deviceDeletionQueue.push_back([this, logger](vk::Device device) {
      logger->print("Destroyed swapchain!");
      device.destroySwapchainKHR(chain);
    });
  } else {
    logger->print("Failed to create swapchain!");
  }
  
}

SurfaceDetails Swapchain::query_surface_support(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface) {
  Logger* logger = Logger::get_logger();

  SurfaceDetails support;

  support.capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface).value;
  logger->log(support.capabilities);
  support.formats = physicalDevice.getSurfaceFormatsKHR(surface).value;
  logger->log(support.formats);
  support.presentModes = physicalDevice.getSurfacePresentModesKHR(surface).value;
  logger->log(support.presentModes);

  return support;
}

vk::Extent2D Swapchain::choose_extent(uint32_t width, uint32_t height, vk::SurfaceCapabilitiesKHR capabilities) {
  // if surface creation didnt fail:
  if(capabilities.currentExtent.width != UINT32_MAX) return capabilities.currentExtent;
  // otherwise create new extent
  vk::Extent2D extent;
  extent.width = std::min(capabilities.maxImageExtent.width, std::max(capabilities.minImageExtent.width, width));
  extent.height = std::min(capabilities.maxImageExtent.height, std::max(capabilities.minImageExtent.height, height));
  return extent;
}

vk::PresentModeKHR Swapchain::choose_present_mode(std::vector<vk::PresentModeKHR> presentModes) {
  for (vk::PresentModeKHR mode : presentModes) {
    if (mode == vk::PresentModeKHR::eMailbox) return mode;
  }
  return vk::PresentModeKHR::eFifo;
}

vk::SurfaceFormatKHR Swapchain::choose_surface_format(std::vector<vk::SurfaceFormatKHR> formats) {
  for (vk::SurfaceFormatKHR format : formats) {
    if (format.format == vk::Format::eB8G8R8A8Unorm &&
    format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) return format;
  }
  return formats[0];
}
