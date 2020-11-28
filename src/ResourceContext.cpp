#include "ResourceContext.hpp"
#include "ResourceContextImpl.hpp"

namespace petrichor
{

    ResourceContext::ResourceContext() : impl(nullptr) {}

    ResourceContext::~ResourceContext()
    {
        if (impl)
        {
            delete impl;
        }
    }

}
