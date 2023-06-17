#include "Position.h"

#include <memory>
#include <iostream>
#include <sstream>

#include "Piece.h"
#include "Util.h"

using namespace Mortan;

Position Position::FromFEN(const char * const fen) {
	const char *p = fen;

	char c;
	int x = 0;
	int y = 7;
	Position position = {};

	while (y >= 0) {
		switch (c = *p++) {
		case 'K':
			position.bySquare[x + y * 8] = WKing;
			position.byColor[White] |= BitAt(x + y * 8);
			position.byKind[King] |= BitAt(x + y * 8);
			position.board |= BitAt(x + y * 8);
			x++;
			break;
		case 'Q':
			position.bySquare[x + y * 8] = WQueen;
			position.byColor[White] |= BitAt(x + y * 8);
			position.byKind[Queen] |= BitAt(x + y * 8);
			position.board |= BitAt(x + y * 8);
			x++;
			break;
		case 'R':
			position.bySquare[x + y * 8] = WRook;
			position.byColor[White] |= BitAt(x + y * 8);
			position.byKind[Rook] |= BitAt(x + y * 8);
			position.board |= BitAt(x + y * 8);
			x++;
			break;
		case 'B':
			position.bySquare[x + y * 8] = WBishop;
			position.byColor[White] |= BitAt(x + y * 8);
			position.byKind[Bishop] |= BitAt(x + y * 8);
			position.board |= BitAt(x + y * 8);
			x++;
			break;
		case 'N':
			position.bySquare[x + y * 8] = WKnight;
			position.byColor[White] |= BitAt(x + y * 8);
			position.byKind[Knight] |= BitAt(x + y * 8);
			position.board |= BitAt(x + y * 8);
			x++;
			break;
		case 'P':
			position.bySquare[x + y * 8] = WPawn;
			position.byColor[White] |= BitAt(x + y * 8);
			position.byKind[Pawn] |= BitAt(x + y * 8);
			position.board |= BitAt(x + y * 8);
			x++;
			break;
		case 'k':
			position.bySquare[x + y * 8] = BKing;
			position.byColor[Black] |= BitAt(x + y * 8);
			position.byKind[King] |= BitAt(x + y * 8);
			position.board |= BitAt(x + y * 8);
			x++;
			break;
		case 'q':
			position.bySquare[x + y * 8] = BQueen;
			position.byColor[Black] |= BitAt(x + y * 8);
			position.byKind[Queen] |= BitAt(x + y * 8);
			position.board |= BitAt(x + y * 8);
			x++;
			break;
		case 'r':
			position.bySquare[x + y * 8] = BRook;
			position.byColor[Black] |= BitAt(x + y * 8);
			position.byKind[Rook] |= BitAt(x + y * 8);
			position.board |= BitAt(x + y * 8);
			x++;
			break;
		case 'b':
			position.bySquare[x + y * 8] = BBishop;
			position.byColor[Black] |= BitAt(x + y * 8);
			position.byKind[Bishop] |= BitAt(x + y * 8);
			position.board |= BitAt(x + y * 8);
			x++;
			break;
		case 'n':
			position.bySquare[x + y * 8] = BKnight;
			position.byColor[Black] |= BitAt(x + y * 8);
			position.byKind[Knight] |= BitAt(x + y * 8);
			position.board |= BitAt(x + y * 8);
			x++;
			break;
		case 'p':
			position.bySquare[x + y * 8] = BPawn;
			position.byColor[Black] |= BitAt(x + y * 8);
			position.byKind[Pawn] |= BitAt(x + y * 8);
			position.board |= BitAt(x + y * 8);
			x++;
			break;

		case '/':
			if (x != 8) {
				std::cerr << "error reading fen: x = " << x << ", but not end of rank\nFEN string: " << fen << "\n";
			}

			x = 0;
			y--;
			break;

		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
			while (c-- != '0') {
				position.bySquare[x + y * 8] = PieceNone;
				x++;
			}
			break;
		case ' ':
			y--;
			break;

		default:
			std::cerr << "error reading fen: unknown board '" << c << "'\nFEN string: " << fen << "\n";
			break;
		}
	}

	// space is already eaten
	switch (c = *p++) {
	case 'w':
		position.opp = White;
		break;
	case 'b':
		position.opp = Black;
		break;
	default:
		std::cerr << "error reading fen: unknown side to move '" << c << "'\nFEN string: " << fen << "\n";
		break;
	}

	p++;
	while (*p != ' ') {
		switch (c = *p++) {
		case 'K':
			position.castlingRights[White] |= KingSide;
			break;
		case 'Q':
			position.castlingRights[White] |= QueenSide;
			break;
		case 'k':
			position.castlingRights[Black] |= KingSide;
			break;
		case 'q':
			position.castlingRights[Black] |= QueenSide;
			break;
		case '-':
			break;
		default:
			std::cerr << "error reading fen: unknown castlingRights '" << c << "'\nFEN string: " << fen << "\n";
			break;
		}
	}

	p++;
	if (*p == '-') {
		p++;
		position.passant = SquareNone;
	} else {
		File file = File(*p - 'a');
		p++;
		Rank rank = Rank((*p - '1') * 8);
		p++;

		position.passant = Square(file + rank);
	}

	//p++;
	//if (*p == '-') {
	//	p++;
	//} else {
	//	position.halfmoveClock = *p - '0';
	//	p++;
	//}

	//p++;
	//if (*p == '-') {
	//	p++;
	//} else {
	//	position.fullmoveNumber = *p - '0';
	//	p++;
	//}

	position.oppInCheck = !!position.Preassure(WeakBit(position.byKind[King] & position.byColor[position.opp]), position.opp, &position.checkPath);
	position.oppInDoubleCheck = position.Preassure(WeakBit(position.byKind[King] & position.byColor[position.opp]), position.opp, &position.checkPath) > 1;

	return position;	
}

