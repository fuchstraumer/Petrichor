#include "ResourceContext.hpp"
#include "ForwardDecl.hpp"


int main(int argc, char* argv[])
{
    using namespace petrichor;
    vpr::Device* device;
    vpr::PhysicalDevice* physicalDevice;

    auto& ctxt = ResourceContext::Get(device, physicalDevice);
    ctxt.Construct(device, physicalDevice);

    

}
