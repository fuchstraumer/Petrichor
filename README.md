# Petrichor

Baseline rendering functionalities for my Vulkan rendering engine - resources, memory, instances, devices, and windows

Still setting this up. Got ridiculously tired of dealing with dragging in the same common set of functionality across a ton of my projects - creating a Vulkan Instance, managing physical devices, creating a logical device, creating a platform-appropriate window, routing input, etc. Also includes things like spawning buffers and textures, and allocating memory for them: I imagine I'll have specialized use cases where I need to get down to bare Vulkan usages elsewhere, but for 90%+ of use cases what I have here (harvested from the resource_context in previous projects) should do the trick of handling these basic tasks. 

All of this stuff should be compatible with DLL builds as well, and I'm trying to keep to a C-style interface as best as I can. The most any of this includes is `vulkan_core.h`, since I can't think of a good way to abstract that and I'm not wholly sure I can just forward define what I need.