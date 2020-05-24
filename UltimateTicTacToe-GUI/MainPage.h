#pragma once

#include <memory>

#include "Game.h"

#include "MainPage.g.h"

namespace winrt::UltimateTicTacToe_GUI::implementation {
	struct MainPage : MainPageT<MainPage> {
		MainPage();

		int32_t MyProperty();
		void MyProperty(int32_t value);

		void newGameButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

		void RenderGameBoard();
		void OpenHelp(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

		void HandleGameButtonClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

	private:
		std::unique_ptr<Game> m_gameBoard = nullptr;
	};
}

namespace winrt::UltimateTicTacToe_GUI::factory_implementation {
	struct MainPage : MainPageT<MainPage, implementation::MainPage> {
	};
}
