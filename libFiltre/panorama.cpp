#include "pch.h"
#include "panorama.h"
#if __has_include("panorama.g.cpp")
#include "panorama.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::libFiltre::implementation
{
    panorama::panorama()
    {
        InitializeComponent();
    }

    int32_t panorama::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void panorama::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void panorama::ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
        Button().Content(box_value(L"Clicked"));
    }
}
