#pragma once

#include <map>
#include <string>
#include <functional>

#include <core.hpp>

#define ROUTE(path, action_handler)\
route(path, &action_handler, #action_handler)\

namespace uva
{
    namespace routing
    {
        struct parameters : var {
            parameters();
            parameters(const var& params);
        };

        enum class before_action_match
        {
            include,
            exclude
        };
        class basic_controller;
        class basic_before_action
        {
        public:
            basic_before_action(std::function<bool()> __before, basic_controller* controller);
        private:
            std::function<bool()> before;
            before_action_match match;
            std::vector<std::string> actions;
        public:    
            bool should_before_action(const std::string& action);
            bool do_before();
        };

        class basic_controller
        {
        public:
            basic_controller();
            virtual ~basic_controller();
        public:
            std::vector<basic_before_action*> before_actions;
            parameters params;
            std::string name;
        };

        class basic_connection
        {
            protected:
                std::map<std::string, std::shared_ptr<basic_controller>> controllers;
            public:
                template<class controller>
                std::shared_ptr<basic_controller> get_controller(const std::string& controller_name)
                {
                    auto it = controllers.find(controller_name);

                    if(it == controllers.end()) {
                        std::shared_ptr<basic_controller> ctrler = std::make_shared<controller>();
                        controllers.insert({controller_name, ctrler});
                        return ctrler;
                    }

                    return it->second;
                }
        };

        class basic_action_target
        {
        public:
            basic_action_target() = default;
            basic_action_target(const std::string& __action, std::function<void()> __call, const std::shared_ptr<basic_controller>& __controller);
            std::string action;
            std::function<void()> call;
            std::shared_ptr<basic_controller> controller;

            bool any_before_action_blocked();
        };

        template<class controller>
        void route(const std::string& route, void (controller::*fn)(), const std::string& action)
        {
            add_route(route, [fn, action](std::shared_ptr<basic_connection> connection) {
                static std::string separator = "::";
                size_t separator_index = action.find(separator);
                std::string action_name = action.substr(separator_index+separator.size());
                std::string ctrler_name = action.substr(0, separator_index);
                auto ctrler = connection->get_controller<controller>(ctrler_name);
                ctrler->name = ctrler_name;

                return basic_action_target(action_name, [ctrler,fn](){ (*(std::static_pointer_cast<controller>(ctrler)).*fn)(); }, ctrler);
            });
        }


		std::map<std::string, std::function<basic_action_target(std::shared_ptr<basic_connection> connection)>>& get_routes();
		void add_route(const std::string& route, const std::function<basic_action_target(std::shared_ptr<basic_connection> connection)> action_handler);

        basic_action_target find_dispatch_target(const std::string& action, std::shared_ptr<basic_connection> connectinon);
        bool dispatch(basic_action_target target, std::shared_ptr<basic_connection> connectinon);
    }; // namespace routing
    
}; // namespace uva

#define CONCATENATE_DIRECT(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_DIRECT(s1, s2)

#define before_action(x) basic_before_action CONCATENATE(before_action_line, __LINE___) = basic_before_action([this]() { return x(); }, this);