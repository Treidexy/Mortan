#include "Position.h"

#include <memory>

#include "Piece.h"
#include "Util.h"

using namespace Mortan;

Position Position::Default() {
	Position position = {};
	position.passant = NoFile;
	Piece bySquare[SquareCount] = {
		WRook, WKnight, WBishop, WQueen, WKing, WBishop, WKnight, WRook,
		WPawn, WPawn, WPawn, WPawn, WPawn, WPawn, WPawn, WPawn,
		PieceNone, PieceNone, PieceNone, PieceNone, PieceNone, PieceNone, PieceNone, PieceNone,
		PieceNone, PieceNone, PieceNone, PieceNone, PieceNone, PieceNone, PieceNone, PieceNone,
		PieceNone, PieceNone, PieceNone, PieceNone, PieceNone, PieceNone, PieceNone, PieceNone,
		PieceNone, PieceNone, PieceNone, PieceNone, PieceNone, PieceNone, PieceNone, PieceNone,
		BPawn, BPawn, BPawn, BPawn, BPawn, BPawn, BPawn, BPawn,
		BRook, BKnight, BBishop, BQueen, BKing, BBishop, BKnight, BRook
	};
	
	std::memcpy(position.physicalBoard.bySquare, bySquare, sizeof(bySquare));
	position.opp = White;

	position.Recalc();

	return position;
}

void Position::Recalc() {
	physicalBoard.byColor[White] = 0;
	physicalBoard.byColor[Black] = 0;
	memset(physicalBoard.byKind, 0, sizeof(physicalBoard.byKind));

	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			Piece piece = physicalBoard.bySquare[x + y * 8];
			if (piece != PieceNone) {
				physicalBoard.byColor[PieceColor(piece)] |= BitAt(x + y * 8);

				PieceKind kind = KindOf(piece);
				physicalBoard.byKind[kind] |= BitAt(x + y * 8);
			}
		}
	}
}

void Position::DoMove(Square from, Square to) {
	physicalBoard.bySquare[to] = physicalBoard.bySquare[from];
	physicalBoard.bySquare[from] = PieceNone;
	Recalc();
}