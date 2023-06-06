#include <stdio.h>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "Position.h"
#include "Piece.h"
#include "Util.h"

using namespace Mortan;

struct MortanApp : public olc::PixelGameEngine {
	Position position;
	Square selSquare = SquareNone;
	Piece selPiece = PieceNone;
	BitBoard moves = 0;

	olc::Sprite *sprites[PieceCount];
	olc::Decal *decals[PieceCount];

	MortanApp() {
		sAppName = "Hello, World!";
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
			SelPiece();
		}
		if (GetMouse(0).bReleased) {
			if (selPiece != PieceNone) {
				MoveSelPiece();
			}
		}

		Clear(olc::Pixel(0, 100, 0));

		for (int y = 0; y < 8; y++) {
			for (int x = 0; x < 8; x++) {
				if ((x + y) % 2 == 1) {
					FillRectDecal({x * 90.0f, (7 - y) * 90.0f}, {90.0f, 90.0f}, olc::Pixel(255, 255, 255));
				}

				if (moves & BitAt(x + y * 8)) {
					FillRectDecal({x * 90.0f, (7 - y) * 90.0f}, {90.0f, 90.0f}, olc::Pixel(0, 255, 0));
				}
			}
		}

		FillRectDecal({(selSquare % 8) * 90.0f, (7 - selSquare / 8) * 90.0f}, {90.0f, 90.0f}, olc::Pixel(255, 0, 0));

		for (int y = 0; y < 8; y++) {
			for (int x = 0; x < 8; x++) {
				Piece piece = position.physicalBoard.bySquare[x + y * 8];
				if (piece != PieceNone) {
					DrawDecal({x * 90.0f, (7 - y) * 90.0f}, decals[piece]);
				}
			}
		}

		return true;
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
