#include <routing.hpp>

using namespace uva;
using namespace routing;

basic_controller::basic_controller()
{

}

uva::routing::basic_controller::~basic_controller()
{
}

std::map<std::string, std::function<basic_action_target(std::shared_ptr<basic_connection> connection)>> &uva::routing::get_routes()
{
    static std::map<std::string, std::function<basic_action_target(std::shared_ptr<basic_connection> connection)>> routes;
    return routes;
}

void uva::routing::add_route(const std::string &route, const std::function<basic_action_target(std::shared_ptr<basic_connection> connection)> action)
{
    auto& routes = get_routes();
    routes.insert({route, action});
}

basic_action_target uva::routing::find_dispatch_target(const std::string &action, std::shared_ptr<basic_connection> connection)
{
    auto& routes = get_routes();

    auto it = routes.find(action);

    if(it != routes.end()) {
        return it->second(connection);
    }

    return basic_action_target();
}

bool uva::routing::dispatch(basic_action_target target, std::shared_ptr<basic_connection> connection)
{
    bool should_call = true;

    for(auto before_action : target.controller->before_actions) {
        if(before_action->should_before_action(target.action)) {
            if(!before_action->do_before()) {
                return false;
                break;
            }
        }
    }
    
    target.call();
    
    return true;
}

uva::routing::basic_action_target::basic_action_target(const std::string &__action, std::function<void()> __call, const std::shared_ptr<basic_controller> &__controller)
    : action(__action), call(__call), controller(__controller)
{

}
