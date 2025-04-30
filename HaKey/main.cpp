// TODO: Delete /lib/libevdev library. I don't think I'm gonna need it anymore.

#include <memory>
#include <iostream>


import System;
import Core;
import KeyDispatcher;



class Layer1 : public HaKey::Core::KeyChainHandler{
    void OnKey(HaKey::Core::KeyEvent key, std::shared_ptr<HaKey::Core::KeyResult> result) override{
        std::cout << key.key_code << " " << key.state  << "\n";
        next(key, result);
    };
};


int main()
{
    HaKey::KeyDispatcher d;
    std::unique_ptr<HaKey::Core::KeyChainHandler> l = std::make_unique<Layer1>();
    d.Add(std::move(l));
    d.Listen();
};



