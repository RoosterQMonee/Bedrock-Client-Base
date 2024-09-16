#include <Utils/System/Window.h>
#include <Windows.h>


void SystemUtils::SetWindowTitle(const winrt::param::hstring& title) {
	auto w = winrt::Windows::ApplicationModel::Core::CoreApplication::MainView().CoreWindow().Dispatcher().RunAsync(winrt::Windows::UI::Core::CoreDispatcherPriority::Normal, [&title]() {
		winrt::Windows::UI::ViewManagement::ApplicationView::GetForCurrentView().Title(title);
	});
}