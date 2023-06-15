#include "bots/NerdFish.h"

#include <unordered_map>
#include <random>
#include <iostream>

#include "Position.h"
#include "Piece.h"
#include "Types.h"
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

	BitBoard rooks = position.byKind[Rook] & ally;
	Square rook1 = WeakBit(rooks);
	Square rook2 = StrongBit(rooks);

	BitBoard rankMask1 = 0xFFull << (rook1 - rook1 % 8);
	BitBoard rankMask2 = 0xFFull << (rook2 - rook2 % 8);

	if (rook1 % 8 < enemyKingSquare % 8) {
		if (rook2 % 8 < enemyKingSquare % 8) {
			if (rook1 % 8 < rook2 % 8) {
				if (BitAt(rook2) & enemyKingEyes) {
					File file = File(rook2 % 8);
					Square target = Square(file + Rank8);
					if (0xFFFFFF0000000000 & enemyKing) {
						target = Square(file + Rank1);
						if (BitAt(target) & rankMask1) {
							target = Square(file + Rank2);
						}
					}

					return {rook2, target, PieceKindNone};
				}

				Square target = Square(rook1 - rook1 % 8 + rook2 % 8 + 1);

				if (BitAt(target) & enemyKingEyes) {
					// move rook to safe rank
					File file = File(rook1 % 8);
					target = Square(file + Rank8);
					if (0xFFFFFF0000000000 & enemyKing) {
						target = Square(file + Rank1);
						if (BitAt(target) & rankMask2) {
							target = Square(file + Rank2);
						}
					}
				}

				return { rook1, target, PieceKindNone };
			} else {
				if (BitAt(rook1) & enemyKingEyes) {
					File file = File(rook1 % 8);
					Square target = Square(file + Rank8);
					if (0xFFFFFF0000000000 & enemyKing) {
						target = Square(file + Rank1);
						if (BitAt(target) & rankMask2) {
							target = Square(file + Rank2);
						}
					}

					return {rook1, target, PieceKindNone};
				}

				Square target = Square(rook2 - rook2 % 8 + rook1 % 8 + 1);
				if (BitAt(target) & enemyKingEyes) {
					// move rook to safe rank
					File file = File(rook2 % 8);
					target = Square(file + Rank8);
					if (0xFFFFFF0000000000 & enemyKing) {
						target = Square(file + Rank1);
						if (BitAt(target) & rankMask1) {
							target = Square(file + Rank2);
						}
					}
				}

				return { rook2, Square(rook2 - rook2 % 8 + rook1 % 8 + 1), PieceKindNone };
			}
		}
	}

	return Ply::None;
}