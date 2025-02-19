#include <vk/state/ShaderModule.h>

namespace aer::vk
{
    ShaderModule::ShaderModule( ref_ptr<Device> device, const ByteCode& byte_code )
    :   _device( device )
    {
        VkShaderModuleCreateInfo createInfo
        {
            VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            VK_NULL_HANDLE,     // pNext
            VkShaderModuleCreateFlags{ 0 },
            static_cast<uint32_t>( byte_code.size() ),
            reinterpret_cast<const uint32_t*>( byte_code.empty() ? VK_NULL_HANDLE : byte_code.data() )
        };

        auto result = vkCreateShaderModule( *device, &createInfo, VK_NULL_HANDLE, &_module );
        CHECK_F( result == VK_SUCCESS, "Failed to create shader module: %s", ResultMessage( result ) );
    }

    ShaderModule::~ShaderModule()
    {
        vkDestroyShaderModule( *_device, _module, VK_NULL_HANDLE );
    }
    
} // namespace aer

