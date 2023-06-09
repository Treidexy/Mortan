#include "Piece.h"

#include "Util.h"

using namespace Mortan;

BitBoard Mortan::kingEyes[SquareCount];
BitBoard Mortan::knightEyes[SquareCount];
BitBoard Mortan::eyeRays[DirectionCount][SquareCount];

namespace {
	BitBoard KingEyes(int x, int y) {
		BitBoard mask = 0;

		if (x > 0) {
			if (y > 0) {
				mask |= BitAt((x - 1) + (y - 1) * 8);
			}

			mask |= BitAt((x - 1) + y * 8);

			if (y < 7) {
				mask |= BitAt((x - 1) + (y + 1) * 8);
			}
		}

		if (y > 0) {
			mask |= BitAt(x + (y - 1) * 8);
		}

		if (y < 7) {
			mask |= BitAt(x + (y + 1) * 8);
		}

		if (x < 7) {
			if (y > 0) {
				mask |= BitAt((x + 1) + (y - 1) * 8);
			}

			mask |= BitAt((x + 1) + y * 8);

			if (y < 7) {
				mask |= BitAt((x + 1) + (y + 1) * 8);
			}
		}

		return mask;
	}

	BitBoard KnightEyes(int x, int y) {
		BitBoard mask = 0;

		if (x > 1) {
			if (y > 0) {
				mask |= BitAt((x - 2) + (y - 1) * 8);
			}

			if (y < 7) {
				mask |= BitAt((x - 2) + (y + 1) * 8);
			}
		}

		if (x > 0) {
			if (y > 1) {
				mask |= BitAt((x - 1) + (y - 2) * 8);
			}

			if (y < 6) {
				mask |= BitAt((x - 1) + (y + 2) * 8);
			}
		}

		if (x < 6) {
			if (y > 0) {
				mask |= BitAt((x + 2) + (y - 1) * 8);
			}

			if (y < 7) {
				mask |= BitAt((x + 2) + (y + 1) * 8);
			}
		}

		if (x < 7) {
			if (y > 1) {
				mask |= BitAt((x + 1) + (y - 2) * 8);
			}

			if (y < 6) {
				mask |= BitAt((x + 1) + (y + 2) * 8);
			}
		}

		return  mask;
	}

	void InitRaysAt(int x, int y) {
		eyeRays[North][x + y * 8] = 0;
		eyeRays[South][x + y * 8] = 0;
		eyeRays[East][x + y * 8] = 0;
		eyeRays[West][x + y * 8] = 0;

		eyeRays[NorthEast][x + y * 8] = 0;
		eyeRays[NorthWest][x + y * 8] = 0;
		eyeRays[SouthEast][x + y * 8] = 0;
		eyeRays[SouthWest][x + y * 8] = 0;

		for (int i = 1; i < 8; i++) {
			bool onEast = x >= i; // x - i >= 0
			bool onWest = x + i < 8;

			if (y + i < 8) {
				eyeRays[North][x + y * 8] |= BitAt(x + (y + i) * 8);

				if (onEast) {
					eyeRays[NorthEast][x + y * 8] |= BitAt((x - i) + (y + i) * 8);
				}
				if (onWest) {
					eyeRays[NorthWest][x + y * 8] |= BitAt((x + i) + (y + i) * 8);
				}
			}

			if (onEast) {
				eyeRays[East][x + y * 8] |= BitAt((x - i) + y * 8);
			}
			if (onWest) {
				eyeRays[West][x + y * 8] |= BitAt((x + i) + y * 8);
			}

			// y - i >= 0
			if (y >= i) {
				eyeRays[South][x + y * 8] |= BitAt(x + (y - i) * 8);

				if (onEast) {
					eyeRays[SouthEast][x + y * 8] |= BitAt((x - i) + (y - i) * 8);
				}
				if (onWest) {
					eyeRays[SouthWest][x + y * 8] |= BitAt((x + i) + (y - i) * 8);
				}
			}
		}
	}
}

void Mortan::InitEyes() {
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			InitRaysAt(x, y);

			kingEyes[x + y * 8] = KingEyes(x, y);
			knightEyes[x + y * 8] = KnightEyes(x, y);
		}
	}
}