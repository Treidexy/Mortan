#include "bots/WeakBot.h"

#include "Position.h"
#include "Piece.h"
#include "Util.h"

#include <iostream>

using namespace Mortan;

Ply WeakBot::MakeMove(const Position &position) {
	BitBoard pieces = position.byColor[position.opp];

	while (pieces) {
		Square square = PopWeak(&pieces);

		BitBoard bb = PieceAttacks(position, square);
		if (bb) {
			return {square, WeakBit(bb), Queen};
		}

		bb = PieceQuites(position, square);
		if (bb) {
			return {square, WeakBit(bb), Queen};
		}
	}

	return Ply::None;
}