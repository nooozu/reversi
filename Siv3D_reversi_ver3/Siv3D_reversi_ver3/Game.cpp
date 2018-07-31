#include "Game.h"
#include "Management.h"
#include "Result.h"

Game::Game()
{
	Graphics::SetBackground(Palette::Lightblue);
}

Game::~Game()
{
}

void Game::Next()
{
	if (step == e_next) {
		//Println(L"盤面チェック開始");

		//置ける場所を調べる
		moves = get_moves(board, turn);

		//置ける場所がない場合
		if (moves.size() == 0) {
			//手番を交代
			turn = -turn;
			//置ける場所を求める
			moves = get_moves(board, turn);
			//両者置ける場所がない場合ゲーム終了
			if (moves.size() == 0) {
				/*
				Println(L"ゲーム終了");
				Println(L"ターン数:", turn_count);
				
				std::string str = "black : " + std::to_string(color_count(board, COLOR_BLACK)) + "  vs  white : " + std::to_string(color_count(board, COLOR_WHITE));
				Println(Widen(str));
				*/
				Management::SceneChange(new Result(color_count(board, COLOR_BLACK), color_count(board, COLOR_WHITE)));
			}
		}
		
		moves = get_moves(board, turn);
		step = e_inputWait;
		
		turn_count++;
		//Println(L"盤面チェック完了");
		System::Sleep(500);
	}
}

void Game::Enter()
{
	step = e_next;

	turn = -1;
	turn_count = 0;
	depth *= 2;
	memset(board, 0, sizeof(board));
	board[3][3] = board[4][4] = 1;
	board[3][4] = board[4][3] = -1;

	log_data.LoadFile("reversi_log.txt");

	font_text = Font(20);
}

void Game::Exit()
{
	log_data.Delete();
}

void Game::Update()
{
	//置ける場所のチェックとゲームが終局しているか調べる
	Next();

	//置ける場所が存在する
	if (moves.size() != 0) {
		int x = 0, y = 0;
		if (turn == -1) {//プレイヤー
			//値を入力
			if (step == e_inputWait) {
				if (PlayerInput(turn, x, y)) {
					step = e_inputCheck;
				}
			}
			//入力された値を反映させる
			if (step == e_inputCheck) {
				for (auto it = moves.begin(); it != moves.end(); it++) {
					//入力が正しければ手番を交代
					if (x * 8 + y == ((*it).first)) {
						make_move(board, x, y, turn, (*it).second);
						turn = -turn;
						step = e_next;
						//Println(L"プレイヤー入力完了");
						std::string str = std::to_string(y) + "," + std::to_string(x) + "\n";
						//Println(Widen(str));
						log_data.WriteString(str);
						break;
					}
					//入力が正しくなければ再入力
					else {
						step = e_inputWait;
					}
				}
			}
		}
		else {//コンピュータ
			//値の入力
			
			if (turn_count > 40) {
				x = negamax(board, turn, 5);
			}
			else if (turn_count > 10) {
				x = input_ai_count_check(board, turn);
			}
			else {
				x = input_ai_checkseet(board, turn);
			}

			for (auto it = moves.begin(); it != moves.end(); it++) {
				if (x == ((*it).first)) {
					make_move(board, x / 8, x % 8, turn, (*it).second);
					turn = -turn;
					step = e_next;

					//角を取れたらcheckseetを修正
					Stone();

					//Println(L"コンピュータ入力完了");
					std::string str = std::to_string(x % 8) + "," + std::to_string(x / 8) + "\n";
					log_data.WriteString(str);
					//Println(Widen(str));
					break;
				}
			}
		}
	}
}

