#include "Piece.h"

#include <stdlib.h>

#include "Position.h"
#include "Util.h"

using namespace Mortan;

namespace {
	BitBoard KingMobility(Square square, BitBoard ally) {
		return kingEyes[square];
	}

	template<Direction dir>
	BitBoard RayMobilityWithBlockers(Square square, BitBoard pieces) {
		BitBoard mask = 0;
		mask |= eyeRays[dir][square];
		BitBoard blockers = eyeRays[dir][square] & pieces;
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

	BitBoard RookMobility(Square square, BitBoard ally, BitBoard enemy) {
		return RayMobilityWithBlockers<North>(square, ally | enemy) |
			RayMobilityWithBlockers<South>(square, ally | enemy) |
			RayMobilityWithBlockers<East>(square, ally | enemy) |
			RayMobilityWithBlockers<West>(square, ally | enemy);
	}

	BitBoard BishopMobility(Square square, BitBoard ally, BitBoard enemy) {
		return RayMobilityWithBlockers<NorthEast>(square, ally | enemy) |
			RayMobilityWithBlockers<NorthWest>(square, ally | enemy) |
			RayMobilityWithBlockers<SouthEast>(square, ally | enemy) |
			RayMobilityWithBlockers<SouthWest>(square, ally | enemy);
	}

	BitBoard QueenMobility(Square square, BitBoard ally, BitBoard enemy) {
		return RookMobility(square, ally, enemy) | BishopMobility(square, ally, enemy);
	}

	BitBoard KnightMobility(Square square, BitBoard ally) {
		return knightEyes[square];
	}

	BitBoard PawnMobility(Square, BitBoard ally, BitBoard enemy, File passant, Rank passantRank) {
		return 0;
	}
}