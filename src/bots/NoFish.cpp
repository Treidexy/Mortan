#include "bots/NoFish.h"

#include "Position.h"
#include "Piece.h"
#include "Util.h"

#include <iostream>

using namespace Mortan;

namespace {
	struct SquareInfo {
		Square square;
		PieceKind kind;
		int value;

		int defence;
		int preassure;

		int distFromCenter;
	};

	struct Move {
		Ply ply;
		
		SquareInfo from;
		SquareInfo to;
	};

	Move GetMove(const Position &, Ply);
	Move BetterMove(Move, Move);
	int MaterialValue(PieceKind);

	SquareInfo GetSquareInfo(const Position &position, Square square) {
		PieceKind kind = KindOf(position.bySquare[square]);
		int value = MaterialValue(kind);

		int defence = position.Preassure(square, !position.opp);
		int preassure = position.Preassure(square, position.opp);

		int x = square % 8;
		int y = square / 8;

		int distFromCenter = std::min(x, 7 - x) + std::min(y, 7 - y);

		return {square, kind, value, defence, preassure, distFromCenter};
	}

	Move GetMove(const Position &position, SquareInfo from, Square to) {
		return {Ply {from.square, to, Queen}, from, GetSquareInfo(position, to)};
	}

	Move BetterMove(Move a, Move b) {
		if (a.from.value == 0) {
			return b;
		}

		// don't be stupid

		if (b.to.defence > b.to.preassure && a.to.defence < a.to.preassure) {
			return b;
		} else if (a.to.defence > a.to.preassure && b.to.defence < b.to.preassure) {
			return a;
		}

		// capture

		if (b.to.defence > b.to.preassure && b.to.value > a.to.value) {
			return b;
		} else if (a.to.defence > a.to.preassure && a.to.value > b.to.value) {
			return a;
		}

		if (b.to.defence < b.to.preassure && b.from.value > b.to.value) {
			return a;
		} else if (a.to.defence < a.to.preassure && a.from.value > a.to.value) {
			return b;
		}


		if (b.from.preassure > b.from.defence) {
			if (a.from.defence >= a.from.preassure) {
				return b;
			}

			return b.from.value > a.from.value ? b : a;
		} else if (a.from.preassure > a.from.defence) {
			if (b.from.defence >= b.from.preassure) {
				return a;
			}

			return a.from.value >= b.from.value ? a : b;
		}

		// MISC

		if (b.to.distFromCenter > a.to.distFromCenter) {
			return b;
		} else if (a.to.distFromCenter > b.to.distFromCenter) {
			return a;
		}

		return rand() % 2 ? a : b;
	}

	int MaterialValue(PieceKind kind) {
		switch (kind) {
		case King:
			return 69;
		case Queen:
			return 9;
		case Rook:
			return 5;
		case Bishop:
			return 3;
		case Knight:
			return 3;
		case Pawn:
			return 1;

		default:
			return 0;
		}
	}
}

Ply NoFish::MakeMove(const Position &position) {
	BitBoard pieces = position.byColor[position.opp];

	Move bestMove = { Ply::None, };

	while (pieces) {
		Square square = PopWeak(&pieces);
		SquareInfo info = GetSquareInfo(position, square);

		BitBoard bb = PieceAttacks(position, square) | PieceQuites(position, square);
		while (bb) {
			Square to = PopWeak(&bb);
			bestMove = BetterMove(bestMove, GetMove(position, info, to));
		}
	}

	return bestMove.ply;
}