#include "shader.hpp"
#include "../logging/logger.hpp"
#include "../backend/file.hpp"
#include <vector>


std::vector<vk::ShaderEXT> make_shader_objects(vk::Device logicalDevice, 
    const char* vertexFilename, const char* fragmentFilename, vk::DispatchLoaderDynamic& dl,
    std::deque<std::function<void(vk::Device)>>& deviceDeletionQueue) {

    Logger* logger = Logger::get_logger();

    vk::ShaderCreateFlagsEXT flags = vk::ShaderCreateFlagBitsEXT::eLinkStage;
    vk::ShaderStageFlags nextStage = vk::ShaderStageFlagBits::eFragment;

    std::vector<char> vertexSrc = read_file(vertexFilename);
    vk::ShaderCodeTypeEXT codeType = vk::ShaderCodeTypeEXT::eSpirv;
    const char* pName = "main";

    vk::ShaderCreateInfoEXT vertexInfo = {};
    vertexInfo.setFlags(flags);
    vertexInfo.setStage(vk::ShaderStageFlagBits::eVertex);
    vertexInfo.setNextStage(nextStage);
    vertexInfo.setCodeType(codeType);
    vertexInfo.setCodeSize(vertexSrc.size());
    vertexInfo.setPCode(vertexSrc.data());
    vertexInfo.setPName(pName);

    std::vector<char> fragmentSrc = read_file(fragmentFilename);
    vk::ShaderCreateInfoEXT fragmentInfo = {};
    fragmentInfo.setFlags(flags);
    fragmentInfo.setStage(vk::ShaderStageFlagBits::eFragment);
    fragmentInfo.setCodeType(codeType);
    fragmentInfo.setCodeSize(fragmentSrc.size());
    fragmentInfo.setPCode(fragmentSrc.data());
    fragmentInfo.setPName(pName);

    std::vector<vk::ShaderCreateInfoEXT> shaderInfo;
    shaderInfo.push_back(vertexInfo);
    shaderInfo.push_back(fragmentInfo);

    vk::ResultValue result = logicalDevice.createShadersEXT(shaderInfo, nullptr, dl);
    std::vector<vk::ShaderEXT> shaders;
    
    if (result.result == vk::Result::eSuccess) {
        logger->print("Successfully made shaders");
        shaders = result.value;
        VkShaderEXT vertexShader = shaders[0];
        deviceDeletionQueue.push_back([vertexShader, dl](vk::Device device) {
            device.destroyShaderEXT(vertexShader, nullptr, dl);
        });
        VkShaderEXT fragmentShader = shaders[1];
        deviceDeletionQueue.push_back([fragmentShader, dl](vk::Device device) {
            device.destroyShaderEXT(fragmentShader, nullptr, dl);
        });
    }
    else {
        logger->print("Shader creation failed");
    }
    return shaders;
}
