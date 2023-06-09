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
	memset(physicalBoard.byColor, 0, sizeof(physicalBoard.byColor));
	memset(physicalBoard.byKind, 0, sizeof(physicalBoard.byKind));
	memset(royaltyBoard.byColor, 0, sizeof(royaltyBoard.byColor));
	memset(mobilityBoard.byColor, 0, sizeof(mobilityBoard.byColor));

	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			Piece piece = physicalBoard.bySquare[x + y * 8];
			if (piece != PieceNone) {
				Color color = PieceColor(piece);
				physicalBoard.byColor[color] |= BitAt(x + y * 8);

				PieceKind kind = KindOf(piece);
				physicalBoard.byKind[kind] |= BitAt(x + y * 8);

				// if we wanna add royaltyness we needa plan it lol
				if (kind == King) {
					royaltyBoard.byColor[color] = (Square) (x + y * 8);
				}
			}
		}
	}

	// mobility stuff and whatnot
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			Piece piece = physicalBoard.bySquare[x + y * 8];
			PieceKind kind = KindOf(piece);
			if (piece != PieceNone) {
				Color color = PieceColor(piece);
				mobilityBoard.byColor[color] |= PieceMobility(kind, (Square) (x + y * 8), physicalBoard.byColor[color], physicalBoard.byColor[!color], passant);
			}
		}
	}
}

void Position::DoPly(Ply ply) {
	physicalBoard.bySquare[ply.to] = physicalBoard.bySquare[ply.from];
	physicalBoard.bySquare[ply.from] = PieceNone;
	Recalc();

	currMove.byColor[opp] = ply;

	if (opp == White) {
		opp = Black;
	} else {
		opp = White;
		moves.push_back(currMove);
		currMove = {}; // unnecessary?
	}
}