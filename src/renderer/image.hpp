#pragma once
#define VULKAN_HPP_NO_EXCEPTIONS
#include "vulkan/vulkan.hpp"

vk::ImageView create_image_view(vk::Device logicalDevice, vk::Image image, vk::Format format);