std::string Position::GetFEN() {
	std::stringstream fen;


	for (int y = 7; y >= 0; y--) {
		int freeCount = 0;
		auto addPieceChar = [&](char c) {
			if (freeCount) {
				fen << char('0' + freeCount);
				freeCount = 0;
			}

			fen << c;
		};

		for (int x = 0; x < 8; x++) {
			switch (bySquare[x + y * 8]) {
			case WKing:
				addPieceChar('K');
				break;
			case WQueen:
				addPieceChar('Q');
				break;
			case WRook:
				addPieceChar('R');
				break;
			case WBishop:
				addPieceChar('B');
				break;
			case WKnight:
				addPieceChar('N');
				break;
			case WPawn:
				addPieceChar('P');
				break;

			case BKing:
				addPieceChar('k');
				break;
			case BQueen:
				addPieceChar('q');
				break;
			case BRook:
				addPieceChar('r');
				break;
			case BBishop:
				addPieceChar('b');
				break;
			case BKnight:
				addPieceChar('n');
				break;
			case BPawn:
				addPieceChar('p');
				break;

			case PieceNone:
				freeCount++;
				break;
			}
		}

		if (y != 0) {
			addPieceChar('/');
		}
	}

	fen << ' ' << (opp == White ? 'w' : 'b');
	fen << ' ';

	if (castlingRights[White] & KingSide) {
		fen << 'K';
	}
	if (castlingRights[White] & QueenSide) {
		fen << 'Q';
	}
	if (castlingRights[Black] & KingSide) {
		fen << 'k';
	}
	if (castlingRights[Black] & QueenSide) {
		fen << 'q';
	}

	if (!(castlingRights[White] | castlingRights[Black])) {
		fen << '-';
	}

	fen << ' ';
	if (passant == SquareNone) {
		fen << '-';
	} else {
		fen << char('a' + passant % 8) << char('0' + passant / 8);
	}
	fen << " 0 1";
	return fen.str();
}

