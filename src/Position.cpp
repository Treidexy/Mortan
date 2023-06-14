#include "Position.h"

#include <memory>
#include <iostream>

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

	p++;
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

Position Position::Default() {
	return FromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

int Position::Preassure(Square square, Color myColor, BitBoard *checkPath) const {
	int preassure = 0;
	if (checkPath)
		*checkPath = 0;

	// preassure += Count(knightEyes[square] & byKind[Knight] & byColor[!myColor]);

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

	fn(RayMobilityWithBlockers<North>(square, board), lateralEnemies);
	fn(RayMobilityWithBlockers<South>(square, board), lateralEnemies);
	fn(RayMobilityWithBlockers<East>(square, board), lateralEnemies);
	fn(RayMobilityWithBlockers<West>(square, board), lateralEnemies);

	fn(RayMobilityWithBlockers<NorthEast>(square, board), diagonalEnemies);
	fn(RayMobilityWithBlockers<NorthWest>(square, board), diagonalEnemies);
	fn(RayMobilityWithBlockers<SouthEast>(square, board), diagonalEnemies);
	fn(RayMobilityWithBlockers<SouthWest>(square, board), diagonalEnemies);

	// I sure hope this works, I didn't test it
	if (!(!myColor == White && square / 8 == 0) || !(!myColor == Black && square / 8 == 7)) {
		fn((pawnEyes[square % 8] << (square / 8 - 1) * PawnForward(!myColor)) & byKind[Pawn] & byColor[!myColor], ~0ull);
	}

	fn(kingEyes[square] & byKind[King] & byColor[!myColor], ~0ull);

	return preassure;
}

bool Position::DoPly(Ply ply) {
	if (!(byColor[opp] & BitAt(ply.from))) {
		return false;
	}

	PieceKind kind = KindOf(bySquare[ply.from]);
	PieceKind captureKind = KindOf(bySquare[ply.to]);
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
	}

	if (kind == Rook) {
		if (ply.from / 8 == AFile) {
			castlingRights[opp] &= ~KingSide;
		} else if (ply.from / 8 == HFile) {
			castlingRights[opp] &= ~QueenSide;
		}
	}

	bool enPassant = false;
	if (kind == Pawn && ply.to == passant) {
		enPassant = true;
		captureKind = Pawn; // hard coded gang!
		captureSquare = Square(passant + (opp == White ? -8 : 8));

		// I might wanna put this somewhere else
		bySquare[passant] = PieceNone;
	}

	board &= ~BitAt(captureSquare);
	board &= ~BitAt(ply.from);
	board |= BitAt(ply.to);

	bySquare[captureSquare] = PieceNone;
	bySquare[ply.to] = bySquare[ply.from];
	bySquare[ply.from] = PieceNone;
	
	byColor[opp] &= ~BitAt(ply.from);
	byColor[opp] |= BitAt(ply.to);
	byColor[!opp] &= ~BitAt(captureSquare);

	byKind[kind] &= ~BitAt(ply.from);
	byKind[kind] |= BitAt(ply.to);
	if (captureKind != PieceKindNone) {
		byKind[captureKind] &= ~BitAt(captureSquare);
	}

	passant = SquareNone;
	if (kind == Pawn && (ply.to - ply.from) == 16) {
		passant = Square(ply.from + 8);
	} else if (kind == Pawn && (ply.to - ply.from) == -16) {
		passant = Square(ply.from - 8);
	}

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
	info.promotion = ply.promotion;

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