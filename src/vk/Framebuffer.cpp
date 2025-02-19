#include <vk/Framebuffer.h>

namespace aer::vk
{
    
Framebuffer::Framebuffer( ref_ptr<RenderPass> renderPass, const ImageViews& attachments, uint32_t width, uint32_t height, uint32_t layers )
    : _device( *renderPass->device ), _framebuffer( VK_NULL_HANDLE ), _width( 0 ), _height( 0 ), _layers( 0 )
{
    std::vector<VkImageView> vk_attachments;
    vk_attachments.reserve( attachments.size() );
    for( auto& attachment : attachments )
    {
        vk_attachments.emplace_back( *attachment );
    }

    VkFramebufferCreateInfo createInfo
    {
        .sType              = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .pNext              = VK_NULL_HANDLE,
        .flags              = VkFramebufferCreateFlags{ 0 },
        .renderPass         = *renderPass,
        .attachmentCount    = static_cast<uint32_t>( vk_attachments.size() ),
        .pAttachments       = vk_attachments.data(),
        .width              = width,
        .height             = height,
        .layers             = layers
    };

    auto result = vkCreateFramebuffer( _device, &createInfo, VK_ALLOCATOR, &_framebuffer );
    CHECK_F( result == VK_SUCCESS, "Failed to create framebuffer: vk%d", result );
}

Framebuffer::~Framebuffer()
{
    if( _framebuffer != VK_NULL_HANDLE )
    {
        vkDestroyFramebuffer( _device, _framebuffer, VK_ALLOCATOR );
    }
}

} // namespace aer::vk
