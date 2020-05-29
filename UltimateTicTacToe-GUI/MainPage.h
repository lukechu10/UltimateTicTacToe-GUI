﻿#pragma once

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

		void UpdateGameBoardQuadrants();
		Windows::Foundation::IAsyncAction ShowGameWinner();
	private:
		// computes the best move and mutates m_gameBoard
		Windows::Foundation::IAsyncAction AiTurn();

		std::unique_ptr<Game> m_gameBoard = nullptr;

		// X always starts and X is player
		bool m_isPlayerTurn = true;
	};
}

namespace winrt::UltimateTicTacToe_GUI::factory_implementation {
	struct MainPage : MainPageT<MainPage, implementation::MainPage> {
	};
}
