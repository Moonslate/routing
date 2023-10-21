#include <routing.hpp>
#include <console.hpp>

using namespace uva;
using namespace routing;
using namespace console;

uva::routing::basic_before_action::basic_before_action(std::function<bool()> __before, basic_controller* controller)
    : before(__before), match(before_action_match::exclude)
{
    controller->before_actions.push_back(this);
}

bool basic_before_action::should_before_action(const std::string &action)
{
    auto it = std::find(actions.begin(), actions.end(), action);
    bool contains = it != actions.end();

    if(match == before_action_match::include) {
        return contains;
    } else {
        return !contains;
    }
}

bool basic_before_action::do_before()
{
    try {
        return before();
    } catch(const std::exception& e) {
        log_error("error during before_action: {}", e.what());
        return false;
    }
}
