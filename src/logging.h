#pragma once
#include "config.h"

namespace vkInit {

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
  VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
  VkDebugUtilsMessageTypeFlagsEXT messageType,
  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
  void* pUserData
);

vk::DebugUtilsMessengerEXT debugMessenger_setup(vk::Instance& instance, vk::DispatchLoaderDynamic& dldi);

}
