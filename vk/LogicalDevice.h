#pragma once

#include <Vendor/glad/include/glad/vulkan.h>

#include <vk/Instance.h>
#include <vk/PhysicalDevice.h>

namespace AEON::Graphics::vk
{

class AEON_DLL Device : public Inherit< Object, Device >
{
public:
                Device( PhysicalDevice* physical_device, Surface* surface );
    operator    VkDevice() const { return m_device; }

    
    template< typename F >
    VkResult GetProcAddr( F& proc_addr, const char* name ) const
    {
        proc_addr = reinterpret_cast<F>( vkGetDeviceProcAddr( m_instance, name ) );
        if( proc_addr == nullptr )
        {
            AE_WARN( "Failed to get procedural address for %s: vk%d", 
                        name,
                        VK_ERROR_EXTENSION_NOT_PRESENT );
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
        else return VK_SUCCESS;
    }

protected:
    virtual ~Device();

private:
    VkDevice                m_device;

    Observer<Instance>      m_instance;
    Shared<PhysicalDevice>  m_physical_device;

    VkQueue                 m_queue_graphics;
    VkSurfaceKHR            m_surface;
    VkQueue                 m_queue_present;
};

}