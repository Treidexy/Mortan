#include "Piece.h"

#include "Util.h"

using namespace Mortan;

BitBoard Mortan::kingEyes[SquareCount];
BitBoard Mortan::rookEyes[SquareCount];
BitBoard Mortan::bishopEyes[SquareCount];
BitBoard Mortan::knightEyes[SquareCount];

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

	BitBoard RookEyes(int x, int y) {
		BitBoard mask = 0;

		mask |= 0xFFull << (y * 8);
		mask |= 0x101010101010101ull << x;

		return mask;
	}

	BitBoard BishopEyes(int x, int y) {
		BitBoard mask = 0;

		for (int i = 1; i < 8; i++) {
			if (x >= i && y >= i) {
				mask |= BitAt((x - i) + (y - i) * 8);
			}

			if (x >= i && y <= 7 - i) {
				mask |= BitAt((x - i) + (y + i) * 8);
			}

			if (x <= 7 - i && y >= i) {
				mask |= BitAt((x + i) + (y - i) * 8);
			}

			if (x <= 7 - i && y <= 7 - i) {
				mask |= BitAt((x + i) + (y + i) * 8);
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
}

void Mortan::InitEyes() {
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			kingEyes[x + y * 8] = KingEyes(x, y);
			rookEyes[x + y * 8] = RookEyes(x, y);
			bishopEyes[x + y * 8] = BishopEyes(x, y);
			knightEyes[x + y * 8] = KnightEyes(x, y);
		}
	}
}