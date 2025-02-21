#pragma once
#include "config.h"
#include <vulkan/vulkan_core.h>

namespace vkInit {
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

		if (debug) {
			std::cout << "extensions to be requested:\n";

			for (const char* extensionName : extensions) {
				std::cout << "\t\"" << extensionName << "\"\n";
			}
		}

		vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo(
			vk::InstanceCreateFlags(),
			&appInfo,
			0, nullptr, // enabled layers
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
