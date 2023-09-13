#include <routing.hpp>

using namespace uva;
using namespace routing;

basic_controller::basic_controller()
{

}

uva::routing::basic_controller::~basic_controller()
{
}

std::vector<std::pair<std::string, std::function<basic_action_target(std::shared_ptr<basic_connection> connection)>>> routes_with_keywords;

std::map<std::string, std::function<basic_action_target(std::shared_ptr<basic_connection> connection)>> &uva::routing::get_routes()
{
    static std::map<std::string, std::function<basic_action_target(std::shared_ptr<basic_connection> connection)>> routes;
    return routes;
}

void uva::routing::add_route(const std::string &route, const std::function<basic_action_target(std::shared_ptr<basic_connection> connection)> action)
{
    auto& routes = get_routes();
    if(route.find(':') != std::string::npos) {
        routes_with_keywords.push_back({route, action});
    }
    routes.insert({route, action});
}

basic_action_target uva::routing::find_dispatch_target(const std::string &action, std::shared_ptr<basic_connection> connection)
{
    std::string __action = action;
    std::string value;
    value.reserve(64);

    std::string key;
    key.reserve(64);

    auto& routes = get_routes();

    auto it = routes.find(__action);

    if(it == routes.end()) {
        for(size_t i = 0; i < routes_with_keywords.size(); ++i) {
            //todo
            //create array with the prefixes

            key.clear();
            value.clear();

            std::string prefix = routes_with_keywords[i].first.substr(0, routes_with_keywords[i].first.find(":"));
            if(action.starts_with(prefix)) {

                std::string_view action_view = __action;
                std::string_view route_with_keyword_view = routes_with_keywords[i].first;

                std::string_view action_without_prefix = action_view.substr(prefix.size());
                std::string_view route_with_keyword_without_prefix = route_with_keyword_view.substr(prefix.size());

                //removes :
                route_with_keyword_without_prefix.remove_prefix(1);

                while(route_with_keyword_without_prefix.size() && isalnum(route_with_keyword_without_prefix.front())) {
                    key.push_back(route_with_keyword_without_prefix.front());
                    route_with_keyword_without_prefix.remove_prefix(1);
                }

                while(action_without_prefix.size() && isalnum(action_without_prefix.front())) {
                    value.push_back(action_without_prefix.front());
                    action_without_prefix.remove_prefix(1);
                }

                if((!action_without_prefix.size() && !route_with_keyword_without_prefix.size()) || __action.ends_with(route_with_keyword_without_prefix)) {
                    __action = routes_with_keywords[i].first;
                    it = routes.find(__action);
                    break;
                }
            }
        }
    }

    if(it != routes.end()) {
        auto target = it->second(connection);
        target.controller->params.clear();

        if(key.size()) {
            target.controller->params[key] = value;
        }

        return target;
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
