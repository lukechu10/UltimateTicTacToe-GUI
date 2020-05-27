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

void MainPage::newGameButton_Click(IInspectable const&, RoutedEventArgs const&) {
	m_gameBoard = make_unique<Game>();

	RenderGameBoard();
	NewGameControl().Flyout().Hide(); // hide flyout
}

void MainPage::RenderGameBoard() {
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
				quadrantGrid.Style(Resources().Lookup(box_value(L"QuadrantGrid")).as<Windows::UI::Xaml::Style>());

				// add grid row and column definitions
				for (size_t i = 0; i < boardSize; i++) {
					Controls::RowDefinition rowDef;
					Controls::ColumnDefinition colDef;
					quadrantGrid.RowDefinitions().Append(rowDef);
					quadrantGrid.ColumnDefinitions().Append(colDef);
				}

				for (size_t row = 0; row < boardSize; row++) {
					for (size_t col = 0; col < boardSize; col++) {
						Controls::Button button; // button control
						hstring buttonStr;		 // button content

						switch (m_gameBoard->getBoard()[quadrantRow][quadrantCol][row][col]) {
						case Player::X:
							buttonStr = L"X";
							break;
						case Player::O:
							buttonStr = L"O";
							break;
						default:
							buttonStr = L" ";
							break;
						}

						button.Content(box_value(buttonStr));															  // get board data and set it to button content
						button.Style(Resources().Lookup(box_value(L"ButtonRevealStyle")).as<Windows::UI::Xaml::Style>()); // set reveal style for button
						button.Click({ this, &MainPage::HandleGameButtonClick });										  // attach click event handler
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

		// player can play anywhere initially
		UpdateGameBoardQuadrants();
	}
}

void MainPage::OpenHelp(IInspectable const& sender, RoutedEventArgs const& e) {
	Windows::System::Launcher::LaunchUriAsync(Windows::Foundation::Uri(L"https://en.wikipedia.org/wiki/Ultimate_tic-tac-toe#Rules"));
}

void MainPage::HandleGameButtonClick(IInspectable const& sender, RoutedEventArgs const& e) {
	Controls::Button button = sender.as<Controls::Button>();			// current target
	Controls::Grid quadrantGrid = button.Parent().as<Controls::Grid>(); // get quadrant board (parent of button)
	Controls::Grid mainGrid = quadrantGrid.Parent().as<Controls::Grid>();
	assert(mainGrid == GameBoardContainer());

	uint32_t row = quadrantGrid.GetRow(button);	   // sub row
	uint32_t col = quadrantGrid.GetColumn(button); // sub col
	uint32_t quadrantRow = mainGrid.GetRow(quadrantGrid);
	uint32_t quadrantCol = mainGrid.GetColumn(quadrantGrid);

	ClickLocation().Text(
		L"Row: " + to_hstring(row) + L" Col: " + to_hstring(col) +
		L" Quadrant Row: " + to_hstring(quadrantRow) + L" Quadrant Col: " + to_hstring(quadrantCol));

	// construct Play object
	Play userPlay(quadrantRow, quadrantCol, row, col);

	// check if Play is valid
	auto& plays = m_gameBoard->moves();

	bool isValid = false;
	for (auto& play : plays)
		if (play == userPlay)
			isValid = true;

	if (!m_gameBoard->isTerminal() && isValid) {
		ErrorMessageText().Text(L""); // remove error message

		// apply Play to GameBoard
		m_gameBoard->applyMove(userPlay);


		// update GameBoard UI
		Player player = m_gameBoard->getBoard()[quadrantRow][quadrantCol][row][col];
		switch (player) {
		case Player::X:
			button.Content(box_value(L"X"));
			break;
		case Player::O:
			button.Content(box_value(L"O"));
			break;
		default:
			button.Content(box_value(L" "));
		}

		// update GameBoard UI next quadrant
		UpdateGameBoardQuadrants();
	}
	else {
		ErrorMessageText().Text(L"That move is not availible!");
	}
}

void MainPage::UpdateGameBoardQuadrants() {
	const int32_t nextRow = m_gameBoard->getNextCoor().first;
	const int32_t nextCol = m_gameBoard->getNextCoor().second;

	NextQuadrantLocationText().Text(
		L"Next row: " + to_hstring(nextRow) + L" Next col: " + to_hstring(nextCol));

	for (auto& child : GameBoardContainer().Children())
	// iterate through all quadrant grids
	{
		Controls::Grid grid = child.as<Controls::Grid>();
		
		int32_t row = GameBoardContainer().GetRow(grid);
		int32_t col = GameBoardContainer().GetColumn(grid);

		if (m_gameBoard->getWinCache()[row][col] == Player::X) {
			grid.Style(Resources().Lookup(box_value(L"QuadrantXWin")).as<Windows::UI::Xaml::Style>()); // red highlight
		}
		else if (m_gameBoard->getWinCache()[row][col] == Player::O) {
			grid.Style(Resources().Lookup(box_value(L"QuadrantOWin")).as<Windows::UI::Xaml::Style>()); // green highlight
		}
		// check if grid is next quadrant
		else if ((nextRow == -1 && nextCol == -1) /* player can play anywhere */
			|| m_gameBoard->getWinCache()[nextRow][nextCol] == Player::X || m_gameBoard->getWinCache()[nextRow][nextCol] == Player::O || (nextRow == row && nextCol == col)) {
			grid.Style(Resources().Lookup(box_value(L"NextQuadrant")).as<Windows::UI::Xaml::Style>()); // purple highlight
		}
		else {
			grid.Style(Resources().Lookup(box_value(L"QuadrantGrid")).as<Windows::UI::Xaml::Style>());
		}
	}
}
