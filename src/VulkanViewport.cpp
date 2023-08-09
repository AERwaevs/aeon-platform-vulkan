#include <Graphics/VulkanViewport.h>
#include <Graphics/VulkanRenderer.h>

#include <vk/PipelineLayout.h>
#include <vk/RenderPass.h>

#include <vk/state/ShaderModule.h>
#include <vk/state/ShaderStage.h>
#include <vk/state/DynamicState.h>
#include <vk/state/VertexInputState.h>
#include <vk/state/InputAssemblyState.h>
#include <vk/state/ViewportState.h>
#include <vk/state/RasterizationState.h>
#include <vk/state/MultisampleState.h>
#include <vk/state/DepthStencilState.h>
#include <vk/state/ColorBlendState.h>

namespace aer::gfx
{

template<>
ref_ptr<Viewport> Viewport::create< API::Vulkan >( Window* window )
{
    return VulkanViewport::create_if( VulkanSupported(), window );
}

const inline auto GetQueueSettings( vk::PhysicalDevice* physical_device, vk::Surface* surface )
{
    auto [ present_family, graphics_family ]
    {
        physical_device->GetQueueFamilies( VK_QUEUE_GRAPHICS_BIT, surface )
    };
    return vk::QueueSettings
    {
        vk::QueueSetting{ graphics_family,{ 1.0f } },
        vk::QueueSetting{ present_family, { 1.0f } },
    };
}

VulkanViewport::VulkanViewport( Window* window )
:   Viewport( window, VulkanRenderer::get_or_create() ),
    _instance( vk::Instance::get_or_create() ),
    _surface( vk::Surface::create( _instance, window->native<vk::Window_t>() ) ),
    _physical_device
    (
        _instance->physical_device( VK_QUEUE_GRAPHICS_BIT, _surface, _instance->DevicePreferences )
    ),
    _device( vk::Device::create
    (
        _physical_device, _surface, GetQueueSettings( _physical_device, _surface )
    ) )
{
    using namespace vk;

    //? if _swapchain is created during initialisation, it seems to execute before _swapchain_prefs is constructed
    _swapchain = Swapchain::create
    (
        _physical_device, _device, _surface,
        window->width(), window->height(), _swapchain_prefs
    );

    //* create shaders
    auto vert_code   = ByteCode::read( "shaders/tri.vert.spv" );
    auto frag_code   = ByteCode::read( "shaders/tri.frag.spv" );
    auto vert_module = ShaderModule::create( _device, *vert_code );
    auto frag_module = ShaderModule::create( _device, *frag_code );
    ref_ptr<ShaderStage> stages[]
    {
        ShaderStage::create( VK_SHADER_STAGE_VERTEX_BIT, vert_module, "main" ),
        ShaderStage::create( VK_SHADER_STAGE_FRAGMENT_BIT, frag_module, "main" )
    };

    //* create renderPass
    RenderPass::Attachments attachments
    {
        { .format = _swapchain->format() }, // colorAttachment
        { .format = VK_FORMAT_D32_SFLOAT }  // depthAttachment
    };
    AttachmentReference colorAttachmentRef
    {
        .attachment = 0,
        .layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };
    SubpassDependency colorDependency
    {
        .srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
    };
    // TODO depth buffering
    AttachmentReference depthAttachmentRef
    {
        .attachment = 1,
        .layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    };
    SubpassDependency depthDependency
    {
        .srcStageMask  = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
        .dstStageMask  = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
        .srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
        .dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
    };
    RenderPass::Subpasses subpasses
    {{
            .colorAttachments{ colorAttachmentRef },
            .depthStencilAttachments{ depthAttachmentRef }
    }};
    RenderPass::Dependencies dependencies
    {
        colorDependency,
        depthDependency
    };
    auto renderpass             = RenderPass::create( _device, attachments, subpasses, dependencies );

    //* create graphics pipeline
    auto dynamic_state          = DynamicState::create();
    auto vertex_input_state     = VertexInputState::create();
    auto input_assembly_state   = InputAssemblyState::create();
    auto viewport_state         = ViewportState::create( _swapchain->extent() );
    auto rasterization_state    = RasterizationState::create();
    auto multisample_state      = MultisampleState::create( VK_SAMPLE_COUNT_4_BIT );
    auto depthstencil_state     = DepthStencilState::create();
    auto colorblend_state       = ColorBlendState::create();
    auto pipeline_layout        = PipelineLayout::create( _device );
         pipeline_layout->Compile( _device );
}

VulkanViewport::~VulkanViewport()
{
}

} // namespace aer::gfx
