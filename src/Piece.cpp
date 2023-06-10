#include "Piece.h"

#include "Position.h"

using namespace Mortan;

template<Direction dir>
BitBoard RayMobilityWithBlockers(Square square, BitBoard pieces) {
	BitBoard mask = 0;
	mask |= eyeRays[dir][square];
	BitBoard blockers = eyeRays[dir][square] & pieces;
	if (blockers) {
		Square blocker;
		if constexpr (dir == North || dir == West || dir == NorthEast || dir == NorthWest) {
			blocker = weakBit(blockers);
		} else if constexpr (dir == South || dir == East || dir == SouthEast || dir == SouthWest) {
			blocker = strongBit(blockers);
		} else {
			abort();
		}

		mask &= ~eyeRays[dir][blocker];
	}

	return mask;
}

template<>
BitBoard RayMobilityWithBlockers<North>(Square square, BitBoard pieces);
template<>
BitBoard RayMobilityWithBlockers<South>(Square square, BitBoard pieces);
template<>
BitBoard RayMobilityWithBlockers<East>(Square square, BitBoard pieces);
template<>
BitBoard RayMobilityWithBlockers<West>(Square square, BitBoard pieces);
template<>
BitBoard RayMobilityWithBlockers<NorthEast>(Square square, BitBoard pieces);
template<>
BitBoard RayMobilityWithBlockers<NorthWest>(Square square, BitBoard pieces);
template<>
BitBoard RayMobilityWithBlockers<SouthEast>(Square square, BitBoard pieces);
template<>
BitBoard RayMobilityWithBlockers<SouthWest>(Square square, BitBoard pieces);