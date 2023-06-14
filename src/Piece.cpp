#include "Piece.h"

#include <functional>

#include "Position.h"

#include "Util.h"


using namespace Mortan;

template<Direction dir>
BitBoard Mortan::RayMobilityWithBlockers(Square square, BitBoard board) {
	BitBoard mask = 0;
	mask |= eyeRays[dir][square];
	BitBoard blockers = eyeRays[dir][square] & board;
	if (blockers) {
		Square blocker;
		if constexpr (dir == North || dir == West || dir == NorthEast || dir == NorthWest) {
			blocker = WeakBit(blockers);
		} else if constexpr (dir == South || dir == East || dir == SouthEast || dir == SouthWest) {
			blocker = StrongBit(blockers);
		} else {
			abort();
		}

		mask &= ~eyeRays[dir][blocker];
	}

	return mask;
}

template BitBoard Mortan::RayMobilityWithBlockers<North>(Square square, BitBoard board);
template BitBoard Mortan::RayMobilityWithBlockers<South>(Square square, BitBoard board);
template BitBoard Mortan::RayMobilityWithBlockers<East>(Square square, BitBoard board);
template BitBoard Mortan::RayMobilityWithBlockers<West>(Square square, BitBoard board);
template BitBoard Mortan::RayMobilityWithBlockers<NorthEast>(Square square, BitBoard board);
template BitBoard Mortan::RayMobilityWithBlockers<NorthWest>(Square square, BitBoard board);
template BitBoard Mortan::RayMobilityWithBlockers<SouthEast>(Square square, BitBoard board);
template BitBoard Mortan::RayMobilityWithBlockers<SouthWest>(Square square, BitBoard board);

// TODO: pins & checks
BitBoard Mortan::PieceQuites(const Position &position, Square square) {
	Color color = ColorOf(position.bySquare[square]);
	PieceKind kind = KindOf(position.bySquare[square]);

	if (kind != King && position.oppInDoubleCheck && color == position.opp) {
		return 0;
	}

	BitBoard board = position.board;

	switch (kind) {
	case King:
	{
		BitBoard eyes = kingEyes[square] & ~board;
		BitBoard mask = 0;
		Square sq;
		while (sq = PopWeak(&eyes)) {
			if (position.Preassure(sq, color) == 0) {
				mask |= BitAt(sq);
			}
		}

		return mask;
	}
	case Queen:
		return (RayMobilityWithBlockers<North>(square, board) |
			RayMobilityWithBlockers<South>(square, board) |
			RayMobilityWithBlockers<East>(square, board) |
			RayMobilityWithBlockers<West>(square, board) |
			RayMobilityWithBlockers<NorthEast>(square, board) |
			RayMobilityWithBlockers<NorthWest>(square, board) |
			RayMobilityWithBlockers<SouthEast>(square, board) |
			RayMobilityWithBlockers<SouthWest>(square, board))
			& ~board;
	case Rook:
		return (RayMobilityWithBlockers<North>(square, board) |
			RayMobilityWithBlockers<South>(square, board) |
			RayMobilityWithBlockers<East>(square, board) |
			RayMobilityWithBlockers<West>(square, board))
			& ~board;
	case Bishop:
		return (RayMobilityWithBlockers<NorthEast>(square, board) |
			RayMobilityWithBlockers<NorthWest>(square, board) |
			RayMobilityWithBlockers<SouthEast>(square, board) |
			RayMobilityWithBlockers<SouthWest>(square, board))
			& ~board;
	case Knight:
		return knightEyes[square] & ~board;
	case Pawn:
	{
		BitBoard mask = 0;

		BitBoard homeMask = color == White ? 0x000000000000FF00 : 0x00FF000000000000;
		int delta = color == White ? 8 : -8;

		mask |= BitAt(square + delta);
		if (!(mask & board) && BitAt(square) & homeMask) {
			mask |= BitAt(square + delta + delta);
		}

		return mask & ~board;
	}

	default:
		abort();
	}
}

BitBoard Mortan::PieceAttacks(const Position &position, Square square) {
	Color color = ColorOf(position.bySquare[square]);
	PieceKind kind = KindOf(position.bySquare[square]);

	BitBoard board = position.board;
	BitBoard enemy = position.byColor[!color];

	switch (kind) {
	case King:
	{
		BitBoard eyes = kingEyes[square] & enemy;
		BitBoard mask = 0;
		Square sq;
		while (sq = PopWeak(&eyes)) {
			if (position.Preassure(sq, color) == 0) {
				mask |= BitAt(sq);
			}
		}

		return mask;
	}
	case Queen:
		return (RayMobilityWithBlockers<North>(square, board) |
			RayMobilityWithBlockers<South>(square, board) |
			RayMobilityWithBlockers<East>(square, board) |
			RayMobilityWithBlockers<West>(square, board) |
			RayMobilityWithBlockers<NorthEast>(square, board) |
			RayMobilityWithBlockers<NorthWest>(square, board) |
			RayMobilityWithBlockers<SouthEast>(square, board) |
			RayMobilityWithBlockers<SouthWest>(square, board))
			& enemy;
	case Rook:
		return (RayMobilityWithBlockers<North>(square, board) |
			RayMobilityWithBlockers<South>(square, board) |
			RayMobilityWithBlockers<East>(square, board) |
			RayMobilityWithBlockers<West>(square, board))
			& enemy;
	case Bishop:
		return (RayMobilityWithBlockers<NorthEast>(square, board) |
			RayMobilityWithBlockers<NorthWest>(square, board) |
			RayMobilityWithBlockers<SouthEast>(square, board) |
			RayMobilityWithBlockers<SouthWest>(square, board))
			& enemy;
	case Knight:
		return knightEyes[square] & enemy;
	case Pawn:
	{
		int delta = color == White ? 8 : -8;
		return (pawnEyes[square % 8] << (square - square % 8 + delta)) & (enemy | BitAt(position.passant));
	}

	default:
		abort();
	}
}