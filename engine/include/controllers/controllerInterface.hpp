#ifndef _CONTROLLER_INTERFACE_HPP_
#define _CONTROLLER_INTERFACE_HPP_

namespace Core
{
    class Engine;
}

namespace Controller
{
    class ControllerInterface
    {
    private:

    public:
        virtual void inputs(const Core::Engine& engine) = 0;
    };
}

#endif