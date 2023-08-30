#pragma once

#include "GraphicsPipelineState.h"

namespace aer::gfx::vk
{
    
struct VertexInputState : public GraphicsPipelineState, public Interfaces< VertexInputState, ICreate, ITypeInfo >
{
    using Bindings   = std::vector<VkVertexInputBindingDescription>;
    using Attributes = std::vector<VkVertexInputAttributeDescription>;

                VertexInputState();
    explicit    VertexInputState( const Bindings&, const Attributes& );

    Bindings    vertexBindingDescriptions;
    Attributes  vertexAttributeDescriptions;

    void        apply( VkGraphicsPipelineCreateInfo& ) const override;
protected:
    virtual         ~VertexInputState() = default;
};
    
} // namespace aer::gfx::vk
