#include "Nav.h"

#include <algorithm>

#include "Piece.h"
#include "Util.h"

using namespace Mortan;

// !independent from impl of TaxiPath
Square Mortan::LateralJoint(Square from, Square to) {
	return Square(to % 8 + from - from % 8);
}

Square Mortan::DiagonalJoint(Square from, Square to) {
	int dx2 = (to % 8 - from % 8) >> 1;
	int dy2 = (to / 8 - from / 8) >> 1;

	int x = from % 8 + dx2 - dy2;
	int y = from / 8 - dx2 + dy2;
	return Square(x + y * 8);
}

BitBoard Mortan::TaxiPathLateral(Square from, Square to) {
	int x = to % 8;
	int y = from / 8;

	auto [x1, x2] = std::minmax(x, from % 8);
	auto [y1, y2] = std::minmax(y, to / 8);

	BitBoard hor = eyeRays[West][x1 + y * 8] & eyeRays[East][x2 + y * 8];
	BitBoard ver = eyeRays[North][x + y1 * 8] & eyeRays[South][x + y2 * 8];

	return hor | ver | BitAt(x + y * 8);
}

BitBoard Mortan::TaxiPathDiagonal(Square from, Square to) {
	if ((from % 8 + from / 8 + 1) % 2 != (to % 8 + to / 8 + 1) % 2) {
		return 0;
	}

	Square joint = DiagonalJoint(from, to);

	// TODO: clean lazy code
	BitBoard ne = (eyeRays[NorthEast][from] & eyeRays[SouthWest][joint]) | (eyeRays[SouthWest][from] & eyeRays[NorthEast][joint]);
	BitBoard nw = (eyeRays[NorthWest][to] & eyeRays[SouthEast][joint]) | (eyeRays[SouthEast][to] & eyeRays[NorthWest][joint]);

	return ne | nw | BitAt(joint);
}