void Game::Draw()
{
	const int start_x = 320 - ((30 + 3) * 8) / 2;
	const int start_y = 100;
	const int grid_size = 30;
	const int space_size = 3;
	const int radius = 13;

	if (turn < 0) {
		font_text(L"黒のターン").drawCenter(320, 40, Palette::Black);
	}
	else if (turn > 0) {
		font_text(L"白のターン").drawCenter(320, 40, Palette::White);
	}

	Point pos;
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			//ボードを表示
			pos.x = start_x + (grid_size + space_size) * x;
			pos.y = start_y + (grid_size + space_size) * y;
			Rect rect = Rect(pos.x, pos.y, grid_size, grid_size);
			rect.draw(Palette::Green);

			//マウスが上にあったら枠を表示
			if (rect.mouseOver) {
				rect.drawFrame(1.0, 0.0, Palette::Yellow);
			}

			//白か黒の石を表示
			if (board[y][x] > 0) {
				Circle(pos.x + grid_size / 2, pos.y + grid_size / 2, radius).draw(Palette::White);
			}
			if (board[y][x] < 0) {
				Circle(pos.x + grid_size / 2, pos.y + grid_size / 2, radius).draw(Palette::Black);
			}

			//置ける場所を表示
			for (auto it = moves.begin(); it != moves.end(); it++) {
				if ((*it).first / 8 == y && (*it).first % 8 == x) {
					//Circle(pos.x + grid_size / 2, pos.y + grid_size / 2, radius / 4).draw(Palette::Yellow);
				}
			}
		}
	}
}

bool Game::PlayerInput(int color, int &ix, int &iy) {

	if (Input::MouseL.pressed) {
		const int start_x = 320 - ((30 + 3) * 8) / 2;
		const int start_y = 100;
		const int grid_size = 30;
		const int space_size = 3;

		Point pos;
		for (int y = 0; y < 8; y++) {
			for (int x = 0; x < 8; x++) {

				pos.x = start_x + (grid_size + space_size) * x;
				pos.y = start_y + (grid_size + space_size) * y;
				Rect grid = Rect(pos.x, pos.y, grid_size, grid_size);

				if (grid.mouseOver) {
					ix = y;
					iy = x;
					//Println(L"Mouse::Input   x = ", ix, L"y = ", iy);
					return true;
				}
			}
		}
	}

	return false;
}

std::vector< std::pair<int, int> > get_directions(int board[][8], int x, int y, int color) {
	std::vector< std::pair<int, int> > directions;
	if (board[x][y]) {
		return directions;
	}
	//north
	if ((y < 6) && (board[x][y + 1] == -color)) {
		for (int i = y + 2; i < 8; i++) {
			if (!board[x][i]) {
				break;
			}
			else if (board[x][i] == color) {
				directions.push_back(std::make_pair<int, int>(0, x * 8 + i));
				break;
			}
		}
	}
	//northeast
	if ((y < 6) && (x < 6) && (board[x + 1][y + 1] == -color)) {
		for (int i = 0; (x + i + 2 < 8) && (y + i + 2 < 8); i++) {
			if (!board[x + i + 2][y + i + 2]) {
				break;
			}
			else if (board[x + i + 2][y + i + 2] == color) {
				directions.push_back(std::make_pair<int, int>(1, (x + i + 2) * 8 + (y + i + 2)));
				break;
			}
		}
	}
	//east
	if ((x < 6) && (board[x + 1][y] == -color)) {
		for (int i = x + 2; i < 8; i++) {
			if (!board[i][y]) {
				break;
			}
			else if (board[i][y] == color) {
				directions.push_back(std::make_pair<int, int>(2, i * 8 + y));
				break;
			}
		}
	}
	//southeast
	if ((y > 1) && (x < 6) && (board[x + 1][y - 1] == -color)) {
		for (int i = 0; (x + i + 2 < 8) && (y - i - 2 >= 0); i++) {
			if (!board[x + i + 2][y - i - 2]) {
				break;
			}
			else if (board[x + i + 2][y - i - 2] == color) {
				directions.push_back(std::make_pair<int, int>(3, (x + i + 2) * 8 + (y - i - 2)));
				break;
			}
		}
	}
	//south
	if ((y > 1) && (board[x][y - 1] == -color)) {
		for (int i = y - 2; i >= 0; i--) {
			if (!board[x][i]) {
				break;
			}
			else if (board[x][i] == color) {
				directions.push_back(std::make_pair<int, int>(4, x * 8 + i));
				break;
			}
		}
	}
	//southwest
	if ((y > 1) && (x > 1) && (board[x - 1][y - 1] == -color)) {
		for (int i = 0; (x - i - 2 >= 0) && (y - i - 2 >= 0); i++) {
			if (!board[x - i - 2][y - i - 2]) {
				break;
			}
			else if (board[x - i - 2][y - i - 2] == color) {
				directions.push_back(std::make_pair<int, int>(5, (x - i - 2) * 8 + (y - i - 2)));
				break;
			}
		}
	}
	//west
	if ((x > 1) && (board[x - 1][y] == -color)) {
		for (int i = x - 2; i >= 0; i--) {
			if (!board[i][y]) {
				break;
			}
			else if (board[i][y] == color) {
				directions.push_back(std::make_pair<int, int>(6, i * 8 + y));
				break;
			}
		}
	}
	//northwest
	if ((y < 6) && (x > 1) && (board[x - 1][y + 1] == -color)) {
		for (int i = 0; (x - i - 2 >= 0) && (y + i + 2 < 8); i++) {
			if (!board[x - i - 2][y + i + 2]) {
				break;
			}
			else if (board[x - i - 2][y + i + 2] == color) {
				directions.push_back(std::make_pair<int, int>(7, (x - i - 2) * 8 + (y + i + 2)));
				break;
			}
		}
	}
	return directions;
}

