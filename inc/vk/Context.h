#pragma once

#include "vk.h"
#include "vk/LogicalDevice.h"
#include "vk/RenderPass.h"
#include "vk/state/GraphicsPipelineState.h"

#include <Base/memory/scratch_memory.h>

namespace aer::gfx::vk
{
    
class Context : public Object, public Interfaces< Context, ICreate >
{
public:
    explicit Context( Device* in_device );
    virtual  ~Context() = default;
public:
    ref_ptr<Device>         device;
    ref_ptr<scratch_memory> scratchMemory;
    ref_ptr<RenderPass>     renderPass;

    GraphicsPipelineStates  states;
};

} // namespace aer::gfx::vk
