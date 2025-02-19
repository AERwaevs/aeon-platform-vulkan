#pragma once

#include "../vk.h"

namespace aer::vk
{
    
class GraphicsPipelineState : public Object
{
public:
    virtual void    apply( VkGraphicsPipelineCreateInfo& ) const {};
    virtual         ~GraphicsPipelineState() noexcept = default;
};

using GraphicsPipelineStates = std::vector<ref_ptr<GraphicsPipelineState>>;

} // namespace aer::vk
