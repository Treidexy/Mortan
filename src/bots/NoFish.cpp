#include "bots/NoFish.h"

#include "Position.h"
#include "Piece.h"
#include "Util.h"

#include <iostream>

using namespace Mortan;

Ply NoFish::MakeMove(const Position &position) {
	BitBoard pieces = position.byColor[position.opp];

	while (pieces) {
		Square square = RandSquare(pieces);
		pieces &= ~BitAt(square);

		BitBoard bb = PieceAttacks(position, square);
		if (bb) {
			return {square, RandSquare(bb), Queen};
		}

		bb = PieceQuites(position, square);
		if (bb) {
			return {square, RandSquare(bb), Queen};
		}
	}

	return Ply::None;
}