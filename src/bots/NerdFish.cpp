#include "bots/NerdFish.h"

#include <unordered_map>
#include <random>

#include "Position.h"
#include "Piece.h"
#include "Types.h"
#include "Util.h"

using namespace Mortan;

#define MAKE_MOVE_IF_CAN(sq) { BitBoard bb = PieceAttacks(position, sq); \
if (bb) {return {sq, WeakBit(bb), Queen};} \
bb = PieceQuites(position, sq); \
if (bb) { \
int idx = std::rand() % Count(bb); \
Square to; \
for (int i = 0; i <= idx; i++) {to = PopWeak(&bb);} \
return {sq, to, Queen};\
} }

namespace {
	
}

Ply NerdFish::MakeMove(const Position &position) {
	Color opp = position.opp;
	BitBoard ally = position.byColor[opp];

	BitBoard iter = ally;
	while (iter) {
		Square square = PopWeak(&iter);

		if (position.Preassure(square, opp) > 1) {
			MAKE_MOVE_IF_CAN(square);
		}
	}

	if (position.byKind[Pawn] & ally & BitAt(E2Sq)) {
		MAKE_MOVE_IF_CAN(E2Sq);
	}
	if (position.byKind[Knight] & ally & BitAt(G1Sq)) {
		MAKE_MOVE_IF_CAN(G1Sq);
	}
	if (position.byKind[Bishop] & ally & BitAt(F1Sq)) {
		MAKE_MOVE_IF_CAN(F1Sq);
	}
	if (position.byKind[Knight] & ally & BitAt(B1Sq)) {
		MAKE_MOVE_IF_CAN(B1Sq);
	}

	iter = ally;
	while (iter) {
		Square square = PopWeak(&iter);
		MAKE_MOVE_IF_CAN(square);
	}

	return Ply::None;
}