std::vector< std::pair<int, std::vector< std::pair<int, int> > > > get_moves(int board[][8], int color) {
	std::vector< std::pair<int, std::vector< std::pair<int, int> > > > m;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			m.push_back(std::make_pair<int, std::vector< std::pair<int, int> > >(i * 8 + j, get_directions(board, i, j, color)));
			if (!m.back().second.size()) {
				m.pop_back();
			}
		}
	}
	return m;
}

void make_move(int board[][8], int x, int y, int color, std::vector< std::pair<int, int> > directions) {
	for (auto it = directions.begin(); it != directions.end(); it++) {
		int i = x;
		int j = y;
		while ((i != ((*it).second / 8)) || (j != ((*it).second & 7))) {
			board[i][j] = color;
			if (i < ((*it).second / 8)) {
				i++;
			}
			else if ((i >(*it).second / 8)) {
				i--;
			}
			if (j < ((*it).second & 7)) {
				j++;
			}
			else if (j >((*it).second & 7)) {
				j--;
			}
		}
	}
}

void undo_move(int board[][8], int x, int y, int color, std::vector< std::pair<int, int> > directions) {
	for (auto it = directions.begin(); it != directions.end(); it++) {
		int i = x;
		int j = y;
		while ((i != ((*it).second / 8)) || (j != ((*it).second & 7))) {
			board[i][j] = -color;
			if (i < ((*it).second / 8)) {
				i++;
			}
			else if ((i >(*it).second / 8)) {
				i--;
			}
			if (j < ((*it).second & 7)) {
				j++;
			}
			else if (j >((*it).second & 7)) {
				j--;
			}
		}
		board[i][j] = color;
	}
	board[x][y] = 0;
}

int score(int board[][8], int color) {
	int sum = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			sum += board[i][j];
		}
	}
	return sum * color;
}

int negamax_aux(int board[][8], int color, int depth, int alpha, int beta) {
	if (depth == 0) {
		return score(board, color);
	}
	std::vector< std::pair<int, std::vector< std::pair<int, int> > > > moves = get_moves(board, color);
	if (moves.size() == 0) {
		if (get_moves(board, -color).size() == 0) {
			return score(board, color);
		}
		int val = -negamax_aux(board, -color, depth - 1, -beta, -alpha);
		if (val >= beta) {
			return val;
		}
		if (val > alpha) {
			alpha = val;
		}
	}
	else {
		for (auto it = moves.begin(); it != moves.end(); it++) {
			make_move(board, (*it).first / 8, (*it).first & 7, color, (*it).second);
			int val = -negamax_aux(board, -color, depth - 1, -beta, -alpha);
			undo_move(board, (*it).first / 8, (*it).first & 7, color, (*it).second);
			if (val >= beta) {
				return val;
			}
			if (val > alpha) {
				alpha = val;
			}
		}
	}
	return alpha;
}

