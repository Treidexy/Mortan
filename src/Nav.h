#pragma once

#include "Types.h"

namespace Mortan {
	// goes clockwise; to go counterclockwise: fn(to, from) instead of fn(from, to)
	Square LateralJoint(Square from, Square to);
	Square DiagonalJoint(Square from, Square to);

	BitBoard TaxiPathLateral(Square from, Square to);
	BitBoard TaxiPathDiagonal(Square from, Square to);
}