#pragma once

#include <memory>

#include "Game.h"

#include "MainPage.g.h"

namespace winrt::UltimateTicTacToe_GUI::implementation {
	struct MainPage : MainPageT<MainPage> {
		MainPage();

		void HandleNewGameButtonClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
		void StartNewGame();

		void RenderGameBoard();
		void OpenHelp(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

		void HandleGameButtonClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);

		void DifficultyComboBox_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e);

		void UpdateGameBoardQuadrants();
		Windows::Foundation::IAsyncAction ShowGameWinner();

	private:
		// computes the best move and mutates m_gameBoard
		Windows::Foundation::IAsyncAction AiTurn();

		std::unique_ptr<Game> m_gameBoard = nullptr;

		// X always starts and X is player
		bool m_isPlayerTurn = true;

		// time allowed for computer to run mcts simulations
		int m_simTime = 0;

		// player cannot change sim time during a game in progress. Change value in HandleNewGameButtonClick
		int m_tempSimTime = 0;
	};
}

namespace winrt::UltimateTicTacToe_GUI::factory_implementation {
	struct MainPage : MainPageT<MainPage, implementation::MainPage> {
	};
}
