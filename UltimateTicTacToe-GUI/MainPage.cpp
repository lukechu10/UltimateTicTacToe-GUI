#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"

using namespace std;
using namespace winrt;
using namespace Windows::UI::Xaml;

using namespace winrt::UltimateTicTacToe_GUI::implementation;

MainPage::MainPage() {
	InitializeComponent();
}

int32_t MainPage::MyProperty() {
	throw hresult_not_implemented();
}

void MainPage::MyProperty(int32_t /* value */) {
	throw hresult_not_implemented();
}

void MainPage::newGameButton_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Windows::UI::Xaml::RoutedEventArgs const&) {
	m_gameBoard = make_unique<Game>();

	GameBoardUpdated();
	NewGameControl().Flyout().Hide(); // hide flyout
}

void MainPage::GameBoardUpdated() {
	if (m_gameBoard != nullptr) {
		// game board is not a nullptr, render game
		
		// reset game board
		GameBoardContainer().Children().Clear();

		const size_t boardSize = m_gameBoard->getBoard().size();

		for (size_t quadrantRow = 0; quadrantRow < boardSize; quadrantRow++) {
			for (size_t quadrantCol = 0; quadrantCol < boardSize; quadrantCol++) {
				// grid for quadrant (sub grid)
				Controls::Grid quadrantGrid;
				quadrantGrid.RowSpacing(3);
				quadrantGrid.ColumnSpacing(3);

				// add grid row and column definitions
				for (size_t i = 0; i < boardSize; i++) {
					Controls::RowDefinition rowDef;
					Controls::ColumnDefinition colDef;
					quadrantGrid.RowDefinitions().Append(rowDef);
					quadrantGrid.ColumnDefinitions().Append(colDef);
				}

				for (size_t row = 0; row < boardSize; row++) {
					for (size_t col = 0; col < boardSize; col++) {
						Controls::Button button;
						hstring buttonStr = winrt::to_hstring(playerToString(m_gameBoard->getBoard()[quadrantRow][quadrantCol][row][col]));
						button.Content(box_value(buttonStr)); // get board data and set it to button content
						button.Width(30);
						button.Height(30);

						// add button to quadrant grid
						quadrantGrid.Children().Append(button);
						quadrantGrid.SetRow(button, row);
						quadrantGrid.SetColumn(button, col);
					}
				}

				GameBoardContainer().Children().Append(quadrantGrid);
				GameBoardContainer().SetRow(quadrantGrid, quadrantRow);
				GameBoardContainer().SetColumn(quadrantGrid, quadrantCol);
			}
		}
	}
}


void MainPage::OpenHelp(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e) {
	Windows::System::Launcher::LaunchUriAsync(Windows::Foundation::Uri(L"https://en.wikipedia.org/wiki/Ultimate_tic-tac-toe#Rules"));
}
