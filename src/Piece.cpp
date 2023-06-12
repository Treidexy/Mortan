#include "Piece.h"

#include "Position.h"

#include "Util.h"

using namespace Mortan;

template<Direction dir>
BitBoard Mortan::RayMobilityWithBlockers(Square square, BitBoard pieces) {
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

template BitBoard Mortan::RayMobilityWithBlockers<North>(Square square, BitBoard pieces);
template BitBoard Mortan::RayMobilityWithBlockers<South>(Square square, BitBoard pieces);
template BitBoard Mortan::RayMobilityWithBlockers<East>(Square square, BitBoard pieces);
template BitBoard Mortan::RayMobilityWithBlockers<West>(Square square, BitBoard pieces);
template BitBoard Mortan::RayMobilityWithBlockers<NorthEast>(Square square, BitBoard pieces);
template BitBoard Mortan::RayMobilityWithBlockers<NorthWest>(Square square, BitBoard pieces);
template BitBoard Mortan::RayMobilityWithBlockers<SouthEast>(Square square, BitBoard pieces);
template BitBoard Mortan::RayMobilityWithBlockers<SouthWest>(Square square, BitBoard pieces);