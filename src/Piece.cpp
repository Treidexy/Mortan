#include "Piece.h"

#include <iostream>

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

namespace {
	BitBoard KingQuites(const Position &position, Square square, Color color) {
		BitBoard eyes = kingEyes[square] & ~position.board;
		BitBoard mask = 0;
		Square sq;
		while (eyes) {
			sq = PopWeak(&eyes);

			if (position.Preassure(sq, color) == 0) {
				mask |= BitAt(sq);
			}
		}

		static constexpr BitBoard kingSide[ColorCount] = {
			0x0000000000000060,
			0x6000000000000000,
		};

		static constexpr BitBoard queenSide[ColorCount] = {
			0x000000000000000E,
			0x0E00000000000000,
		};

		// do not need to do bounds checking bc king cannot move if castlingRights
		if (position.castlingRights[color] & Castling::KingSide) {
			if (!(kingSide[color] & position.board || position.Preassure(Square(square + 1), color) || position.Preassure(Square(square + 2), color))) {
				mask |= BitAt(square + 2);
			}
		}
		if (position.castlingRights[color] & Castling::QueenSide) {
			if (!(queenSide[color] & position.board || position.Preassure(Square(square - 1), color) || position.Preassure(Square(square - 2), color))) {
				mask |= BitAt(square - 2);
			}
		}

		return mask;
	}

	BitBoard KingAttacks(const Position &position, Square square, Color color, BitBoard enemy) {
		BitBoard eyes = kingEyes[square] & enemy;
		BitBoard mask = 0;
		Square sq;
		while (eyes) {
			sq = PopWeak(&eyes);

			if (position.Preassure(sq, color) == 0) {
				mask |= BitAt(sq);
			}
		}

		return mask & enemy;
	}
}

// TODO: pins & checks
BitBoard Mortan::PieceQuites(const Position &position, Square square) {
	Color color = ColorOf(position.bySquare[square]);
	PieceKind kind = KindOf(position.bySquare[square]);

	if (kind == King) {
		return KingQuites(position, square, color);
	}

	if (position.oppInDoubleCheck && color == position.opp) {
		return 0;
	}

	BitBoard ally = position.byColor[color];
	Square king = WeakBit(position.byKind[King] & ally);
	BitBoard board = position.board;
	BitBoard thing = position.checkPath;
	if (color != position.opp || !position.oppInCheck) {
		thing = ~0ull;
	}

	BitBoard lateralEnemies = (position.byKind[Rook] | position.byKind[Queen]) & position.byColor[!color];
	BitBoard diagonalEnemies = (position.byKind[Bishop] | position.byKind[Queen]) & position.byColor[!color];

	BitBoard pinPath = 0;
	bool isPinned = false;
	auto fn1 = [&](BitBoard bb) {
		if (bb & BitAt(square) && bb & lateralEnemies && Count(bb & ally) == 1) {
			isPinned = true;
			pinPath |= bb;
		}
	};
	auto fn2 = [&](BitBoard bb) {
		if (bb & BitAt(square) && bb & diagonalEnemies && Count(bb & ally) == 1) {
			isPinned = true;
			pinPath |= bb;
		}
	};

	fn1(RayMobilityWithBlockers<North>(king, lateralEnemies));
	fn1(RayMobilityWithBlockers<South>(king, lateralEnemies));
	fn1(RayMobilityWithBlockers<East>(king, lateralEnemies));
	fn1(RayMobilityWithBlockers<West>(king, lateralEnemies));

	fn2(RayMobilityWithBlockers<NorthEast>(king, diagonalEnemies));
	fn2(RayMobilityWithBlockers<NorthWest>(king, diagonalEnemies));
	fn2(RayMobilityWithBlockers<SouthEast>(king, diagonalEnemies));
	fn2(RayMobilityWithBlockers<SouthWest>(king, diagonalEnemies));

	if (!isPinned) {
		pinPath = ~0ull;
	}

	switch (kind) {
	case Queen:
		return (RayMobilityWithBlockers<North>(square, board) |
			RayMobilityWithBlockers<South>(square, board) |
			RayMobilityWithBlockers<East>(square, board) |
			RayMobilityWithBlockers<West>(square, board) |
			RayMobilityWithBlockers<NorthEast>(square, board) |
			RayMobilityWithBlockers<NorthWest>(square, board) |
			RayMobilityWithBlockers<SouthEast>(square, board) |
			RayMobilityWithBlockers<SouthWest>(square, board))
			& ~board & thing & pinPath;
	case Rook:
		return (RayMobilityWithBlockers<North>(square, board) |
			RayMobilityWithBlockers<South>(square, board) |
			RayMobilityWithBlockers<East>(square, board) |
			RayMobilityWithBlockers<West>(square, board))
			& ~board & thing & pinPath;
	case Bishop:
		return (RayMobilityWithBlockers<NorthEast>(square, board) |
			RayMobilityWithBlockers<NorthWest>(square, board) |
			RayMobilityWithBlockers<SouthEast>(square, board) |
			RayMobilityWithBlockers<SouthWest>(square, board))
			& ~board & thing & pinPath;
	case Knight:
		return knightEyes[square] & ~board & thing & pinPath;
	case Pawn:
	{
		BitBoard mask = 0;

		BitBoard homeMask = color == White ? 0x000000000000FF00 : 0x00FF000000000000;
		int delta = PawnForward(color);

		mask |= BitAt(square + delta);
		if (!(mask & board) && BitAt(square) & homeMask) {
			mask |= BitAt(square + delta + delta);
		}

		return mask & ~board & thing & pinPath;
	}

	default:
		abort();
	}
}

