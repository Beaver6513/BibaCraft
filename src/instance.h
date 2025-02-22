#pragma once
#include "config.h"

namespace vkInit {
bool supported(std::vector<const char*>& extensions, std::vector<const char*> layers, bool debug) {
  std::vector<vk::ExtensionProperties> supportedExtensions   = vk::enumerateInstanceExtensionProperties();

  if (debug) {
    std::cout << "Device can support these extensions: \n";
    for (vk::ExtensionProperties supportedExtensions : supportedExtensions) {
      std::cout << "\t" << supportedExtensions.extensionName << std::endl;
    }
  }

  for (const char* extension : extensions) {
    // std::find returns an iterator to the element if found, otherwise vec2.end()
    if (std::find(extensions.begin(), extensions.end(), extension) != extensions.end()) {
        std::cout << "Extension : " << extension << " is compatible!" << std::endl;
    } else {
        std::cout << "Extension : " << extension << " is not compatible!" << std::endl;
    }
  }
  
  std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();

  if (debug) {
    std::cout << "Device can support these layers: \n";
    for (vk::LayerProperties supportedLayers : supportedLayers) {
      std::cout << "\t" << supportedLayers.layerName << std::endl;
    }
  }

  for (const char* layer : layers) {
    // std::find returns an iterator to the element if found, otherwise vec2.end()
    if (std::find(layers.begin(), layers.end(), layer) != layers.end()) {
        std::cout << "Layer : " << layer << " is compatible!" << std::endl;
    } else {
        std::cout << "Layer : " << layer << " is not compatible!" << std::endl;
    }
  }
  return true;
}

// A vulkan instance stores application info
vk::Instance make_vk_Instance(bool debug, const char* applicationName) {
  if (debug) {
    std::cout << "Making Vulkan Instance\n";
  }

  uint32_t instance_version{ 0 };
  vk::enumerateInstanceVersion(&instance_version);
  if (debug) {
    std::cout << "System can support Vulkan variant: " << 
    VK_API_VERSION_VARIANT(instance_version) << ", major: " <<
    VK_API_VERSION_MAJOR(instance_version) << ", minor: " <<
    VK_API_VERSION_MINOR(instance_version) << ", patch: " <<
    VK_API_VERSION_PATCH(instance_version) << std::endl;
  }
  // Here we zero the patch number to allow compatibility with every patch
  // We also turn back on earlier version for compatibility
  instance_version &= ~(0xFFFU);
  instance_version = VK_MAKE_API_VERSION(0, 1, 0, 0);

  vk::ApplicationInfo appInfo = vk::ApplicationInfo(
    applicationName,
    instance_version,
    "VKRenderer Engine",
    instance_version,
    instance_version
  );


  /*
  * Everything with Vulkan is "opt-in", so we need to query which extensions glfw needs
  * in order to interface with vulkan.
  */
  uint32_t glfwExtensionCount = 0;
  const char** glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
  if (debug) extensions.push_back("VK_EXT_debug_utils");

  if (debug) {
    std::cout << "extensions to be requested:\n";

    for (const char* extensionName : extensions) {
      std::cout << "\t\"" << extensionName << "\"\n";
    }
  }

  
  // Validation Layers Setup
  std::vector<const char*> layers;
  if (debug) layers.push_back("VK_LAYER_KHRONOS_validation");

  if (!supported(extensions, layers, debug)) return nullptr;


  vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo(
    vk::InstanceCreateFlags(),
    &appInfo,
    static_cast<uint32_t>(layers.size()), layers.data(), // enabled layers
    static_cast<uint32_t>(extensions.size()), extensions.data() // enabled extensions
  );


  try {
    return vk::createInstance(createInfo);
  } catch (vk::SystemError err) {
    if (debug) {
      std::cout << "Failed to create Instance!\n";
    }
    return nullptr;
  }
}
}
