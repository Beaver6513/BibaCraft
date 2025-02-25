#include "image.hpp"

vk::ImageView create_image_view(vk::Device logicalDevice, vk::Image image, vk::Format format) {
  vk::ImageViewCreateInfo createInfo = {};
  createInfo.image = image;
  createInfo.viewType = vk::ImageViewType::e2D;
  createInfo.format = format;
  createInfo.components.r = vk::ComponentSwizzle::eIdentity;
  createInfo.components.g = vk::ComponentSwizzle::eIdentity;
  createInfo.components.b = vk::ComponentSwizzle::eIdentity;
  createInfo.components.a = vk::ComponentSwizzle::eIdentity;
  createInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
  createInfo.subresourceRange.baseMipLevel = 0;
  createInfo.subresourceRange.levelCount = 1;
  createInfo.subresourceRange.baseArrayLayer = 0;
  createInfo.subresourceRange.layerCount = 1;
  return logicalDevice.createImageView(createInfo).value;
}
