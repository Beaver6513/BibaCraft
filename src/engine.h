#pragma once
#include "config.h"

namespace VLKRenderer {
class Engine {
public:
  Engine();
  ~Engine();
private:
  //whether to print debug messages in functions
  bool debugMode = true;
  //glfw window parameters
  int width{ 640 };
  int height{ 480 };
  const char* window_title = "VLKRenderer_test";
  
  // Vulkan Instance
  vk::Instance instance{ nullptr };

  // Debug Utils
  vk::DebugUtilsMessengerEXT debugMessenger{ nullptr };
  vk::DispatchLoaderDynamic dldi;

  GLFWwindow* window{ nullptr };

  //  glfw setup
  void build_glfw_window();
  // Vulkan instance setup
  void instance_setup();
  // Debug Messenger setup
  void debugMessenger_setup();
};	
}

