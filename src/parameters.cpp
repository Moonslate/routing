#include <routing.hpp>

using namespace uva;
using namespace routing;

parameters::parameters()
    : var(var::map())
{
    
}

parameters::parameters(const var& params)
    : var(params)
{

}