Position Position::Default() {
	return FromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

int Position::Preassure(Square square, Color myColor, BitBoard *checkPath) const {
	int preassure = 0;
	if (checkPath)
		*checkPath = 0;

	// preassure += Count(knightEyes[square] & byKind[Knight] & byColor[!myColor]);

	// tinay hack, might fix later
	BitBoard boardWithoutKing = board & ~(byKind[King] & byColor[myColor]);

	BitBoard lateralEnemies = (byKind[Rook] | byKind[Queen]) & byColor[!myColor];
	BitBoard diagonalEnemies = (byKind[Bishop] | byKind[Queen]) & byColor[!myColor];

	auto fn = [&](BitBoard ray, BitBoard enemy) {
		if (ray & enemy) {
			preassure += Count(ray & enemy);
			if (checkPath)
				*checkPath |= ray;
		}
	};

	fn(knightEyes[square] & byKind[Knight] & byColor[!myColor], ~0ull);

	fn(RayMobilityWithBlockers<North>(square, boardWithoutKing), lateralEnemies);
	fn(RayMobilityWithBlockers<South>(square, boardWithoutKing), lateralEnemies);
	fn(RayMobilityWithBlockers<East>(square, boardWithoutKing), lateralEnemies);
	fn(RayMobilityWithBlockers<West>(square, boardWithoutKing), lateralEnemies);

	fn(RayMobilityWithBlockers<NorthEast>(square, boardWithoutKing), diagonalEnemies);
	fn(RayMobilityWithBlockers<NorthWest>(square, boardWithoutKing), diagonalEnemies);
	fn(RayMobilityWithBlockers<SouthEast>(square, boardWithoutKing), diagonalEnemies);
	fn(RayMobilityWithBlockers<SouthWest>(square, boardWithoutKing), diagonalEnemies);

	// I sure hope this works, I didn't test it
	if (!(!myColor == White && square / 8 == 0) || !(!myColor == Black && square / 8 == 7)) {
		fn((pawnEyes[square % 8] << ((square - square % 8) - PawnForward(!myColor))) & byKind[Pawn] & byColor[!myColor], ~0ull);
	}

	fn(kingEyes[square] & byKind[King] & byColor[!myColor], ~0ull);

	return preassure;
}

// TODO: clean up
bool Position::DoPly(Ply ply) {
	if (ply.from == SquareNone) {
		return false;
	}

	if (!(byColor[opp] & BitAt(ply.from))) {
		return false;
	}

	PieceKind kind = KindOf(bySquare[ply.from]);
	PieceKind captureKind = KindOf(bySquare[ply.to]);
	PieceKind newKind = kind;
	Square captureSquare = ply.to;

	// veryfy ply
	if (captureKind != PieceKindNone || (kind == Pawn && ply.from % 8 - ply.to % 8 != 0)) {
		if (!(PieceAttacks(*this, ply.from) & BitAt(ply.to))) {
			return false;
		}
	} else {
		if (!(PieceQuites(*this, ply.from) & BitAt(ply.to))) {
			return false;
		}
	}

	if (kind == King) {
		castlingRights[opp] = CastlingNone;

		// castling
		if (ply.to - ply.from == 2) { // kingside
			board |= BitAt(ply.from + 1);
			board &= ~BitAt(ply.from + 3);

			bySquare[ply.from + 1] = bySquare[ply.from + 3];
			bySquare[ply.from + 3] = PieceNone;

			byColor[opp] &= ~BitAt(ply.from + 3);
			byColor[opp] |= BitAt(ply.from + 1);

			byKind[ply.from + 1] = byKind[ply.from + 3];
			byKind[ply.from + 3] = PieceKindNone;
		} else if (ply.from - ply.to == 2) { // queenside
			board |= BitAt(ply.from - 1);
			board &= ~BitAt(ply.from - 4);

			bySquare[ply.from - 1] = bySquare[ply.from - 4];
			bySquare[ply.from - 4] = PieceNone;

			byColor[opp] &= ~BitAt(ply.from - 4);
			byColor[opp] |= BitAt(ply.from - 1);

			byKind[ply.from - 1] = byKind[ply.from - 4];
			byKind[ply.from - 4] = PieceKindNone;
		}
	} 

	if (kind == Rook) {
		if (ply.from / 8 == AFile) {
			castlingRights[opp] &= ~KingSide;
		} else if (ply.from / 8 == HFile) {
			castlingRights[opp] &= ~QueenSide;
		}
	}

	bool enPassant = false;
	if (kind == Pawn) {
		if (ply.to / 8 == 0 || ply.to / 8 == 7) {
			newKind = ply.promotion;
		}

		if (ply.to == passant) {
			enPassant = true;
			captureKind = Pawn; // hard coded gang!
			captureSquare = Square(passant + (opp == White ? -8 : 8));

			// I might wanna put this somewhere else
			bySquare[passant] = PieceNone;
		}
	}

	passant = SquareNone;
	if (kind == Pawn) {
		if (ply.to - ply.from == 16) {
			passant = Square(ply.from + 8);
		} else if (ply.from - ply.to == 16) {
			passant = Square(ply.from - 8);
		}
	}


	if (captureKind != PieceKindNone) {
		board &= ~BitAt(captureSquare);
		bySquare[captureSquare] = PieceNone;
		byColor[!opp] &= ~BitAt(captureSquare);
		byKind[captureKind] &= ~BitAt(captureSquare);
	}

	board |= BitAt(ply.to);
	board &= ~BitAt(ply.from);

	bySquare[ply.to] = PieceOf(opp, newKind);
	bySquare[ply.from] = PieceNone;
	
	byColor[opp] |= BitAt(ply.to);
	byColor[opp] &= ~BitAt(ply.from);

	byKind[newKind] |= BitAt(ply.to);
	byKind[kind] &= ~BitAt(ply.from);


	// TODO: optimize
	oppInCheck = !!Preassure(WeakBit(byKind[King] & byColor[!opp]), !opp, &checkPath);
	oppInDoubleCheck = Preassure(WeakBit(byKind[King] & byColor[!opp]), !opp, &checkPath) > 1;


	PlyInfo info = {};
	info.from = ply.from;
	info.to = ply.to;
	info.passant = passant;
	info.check = oppInCheck;
	info.doubleCheck = oppInDoubleCheck;
	info.enPassant = enPassant;
	info.castling = CastlingNone;
	info.promotion = newKind;

	currMove.byColor[opp] = info;


	if (opp == White) {
		opp = Black;
	} else {
		opp = White;
		moves.push_back(currMove);
		currMove = {}; // unnecessary?
	}

	return true;
}