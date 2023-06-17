#include "bots/NerdFish.h"

#include <unordered_map>
#include <random>
#include <iostream>

#include "Position.h"
#include "Piece.h"
#include "Types.h"
#include "Nav.h"
#include "Util.h"

using namespace Mortan;

namespace {
}

Ply NerdFish::MakeMove(const Position &position) {
	Color opp = position.opp;
	BitBoard ally = position.byColor[opp];

	BitBoard enemyKing = position.byKind[King] & position.byColor[!opp];
	Square enemyKingSquare = WeakBit(enemyKing);
	BitBoard enemyKingEyes = kingEyes[enemyKingSquare];

	std::vector<Ply> checks;

	BitBoard iter = ally;
	while (iter) {
		Square square = PopWeak(&iter);

		BitBoard attacks = PieceAttacks(position, square);
		BitBoard quites = PieceQuites(position, square);


	}

	BitBoard rookLike = (position.byKind[Rook] | position.byKind[Queen]) & ally;
	while (rookLike) {
		Square square = PopWeak(&rookLike);

		if (!(TaxiPathLateral(enemyKingSquare, square) & position.board)) {
			checks.push_back(Ply {square, LateralJoint(enemyKingSquare, square), PieceKindNone});
		}
		if (!(TaxiPathLateral(square, enemyKingSquare) & position.board)) {
			checks.push_back(Ply {square, LateralJoint(square, enemyKingSquare), PieceKindNone});
		}
	}

	BitBoard bishopLike = (position.byKind[Bishop] | position.byKind[Queen]) & ally;
	while (bishopLike) {
		Square square = PopWeak(&bishopLike);

		if (!(TaxiPathDiagonal(enemyKingSquare, square) & position.board)) {
			checks.push_back(Ply {square, DiagonalJoint(enemyKingSquare, square), PieceKindNone});
		}
		if (!(TaxiPathDiagonal(square, enemyKingSquare) & position.board)) {
			checks.push_back(Ply {square, DiagonalJoint(square, enemyKingSquare), PieceKindNone});
		}
	}

	return Ply::None;
}