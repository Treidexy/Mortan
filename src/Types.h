#pragma once

#include <stdint.h>

namespace Mortan {
	using BitBoard = uint64_t;

	// DO NOT CHANGE ANY: int

	enum Color: int {
		White = 0,
		Black = 1,

		ColorCount = 2,
		ColorNone,
	};

	enum PieceKind: int {
		King,
		Queen,
		Rook,
		Bishop,
		Knight,
		Pawn,

		PieceKindCount = 6,
		PieceKindNone,
	};

	enum Piece: int {
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

	enum Square: int {
		A1Sq, B1Sq, C1Sq, D1Sq, E1Sq, F1Sq, G1Sq, H1Sq,
		A2Sq, B2Sq, C2Sq, D2Sq, E2Sq, F2Sq, G2Sq, H2Sq,
		A3Sq, B3Sq, C3Sq, D3Sq, E3Sq, F3Sq, G3Sq, H3Sq,
		A4Sq, B4Sq, C4Sq, D4Sq, E4Sq, F4Sq, G4Sq, H4Sq,
		A5Sq, B5Sq, C5Sq, D5Sq, E5Sq, F5Sq, G5Sq, H5Sq,
		A6Sq, B6Sq, C6Sq, D6Sq, E6Sq, F6Sq, G6Sq, H6Sq,
		A7Sq, B7Sq, C7Sq, D7Sq, E7Sq, F7Sq, G7Sq, H7Sq,
		A8Sq, B8Sq, C8Sq, D8Sq, E8Sq, F8Sq, G8Sq, H8Sq,

		SquareCount = 64,
		SquareNone = 64,
	};

	enum Rank: int {
		Rank1 = 8 * 0,
		Rank2 = 8 * 1,
		Rank3 = 8 * 2,
		Rank4 = 8 * 3,
		Rank5 = 8 * 4,
		Rank6 = 8 * 5,
		Rank7 = 8 * 6,
		Rank8 = 8 * 7,

		NoRank = 64,
	};

	enum File: int {
		AFile,
		BFile,
		CFile,
		DFile,
		EFile,
		FFile,
		GFile,
		HFile,

		FileCount = 8,
		NoFile = 8,
	};

	// North = y++, East = x--,
	enum Direction {
		North,
		South,
		East,
		West,

		NorthEast,
		NorthWest,
		SouthEast,
		SouthWest,

		DirectionCount = 8,
		DirectionNone = 8,
	};

	enum Castling: int {
		CastlingNone = 0,
		KingSide = 1 << 0,
		QueenSide = 1 << 1,
	};

	// because C++ is FUCKING GARBAGE
	inline Color operator! (Color a) { return Color(!int(a)); }

	inline Castling operator~ (Castling a) { return Castling(~int(a)); }
	inline Castling operator| (Castling a, Castling b) { return Castling(int(a) | int(b)); }
	inline Castling operator& (Castling a, Castling b) { return Castling(int(a) & int(b)); }
	inline Castling operator^ (Castling a, Castling b) { return Castling(int(a) ^ int(b)); }
	inline Castling &operator|= (Castling &a, Castling b) { return (Castling &) ((int &) a |= b); }
	inline Castling &operator&= (Castling &a, Castling b) { return (Castling &) ((int &) a &= b); }
	inline Castling &operator^= (Castling &a, Castling b) { return (Castling &) ((int &) a ^= b); }
}