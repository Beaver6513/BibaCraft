#include "device.hpp"
#include "../logging/logger.hpp"

bool supports(
    const vk::PhysicalDevice& device,
	const char** ppRequestedExtensions,
	const uint32_t requestedExtensionCount) {

  Logger* logger = Logger::get_logger();
  logger->print("Requested physical device with following extensions: ");
  logger->print_list(ppRequestedExtensions, requestedExtensionCount);

  std::vector<vk::ExtensionProperties> extensions = device.enumerateDeviceExtensionProperties().value;
  logger->print("Physical device can support: ");
  logger->print_extensions(extensions);

  for (uint32_t i = 0; i < requestedExtensionCount; ++i) {
    bool supported = false;
    for(vk::ExtensionProperties& extension : extensions) {
      std::string name = extension.extensionName;
      if(!name.compare(ppRequestedExtensions[i])) {
        supported = true;
        break;
      }
    }
    if (!supported) return true;
  }
  return true;
}

bool is_suitable(const vk::PhysicalDevice& device) {
  Logger* logger = Logger::get_logger();
  logger->print("Checking if device can support the requested extension");
  const char* ppRequestedExtensions = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
  bool isSupported = supports(device, &ppRequestedExtensions, 1);
  std::stringstream message;
  message << "Device " << (isSupported ? "can" : "can't") << " support the requested extension";
  logger->print(message.str());
  return isSupported;
}

vk::PhysicalDevice choose_physical_device(const vk::Instance& instance) {
  Logger* logger = Logger::get_logger();
  logger->print("Choosing physical device...");
  std::vector<vk::PhysicalDevice> aviableDevices = instance.enumeratePhysicalDevices().value;
  for (vk::PhysicalDevice device : aviableDevices) {
    logger->log(device);
    if (is_suitable(device)) {
      logger->print("This device has been choosen!");
      return device;
    }
  }
  return nullptr;
}

uint32_t findQueueFamilyIndex(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, vk::QueueFlags queueType) {
Logger* logger = Logger::get_logger();

	std::vector<vk::QueueFamilyProperties> queueFamilies = physicalDevice.getQueueFamilyProperties();
	logger->log(queueFamilies);

	for (uint32_t i = 0; i < queueFamilies.size(); ++i) {
		vk::QueueFamilyProperties queueFamily = queueFamilies[i];

		bool canPresent = false;
		if (surface) {
			if (physicalDevice.getSurfaceSupportKHR(i, surface).result == vk::Result::eSuccess) {
				canPresent = true;
			}
		} else canPresent = true;

		bool supported = false;
		if (queueFamily.queueFlags & queueType) supported = true;
		if (supported && canPresent) return i;
	}
	return UINT32_MAX;}

	Logger* logger = Logger::get_logger();
  vk::Device create_logical_device(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface,
    std::deque<std::function<void(vk::Device)>>& deletionQueue) {

	/*
	* At time of creation, any required queues will also be created,
	* so queue create info must be passed in.
	*/
	uint32_t graphicsIndex = findQueueFamilyIndex(physicalDevice, surface, vk::QueueFlagBits::eGraphics);
	float queuePriority = 1.0f;

	vk::DeviceQueueCreateInfo queueCreateInfo = vk::DeviceQueueCreateInfo(
		vk::DeviceQueueCreateFlags(), graphicsIndex, 
		1, &queuePriority);

	/*
	* Device features must be requested before the device is abstracted,
	* so that we only pay for what we need.
	*/
	vk::PhysicalDeviceFeatures deviceFeatures = vk::PhysicalDeviceFeatures();
	uint32_t enabledLayerCount = 0;
	const char** ppEnabledLayers = nullptr;
	if (logger->is_enabled()) {
		enabledLayerCount = 1;
		ppEnabledLayers = (const char**)malloc(sizeof(const char*));
		ppEnabledLayers[0] = "VK_LAYER_KHRONOS_validation";
	}

	/*
	* Extensions, not used for now
	*/
	uint32_t enabledExtensionCount = 1;
	const char** ppEnabledExtensions = (const char**)malloc(enabledExtensionCount * sizeof(char*));
	ppEnabledExtensions[0] = "VK_KHR_swapchain";
	//ppEnabledExtensions[1] = "VK_EXT_shader_object";

	vk::DeviceCreateInfo deviceInfo = vk::DeviceCreateInfo(
		vk::DeviceCreateFlags(), 
		1, &queueCreateInfo,
		enabledLayerCount, ppEnabledLayers,
		enabledExtensionCount, ppEnabledExtensions, // for now no extensions
		&deviceFeatures);

	vk::ResultValueType<vk::Device>::type logicalDevice = physicalDevice.createDevice(deviceInfo);
	if (logicalDevice.result == vk::Result::eSuccess) {
		logger->print("GPU has been successfully abstracted!");

		deletionQueue.push_back([](vk::Device device) {
			device.destroy();
			logger->print("Deleted Logical Device!");
		});

		return logicalDevice.value;
	} else {
		logger->print("Device creation failed!");
		return nullptr;
	}
}
