#pragma once

#include <stdint.h>

namespace Mortan {
	using BitBoard = uint64_t;

	enum Color {
		White,
		Black,

		ColorCount = 2,
		ColorNone,
	};

	enum PieceKind {
		King,
		Queen,
		Rook,
		Bishop,
		Knight,
		Pawn,

		PieceKindCount = 6,
		PieceKindNone,
	};

	enum Piece {
		WKing,
		WQueen,
		WRook,
		WBishop,
		WKnight,
		WPawn,

		BKing,
		BQueen,
		BRook,
		BBishop,
		BKnight,
		BPawn,

		PieceCount = 12,
		PieceNone,
	};

	enum Square {
		A1Sq, A2Sq, A3Sq, A4Sq, A5Sq, A6Sq, A7Sq, A8Sq,
		B1Sq, B2Sq, B3Sq, B4Sq, B5Sq, B6Sq, B7Sq, B8Sq,
		C1Sq, C2Sq, C3Sq, C4Sq, C5Sq, C6Sq, C7Sq, C8Sq,
		D1Sq, D2Sq, D3Sq, D4Sq, D5Sq, D6Sq, D7Sq, D8Sq,
		E1Sq, E2Sq, E3Sq, E4Sq, E5Sq, E6Sq, E7Sq, E8Sq,
		F1Sq, F2Sq, F3Sq, F4Sq, F5Sq, F6Sq, F7Sq, F8Sq,
		G1Sq, G2Sq, G3Sq, G4Sq, G5Sq, G6Sq, G7Sq, G8Sq,
		H1Sq, H2Sq, H3Sq, H4Sq, H5Sq, H6Sq, H7Sq, H8Sq,

		SquareCount = 64,
		SquareNone = 64,
	};

	enum Rank {
		Rank1, Rank2, Rank3, Rank4, Rank5, Rank6, Rank7, Rank8,

		NoRank = 8,
	};

	enum File {
		AFile = 8 * 0,
		BFile = 8 * 1,
		CFile = 8 * 2,
		DFile = 8 * 3,
		EFile = 8 * 4,
		FFile = 8 * 5,
		GFile = 8 * 6,
		HFile = 8 * 7,

		NoFile = 64,
	};
}