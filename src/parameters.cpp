#include <routing.hpp>

using namespace uva;
using namespace routing;

parameters::parameters()
    : var(empty_map)
{
    
}

parameters::parameters(const var& params)
    : var(params)
{

}