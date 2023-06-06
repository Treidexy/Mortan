#include <stdio.h>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "Position.h"
#include "Piece.h"
#include "Util.h"

using namespace Mortan;

struct Arrow {
	Square from;
	Square to;

	bool operator ==(const Arrow &other) const {
		return from == other.from && to == other.to;
	}
};

struct MortanApp : public olc::PixelGameEngine {
	Position position;
	Square selSquare = SquareNone;
	Piece selPiece = PieceNone;
	BitBoard moves = 0;

	BitBoard noted = 0;
	Square arrowSelSquare = SquareNone;
	std::vector<Arrow> arrows;

	olc::Sprite *sprites[PieceCount];
	olc::Decal *decals[PieceCount];

	MortanApp() {
		sAppName = "Martin";
	}

	bool OnUserCreate() override {
		position = Position::Default();

		decals[WKing] = new olc::Decal(sprites[WKing] = new olc::Sprite("res/w_king.png"));
		decals[WQueen] = new olc::Decal(sprites[WQueen] = new olc::Sprite("res/w_queen.png"));
		decals[WRook] = new olc::Decal(sprites[WRook] = new olc::Sprite("res/w_rook.png"));
		decals[WBishop] = new olc::Decal(sprites[WBishop] = new olc::Sprite("res/w_bishop.png"));
		decals[WKnight] = new olc::Decal(sprites[WKnight] = new olc::Sprite("res/w_knight.png"));
		decals[WPawn] = new olc::Decal(sprites[WPawn] = new olc::Sprite("res/w_pawn.png"));

		decals[BKing] = new olc::Decal(sprites[BKing] = new olc::Sprite("res/b_king.png"));
		decals[BQueen] = new olc::Decal(sprites[BQueen] = new olc::Sprite("res/b_queen.png"));
		decals[BRook] = new olc::Decal(sprites[BRook] = new olc::Sprite("res/b_rook.png"));
		decals[BBishop] = new olc::Decal(sprites[BBishop] = new olc::Sprite("res/b_bishop.png"));
		decals[BKnight] = new olc::Decal(sprites[BKnight] = new olc::Sprite("res/b_knight.png"));
		decals[BPawn] = new olc::Decal(sprites[BPawn] = new olc::Sprite("res/b_pawn.png"));

		return true;
	}

	Square MouseSquare() {
		return (Square) (GetMouseX() / 90 + (7 - GetMouseY() / 90) * 8);
	}

	void SelPiece() {
		selSquare = MouseSquare();
		selPiece = position.physicalBoard.bySquare[selSquare];
		if (selPiece == PieceNone) {
			selSquare = SquareNone;
			selPiece = PieceNone;
			moves = 0;
		} else {
			Color col = PieceColor(selPiece);
			moves = PieceMobility(KindOf(selPiece), selSquare, position.physicalBoard.byColor[col], position.physicalBoard.byColor[!col], NoFile);
		}
	}

	void MoveSelPiece() {
		Square fromSquare = selSquare;
		selSquare = MouseSquare();
		if (fromSquare != selSquare) {
			position.DoMove(fromSquare, selSquare);
		}

		selSquare = SquareNone;
		selPiece = PieceNone;
		moves = 0;
	}

	bool OnUserUpdate(float fElapsedTime) override {
		if (GetMouse(0).bPressed) {
			noted = 0;
			arrows.clear();

			SelPiece();
		}
		if (GetMouse(0).bReleased) {
			if (selPiece != PieceNone) {
				MoveSelPiece();
			}
		}

		if (GetMouse(1).bPressed) {
			arrowSelSquare = MouseSquare();
		}
		if (GetMouse(1).bReleased) {
			Square toSquare = MouseSquare();
			if (arrowSelSquare == toSquare) {
				noted ^= BitAt(arrowSelSquare);
			} else {
				Arrow arrow = {arrowSelSquare, toSquare};
				auto it = std::find(arrows.cbegin(), arrows.cend(), arrow);
				if (it != arrows.cend()) {
					arrows.erase(it);
				} else {
					arrows.push_back(arrow);
				}
			}

			arrowSelSquare = SquareNone;
		}

		Clear(olc::Pixel(0, 100, 0));

		for (int y = 0; y < 8; y++) {
			for (int x = 0; x < 8; x++) {
				if (noted & BitAt(x + y * 8)) {
					FillRectDecal({x * 90.0f, (7 - y) * 90.0f}, {90.0f, 90.0f}, olc::RED);
				}else if ((x + y) % 2 == 1) {
					FillRectDecal({x * 90.0f, (7 - y) * 90.0f}, {90.0f, 90.0f});
				}

				if (moves & BitAt(x + y * 8)) {
					FillRectDecal({x * 90.0f, (7 - y) * 90.0f}, {90.0f, 90.0f}, olc::GREEN);
				}
			}
		}

		FillRectDecal({(selSquare % 8) * 90.0f, (7 - selSquare / 8) * 90.0f}, {90.0f, 90.0f}, olc::YELLOW);

		for (int y = 0; y < 8; y++) {
			for (int x = 0; x < 8; x++) {
				Piece piece = position.physicalBoard.bySquare[x + y * 8];
				if (piece != PieceNone) {
					DrawDecal({x * 90.0f, (7 - y) * 90.0f}, decals[piece]);
				}
			}
		}

		DrawArrows();

		return true;
	}

	void DrawArrows() {
		const auto fn2 = [&](olc::vf2d from, olc::vf2d to) {
			float d = 3.0f;

			const auto fn = [&](float a, float b) {
				DrawLineDecal(from, to + olc::vf2d{a, b}, olc::DARK_YELLOW);
			};

			fn(-d, -d);
			fn(-d, 0);
			fn(-d, +d);

			fn(0, -d);
			fn(0, 0);
			fn(0, +d);

			fn(+d, -d);
			fn(+d, 0);
			fn(+d, +d);
		};

		for (const auto &arrow : arrows) {
			olc::vf2d from = {(arrow.from % 8 + 0.5f) * 90.0f, (7 - arrow.from / 8 + 0.5f) * 90.0f};
			olc::vf2d to = {(arrow.to % 8 + 0.5f) * 90.0f, (7 - arrow.to / 8 + 0.5f) * 90.0f};
			fn2(from, to);
		}

		if (GetMouse(1).bHeld) {
			Square hover = MouseSquare();
			if (arrowSelSquare != hover) {
				olc::vf2d from = {(arrowSelSquare % 8 + 0.5f) * 90.0f, (7 - arrowSelSquare / 8 + 0.5f) * 90.0f};
				olc::vf2d to = {(hover % 8 + 0.5f) * 90.0f, (7 - hover / 8 + 0.5f) * 90.0f};
				fn2(from, to);
			}
		}
	}

	bool OnUserDestroy() override {
		for (int i = 0; i < PieceCount; i++) {
			delete sprites[i];
			delete decals[i];
		}
		return true;
	}
};

int main() {
	InitEyes();

	MortanApp game;
	if (game.Construct(720, 720, 1, 1)) {
		game.Start();
	}
	return 0;
}
