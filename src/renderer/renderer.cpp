#include "renderer.hpp"
#include "shader.hpp"
#include "device.hpp"

Engine::Engine(GLFWwindow* window) :
	window(window) {

	logger = Logger::get_logger();
	logger->print("Graphics engine Object constructor called");

  instance = make_instance("Test Application", instanceDeletionQueue);
  dldi = vk::DispatchLoaderDynamic(instance, vkGetInstanceProcAddr);
  debugMessenger = logger->make_debug_messenger(instance, dldi, instanceDeletionQueue);

  VkSurfaceKHR raw_surface;
	glfwCreateWindowSurface(instance, window, nullptr, &raw_surface);
	surface = raw_surface;
	instanceDeletionQueue.push_back([this](vk::Instance instance){
		instance.destroySurfaceKHR(surface);
	});

  physicalDevice = choose_physical_device(instance);

  logicalDevice = create_logical_device(physicalDevice, surface, deviceDeletionQueue);
  uint32_t graphicsQueueFamilyIndex = findQueueFamilyIndex(physicalDevice, surface, vk::QueueFlagBits::eGraphics);
  graphicsQueue = logicalDevice.getQueue(graphicsQueueFamilyIndex, 0);

  // Swapchain building
  int width, height;
  glfwGetWindowSize(window, &width ,&height);
  swapchain.build(logicalDevice, physicalDevice, surface, width, height, deviceDeletionQueue);
  std::vector<vk::Image> images = logicalDevice.getSwapchainImagesKHR(swapchain.chain).value;
  for (uint32_t i = 0; i < images.size(); ++i) frames.push_back(Frame(images[i], logicalDevice, swapchain.format.format, deviceDeletionQueue));

  // shader stuff
  shaders = make_shader_objects(logicalDevice,
    "shaders/vertex.spv", "shaders/fragment.spv", dldi,
    deviceDeletionQueue);
}

Engine::~Engine() {
  logger->print("Engine destructor called!");

	logger->print("Graphics engine Object destructor called");
  while (deviceDeletionQueue.size() > 0) {
		deviceDeletionQueue.back()(logicalDevice);
		deviceDeletionQueue.pop_back();
	}

	logger->print("Graphics engine Object destructor called");
  while (instanceDeletionQueue.size() > 0) {
		instanceDeletionQueue.back()(instance);
		instanceDeletionQueue.pop_back();
	}
}