int negamax(int board[][8], int color, int depth) {
	int alpha = -65;
	int beta = 65;
	std::vector< std::pair<int, std::vector< std::pair<int, int> > > > moves = get_moves(board, color);
	int move = moves[0].first;
	for (auto it = moves.begin(); it != moves.end(); it++) {
		make_move(board, (*it).first / 8, (*it).first & 7, color, (*it).second);
		int val = -negamax_aux(board, -color, depth - 1, -beta, -alpha);
		undo_move(board, (*it).first / 8, (*it).first & 7, color, (*it).second);
		if (val >= beta) {
			return (*it).first;
		}
		if (val > alpha) {
			alpha = val;
			move = (*it).first;
		}
	}
	return move;
}

int input_ai_checkseet(int board[][8], int color)
{
	//置ける場所を検索
	std::vector<std::pair<int, std::vector<std::pair<int, int>>>> moves = get_moves(board, color);

	int move = moves[0].first;
	int current = -9999;

	for (auto it = moves.begin(); it != moves.end(); it++) {
		//ある場所に駒を置いた場合
		make_move(board, (*it).first / 8, (*it).first & 7, color, (*it).second);

		int cb = color_count(board, COLOR_BLACK);
		int cw = color_count(board, COLOR_WHITE);

		int val = check_seet[(*it).first & 7][(*it).first / 8];

		if (val > current) {
			current = val;
			move = (*it).first;
		}

		//盤面を戻す
		undo_move(board, (*it).first / 8, (*it).first & 7, color, (*it).second);
	}

	return move;
}

int input_ai_count_check(int board[][8], int color)
{
	//置ける場所を検索
	std::vector<std::pair<int, std::vector<std::pair<int, int>>>> moves = get_moves(board, color);

	int move = moves[0].first;
	int current = -9999;

	for (auto it = moves.begin(); it != moves.end(); it++) {
		//ある場所に駒を置いた場合
		make_move(board, (*it).first / 8, (*it).first & 7, color, (*it).second);

		int cb = color_count(board, COLOR_BLACK);
		int cw = color_count(board, COLOR_WHITE);

		int val = ((cw - cb)) + check_seet[(*it).first / 8][(*it).first & 7];

		//Println(L"x = ", (*it).first / 8, L" : y = ", (*it).first & 7, L" : val = ", val);

		if (val > current) {
			current = val;
			move = (*it).first;
		}

		//盤面を戻す
		undo_move(board, (*it).first / 8, (*it).first & 7, color, (*it).second);
	}

	return move;
}


int input_ai_count_check2(int board[][8], int color)
{
	//置ける場所を検索
	std::vector<std::pair<int, std::vector<std::pair<int, int>>>> moves = get_moves(board, color);

	int move = moves[0].first;
	int current = -9999;

	for (auto it = moves.begin(); it != moves.end(); it++) {
		//ある場所に駒を置いた場合
		make_move(board, (*it).first / 8, (*it).first & 7, color, (*it).second);

		int cb = color_count(board, COLOR_BLACK);
		int cw = color_count(board, COLOR_WHITE);

		int val = ((cw * 3 - cb * 3)) + check_seet[(*it).first / 8][(*it).first & 7];

		//Println(L"x = ", (*it).first / 8, L" : y = ", (*it).first & 7, L" : val = ", val);

		if (val > current) {
			current = val;
			move = (*it).first;
		}

		//盤面を戻す
		undo_move(board, (*it).first / 8, (*it).first & 7, color, (*it).second);
	}

	return move;
}



int color_count(int board[][8], int color)
{
	int count = 0;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j] == color) {
				count++;
			}
		}
	}

	return count;
}

void Game::Wing()
{

}

void Game::Stone()
{
	//左上
	if (board[0][0] == COLOR_WHITE) {
		check_seet[0][1] = 5;
		check_seet[1][0] = 5;
		check_seet[1][1] = 5;
	}
	//右上
	if (board[0][7] == COLOR_WHITE) {
		check_seet[0][6] = 5;
		check_seet[1][7] = 5;
		check_seet[1][6] = 5;
	}
	//左下
	if (board[7][0] == COLOR_WHITE) {
		check_seet[6][0] = 5;
		check_seet[7][1] = 5;
		check_seet[6][1] = 5;
	}
	//右下
	if (board[7][7] == COLOR_WHITE) {
		check_seet[6][7] = 5;
		check_seet[7][6] = 5;
		check_seet[6][6] = 5;
	}
}