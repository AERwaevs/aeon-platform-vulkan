#include <vk/Surface.h>

namespace aer::gfx::vk
{

#if defined( AEON_PLATFORM_WINDOWS )

template<> ref_ptr<Surface>
Surface::create( ref_ptr<Instance> instance, Window_t window )
{
    return ref_ptr<Win32Surface>( new Win32Surface( instance, window ) );
}

Win32Surface::Win32Surface( ref_ptr<Instance> instance, Window_t window )
: Surface( instance )
{
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo
    {
        VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        VK_NULL_HANDLE, // pNext
        VkWin32SurfaceCreateFlagsKHR{ 0 },
        GetModuleHandle( nullptr ),
        window
    };
    auto result = vkCreateWin32SurfaceKHR( *instance, &surfaceCreateInfo, VK_ALLOCATOR, &_surface );

#elif defined( AEON_PLATFORM_ANDROID )

template<> ref_ptr<Surface>
Surface::create( ref_ptr<Instance> instance, Window_t window )
{
    return ref_ptr<AndroidSurface>( new AndroidSurface( instance, window ) );
}

AndroidSurface::AndroidSurface( ref_ptr<Instance> instance, Window_t window )
: Surface( instance )
{
    VkAndroidSurfaceCreateInfoKHR surfaceCreateInfo
    {
        VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
        VK_NULL_HANDLE, // pNext
        VkAndroidSurfaceCreateFlagsKHR{ 0 },
        window
    };
    auto result = vkCreateAndroidSurfaceKHR( *instance, &surfaceCreateInfo, VK_ALLOCATOR, &_surface );

#elif defined( AEON_PLATFORM_LINUX )
template<> ref_ptr<Surface>
Surface::create( ref_ptr<Instance> instance, Window_t window )
{
    return ref_ptr<LinuxSurface>( new LinuxSurface( instance, window ) );
}

LinuxSurface::LinuxSurface( ref_ptr<Instance> instance, Window_t arg )
: Surface( instance )
{
    VkXcbSurfaceCreateInfoKHR surfaceCreateInfo
    {
        VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR, VK_NULL_HANDLE, VkXcbSurfaceCreateFlagsKHR{ 0 },
        arg.connection(),
        arg.window()
    };
    auto result = vkCreateXcbSurfaceKHR( *instance, &surfaceCreateInfo, VK_ALLOCATOR, &_surface );

#endif
    AE_WARN_IF( result != VK_SUCCESS, "Failed to create surface: %s", ResultMessage( result ) );
}

Surface::~Surface()
{
    vkDestroySurfaceKHR( *_instance, _surface, VK_ALLOCATOR );
}

}