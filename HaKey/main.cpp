#include <memory>
#include <iostream>
#include <exception>
#include <cstdlib>

import KeyDispatcher;
import Layers;

int main(int argc, char *[])
{
#if LINUX
    if (argc > 1)
    {
        std::cerr << "Usage: HaKey\n";
        return EXIT_FAILURE;
    }
#endif

    HaKey::KeyDispatcher dispatcher;

    dispatcher.Add(std::make_unique<HaKey::Layers::HomeRowNavigation>());
    dispatcher.Add(std::make_unique<HaKey::Layers::_60KeyRemap>());
    dispatcher.Add(std::make_unique<HaKey::Layers::StringReplaceLayer>());

    try
    {
        dispatcher.Listen();
    }
    catch (const std::exception &error)
    {
        std::cerr << error.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
