#include "Replay.h"
#include "Management.h"
#include "Result.h"

Replay::Replay()
{
	Graphics::SetBackground(Palette::Lightgreen);
}

Replay::~Replay()
{
}

void Replay::Next()
{
	if (step == e_next) {
		//Println(L"�Ֆʃ`�F�b�N�J�n");

		//�u����ꏊ�𒲂ׂ�
		moves = get_moves(board, turn);

		//�u����ꏊ���Ȃ��ꍇ
		if (moves.size() == 0) {
			//��Ԃ����
			turn = -turn;
			//�u����ꏊ�����߂�
			moves = get_moves(board, turn);
			//���Ғu����ꏊ���Ȃ��ꍇ�Q�[���I��
			if (moves.size() == 0) {
				/*
				Println(L"�Q�[���I��");
				Println(L"�^�[����:", turn_count);

				std::string str = "black : " + std::to_string(color_count(board, COLOR_BLACK)) + "  vs  white : " + std::to_string(color_count(board, COLOR_WHITE));
				Println(Widen(str));
				*/
			}
		}

		moves = get_moves(board, turn);
		step = e_inputWait;

		for (auto it = moves.begin(); it != moves.end(); it++) {
			//Println(L"x = ", (*it).first / 8, L"  y = ", (*it).first % 8);
		}

		turn_count++;
		//Println(L"�Ֆʃ`�F�b�N����");
		System::Sleep(500);
	}
}

void Replay::Enter()
{
	log.ReadFile("reversi_log.txt");

	for (int i = 0; i < log.get_LogSize(); i++) {
		std::pair<int, int> input_pos = log.get_log(i);

		//Println(L"y = ", input_pos.first, L" : x = ", input_pos.second, L"\n");
	}

	step = e_next;

	turn = -1;
	turn_count = 0;
	memset(board, 0, sizeof(board));
	board[3][3] = board[4][4] = 1;
	board[3][4] = board[4][3] = -1;

	replay_count = 0;
	frame = 0;

	font_text = Font(20);
}

void Replay::Exit()
{

}

void Replay::Update()
{
	Next();

	if (frame > 10) {

		if (replay_count < log.get_LogSize()) {
			std::pair<int, int>pos = log.get_log(replay_count);

			int y = pos.first, x = pos.second;

			if (step == e_inputWait) {
				step = e_inputCheck;
			}
			//���͂��ꂽ�l�𔽉f������
			if (step == e_inputCheck) {
				for (auto it = moves.begin(); it != moves.end(); it++) {
					//���͂���������Ύ�Ԃ����
					if (x * 8 + y == ((*it).first)) {
						make_move(board, x, y, turn, (*it).second);
						turn = -turn;
						step = e_next;
						break;
					}
				}
			}
		}
		else {
			Management::SceneChange(new Result(color_count(board, COLOR_BLACK), color_count(board, COLOR_WHITE)));
		}

		replay_count++;
		frame = 0;
	}
	
	
	frame++;
}

void Replay::Draw()
{
	const int start_y = 100;
	const int start_x = 320 - ((30 + 3) * 8) / 2;
	const int grid_size = 30;
	const int space_size = 3;
	const int radius = 13;

	if (turn < 0) {
		font_text(L"���̃^�[��").drawCenter(320, 40, Palette::Black);
	}
	else if (turn > 0) {
		font_text(L"���̃^�[��").drawCenter(320, 40, Palette::White);
	}

	Point pos;
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			//�{�[�h��\��
			pos.x = start_x + (grid_size + space_size) * x;
			pos.y = start_y + (grid_size + space_size) * y;
			Rect rect = Rect(pos.x, pos.y, grid_size, grid_size);
			rect.draw(Palette::Green);

			//�}�E�X����ɂ�������g��\��
			if (rect.mouseOver) {
				rect.drawFrame(1.0, 0.0, Palette::Yellow);
			}

			//�������̐΂�\��
			if (board[y][x] > 0) {
				Circle(pos.x + grid_size / 2, pos.y + grid_size / 2, radius).draw(Palette::White);
			}
			if (board[y][x] < 0) {
				Circle(pos.x + grid_size / 2, pos.y + grid_size / 2, radius).draw(Palette::Black);
			}

			//�u����ꏊ��\��
			for (auto it = moves.begin(); it != moves.end(); it++) {
				if ((*it).first / 8 == y && (*it).first % 8 == x) {
					Circle(pos.x + grid_size / 2, pos.y + grid_size / 2, radius / 4).draw(Palette::Yellow);
				}
			}
		}
	}
}