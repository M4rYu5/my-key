// TODO: Delete /lib/libevdev library. I don't think I'm gonna need it anymore.

#include <memory>
#include <iostream>
#include <cstdlib>

import System;
import Core;
import KeyDispatcher;
import Layers;



int main(int argc, char *argv[])
{
    int linux_device_id = 0;

#if LINUX
    if (argc > 1)
    {
        try
        {
            linux_device_id = std::stoi(argv[1]);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Invalid device ID: " << argv[1] << " (defaulting to 0)\n";
        }
    }
#endif

    HaKey::KeyDispatcher dispatcher;

    std::unique_ptr<HaKey::Core::KeyChainHandler> home_row = std::make_unique<HaKey::Layers::HomeRowNavigation>();
    dispatcher.Add(std::move(home_row));

    std::unique_ptr<HaKey::Core::KeyChainHandler> _60KeyRemap = std::make_unique<HaKey::Layers::_60KeyRemap>();
    dispatcher.Add(std::move(_60KeyRemap));

    dispatcher.Listen(linux_device_id);
};
