#pragma once
#include "App.xaml.g.h"

namespace winrt::UltimateTicTacToe_GUI::implementation {
	struct App : AppT<App> {
		App();

		void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs const&);
		void OnSuspending(IInspectable const&, Windows::ApplicationModel::SuspendingEventArgs const&);
		void OnNavigationFailed(IInspectable const&, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs const&);
	};
}
