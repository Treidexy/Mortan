#include "Piece.h"

#include <stdlib.h>

#include "Position.h"
#include "Util.h"

using namespace Mortan;

namespace {
	BitBoard KingMobility(Square square, BitBoard ally) {
		BitBoard mask = kingEyes[square];
		return mask & ~ally;
	}

	BitBoard RookMobility(Square square, BitBoard ally, BitBoard enemy) {
		return eyeRays[North][square] | eyeRays[South][square] | eyeRays[East][square] | eyeRays[West][square];
	}

	BitBoard BishopMobility(Square square, BitBoard ally, BitBoard enemy) {
		return eyeRays[NorthEast][square] | eyeRays[NorthWest][square] | eyeRays[SouthEast][square] | eyeRays[SouthWest][square];
	}

	BitBoard QueenMobility(Square square, BitBoard ally, BitBoard enemy) {
		return RookMobility(square, ally, enemy) | BishopMobility(square, ally, enemy);
	}

	BitBoard KnightMobility(Square square, BitBoard ally) {
		BitBoard mask = knightEyes[square];
		return mask & ~ally;
	}
	BitBoard PawnMobility(Square, BitBoard ally, BitBoard enemy, File passant) { return 0; }
}

BitBoard Mortan::PieceMobility(PieceKind kind, Square square, BitBoard ally, BitBoard enemy, File passant) {
	switch (kind) {
	case King:
		return KingMobility(square, ally);
	case Queen:
		return QueenMobility(square, ally, enemy);
	case Rook:
		return RookMobility(square, ally, enemy);
	case Bishop:
		return BishopMobility(square, ally, enemy);
	case Knight:
		return KnightMobility(square, ally);
	case Pawn:
		return PawnMobility(square, ally, enemy, passant);

	default:
		abort();
	}
}