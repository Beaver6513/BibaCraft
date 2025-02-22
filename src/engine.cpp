#include "engine.h"
#include "instance.h"
#include "logging.h"


namespace VLKRenderer {
Engine::Engine() {

  if (debugMode) {
    std::cout << "VLKRenderer constructor called\n";
  }
  
  build_glfw_window();
  instance_setup();
  debugMessenger_setup();
}

void Engine::build_glfw_window() {

  //initialize glfw
  glfwInit();

  //no default rendering client, we'll hook vulkan up
  //to the window later
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  //resizing breaks the swapchain, we'll disable it for now
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  //GLFWwindow* glfwCreateWindow (int width, int height, const char *title, GLFWmonitor *monitor, GLFWwindow *share)
  if ((window = glfwCreateWindow(width, height, window_title, nullptr, nullptr))) {
    if (debugMode) {
      std::cout << "Successfully made a glfw window called "<< window_title <<", width: " << width << ", height: " << height << '\n';
    }
  }
  else {
    if (debugMode) {
      std::cout << "GLFW window creation failed\n";
    }
  }
}

void Engine::instance_setup() {
  instance = vkInit::make_vk_Instance(debugMode, window_title);
  dldi = vk::DispatchLoaderDynamic(instance, vkGetInstanceProcAddr); 
}

void Engine::debugMessenger_setup() {
  debugMessenger = vkInit::debugMessenger_setup(instance, dldi);
}

Engine::~Engine() {

  if (debugMode) {
    std::cout << "VLKRenderer destructor called\n";
  }
    
  instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dldi);
  //destroy allocated vulkan instance
  instance.destroy();
  //terminate glfw
  glfwTerminate();
}
}