BitBoard Mortan::PieceAttacks(const Position &position, Square square) {
	Color color = ColorOf(position.bySquare[square]);
	PieceKind kind = KindOf(position.bySquare[square]);
	BitBoard enemy = position.byColor[!color];

	if (kind == King) {
		return KingAttacks(position, square, color, enemy);
	}

	if (position.oppInDoubleCheck && color == position.opp) {
		return 0;
	}

	BitBoard ally = position.byColor[color];
	Square king = WeakBit(position.byKind[King] & ally);
	BitBoard board = position.board;
	BitBoard thing = position.checkPath;
	if (color != position.opp || !position.oppInCheck) {
		thing = ~0ull;
	}

	BitBoard lateralEnemies = (position.byKind[Rook] | position.byKind[Queen]) & position.byColor[!color];
	BitBoard diagonalEnemies = (position.byKind[Bishop] | position.byKind[Queen]) & position.byColor[!color];

	BitBoard pinPath = 0;
	bool isPinned = false;
	auto fn1 = [&](BitBoard bb) {
		if (bb & BitAt(square) && bb & lateralEnemies && Count(bb & ally) == 1) {
			isPinned = true;
			pinPath |= bb;
		}
	};
	auto fn2 = [&](BitBoard bb) {
		if (bb & BitAt(square) && bb & diagonalEnemies && Count(bb & ally) == 1) {
			isPinned = true;
			pinPath |= bb;
		}
	};

	fn1(RayMobilityWithBlockers<North>(king, lateralEnemies));
	fn1(RayMobilityWithBlockers<South>(king, lateralEnemies));
	fn1(RayMobilityWithBlockers<East>(king, lateralEnemies));
	fn1(RayMobilityWithBlockers<West>(king, lateralEnemies));

	fn2(RayMobilityWithBlockers<NorthEast>(king, diagonalEnemies));
	fn2(RayMobilityWithBlockers<NorthWest>(king, diagonalEnemies));
	fn2(RayMobilityWithBlockers<SouthEast>(king, diagonalEnemies));
	fn2(RayMobilityWithBlockers<SouthWest>(king, diagonalEnemies));

	if (!isPinned) {
		pinPath = ~0ull;
	}

	switch (kind) {
	case Queen:
		return (RayMobilityWithBlockers<North>(square, board) |
			RayMobilityWithBlockers<South>(square, board) |
			RayMobilityWithBlockers<East>(square, board) |
			RayMobilityWithBlockers<West>(square, board) |
			RayMobilityWithBlockers<NorthEast>(square, board) |
			RayMobilityWithBlockers<NorthWest>(square, board) |
			RayMobilityWithBlockers<SouthEast>(square, board) |
			RayMobilityWithBlockers<SouthWest>(square, board))
			& enemy & thing & pinPath;
	case Rook:
		return (RayMobilityWithBlockers<North>(square, board) |
			RayMobilityWithBlockers<South>(square, board) |
			RayMobilityWithBlockers<East>(square, board) |
			RayMobilityWithBlockers<West>(square, board))
			& enemy & thing & pinPath;
	case Bishop:
		return (RayMobilityWithBlockers<NorthEast>(square, board) |
			RayMobilityWithBlockers<NorthWest>(square, board) |
			RayMobilityWithBlockers<SouthEast>(square, board) |
			RayMobilityWithBlockers<SouthWest>(square, board))
			& enemy & thing & pinPath;
	case Knight:
		return knightEyes[square] & enemy & thing & pinPath;
	case Pawn:
		return (pawnEyes[square % 8] << (square - square % 8 + PawnForward(color)))
			& (enemy | BitAt(position.passant)) & thing & pinPath;

	default:
		abort();
	}
}