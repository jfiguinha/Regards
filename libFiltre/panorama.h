#pragma once

#include "panorama.g.h"

namespace winrt::libFiltre::implementation
{
    struct panorama : panoramaT<panorama>
    {
        panorama();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void ClickHandler(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::libFiltre::factory_implementation
{
    struct panorama : panoramaT<panorama, implementation::panorama>
    {
    };
}
