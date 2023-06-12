#include "Position.h"

#include <memory>
#include <iostream>

#include "Piece.h"
#include "Util.h"

using namespace Mortan;

Position Position::FromFEN(const char * const fen) {
	const char *p = fen;

	char c;
	int idx = 63;
	Position position = {};

	while (idx >= 0) {
		switch (c = *p++) {
		case 'K':
			position.bySquare[idx] = WKing;
			position.byColor[White] |= BitAt(idx);
			position.byKind[King] |= BitAt(idx);
			idx--;
			break;
		case 'Q':
			position.bySquare[idx] = WQueen;
			position.byColor[White] |= BitAt(idx);
			position.byKind[Queen] |= BitAt(idx);
			idx--;
			break;
		case 'R':
			position.bySquare[idx] = WRook;
			position.byColor[White] |= BitAt(idx);
			position.byKind[Rook] |= BitAt(idx);
			idx--;
			break;
		case 'B':
			position.bySquare[idx] = WBishop;
			position.byColor[White] |= BitAt(idx);
			position.byKind[Bishop] |= BitAt(idx);
			idx--;
			break;
		case 'N':
			position.bySquare[idx] = WKnight;
			position.byColor[White] |= BitAt(idx);
			position.byKind[Knight] |= BitAt(idx);
			idx--;
			break;
		case 'P':
			position.bySquare[idx] = WPawn;
			position.byColor[White] |= BitAt(idx);
			position.byKind[Pawn] |= BitAt(idx);
			idx--;
			break;
		case 'k':
			position.bySquare[idx] = BKing;
			position.byColor[Black] |= BitAt(idx);
			position.byKind[King] |= BitAt(idx);
			idx--;
			break;
		case 'q':
			position.bySquare[idx] = BQueen;
			position.byColor[Black] |= BitAt(idx);
			position.byKind[Queen] |= BitAt(idx);
			idx--;
			break;
		case 'r':
			position.bySquare[idx] = BRook;
			position.byColor[Black] |= BitAt(idx);
			position.byKind[Rook] |= BitAt(idx);
			idx--;
			break;
		case 'b':
			position.bySquare[idx] = BBishop;
			position.byColor[White] |= BitAt(idx);
			position.byKind[Bishop] |= BitAt(idx);
			idx--;
			break;
		case 'n':
			position.bySquare[idx] = BKnight;
			position.byColor[Black] |= BitAt(idx);
			position.byKind[Knight] |= BitAt(idx);
			idx--;
			break;
		case 'p':
			position.bySquare[idx] = BPawn;
			position.byColor[Black] |= BitAt(idx);
			position.byKind[Pawn] |= BitAt(idx);
			idx--;
			break;

		case '/':
			if (idx % 8 != 7) {
				std::cerr << "error reading fen: idx = " << idx << ", but not end of rank\nFEN string: " << fen << "\n";
			}
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
				position.bySquare[idx] = PieceNone;
				idx--;
			}
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
			position.castling[White] |= KingSide;
			break;
		case 'Q':
			position.castling[White] |= QueenSide;
			break;
		case 'k':
			position.castling[Black] |= KingSide;
			break;
		case 'q':
			position.castling[Black] |= QueenSide;
			break;
		case '-':
			break;
		default:
			std::cerr << "error reading fen: unknown castling '" << c << "'\nFEN string: " << fen << "\n";
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

	return position;	
}

Position Position::Default() {
	return FromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

int Position::Preassure(Square square, Color myColor) {
	int preassure = 0;

	preassure += Count(knightEyes[square] & byKind[Knight] & byColor[!myColor]);

	BitBoard lateralEnemies = (byKind[Rook] | byKind[Queen]) & byColor[!myColor];
	BitBoard diagonalEnemies = (byKind[Bishop] | byKind[Queen]) & byColor[!myColor];

	// double exclam to normalize to 1 or 0
	preassure += !!(RayMobilityWithBlockers<North>(square, board) & lateralEnemies);
	preassure += !!(RayMobilityWithBlockers<South>(square, board) & lateralEnemies);
	preassure += !!(RayMobilityWithBlockers<East>(square, board) & lateralEnemies);
	preassure += !!(RayMobilityWithBlockers<West>(square, board) & lateralEnemies);

	preassure += !!(RayMobilityWithBlockers<NorthEast>(square, board) & diagonalEnemies);
	preassure += !!(RayMobilityWithBlockers<NorthWest>(square, board) & diagonalEnemies);
	preassure += !!(RayMobilityWithBlockers<SouthEast>(square, board) & diagonalEnemies);
	preassure += !!(RayMobilityWithBlockers<SouthWest>(square, board) & diagonalEnemies);

	return preassure;
}

void Position::DoPly(Ply ply) {
	PieceKind kind = KindOf(bySquare[ply.from]);
	PieceKind captureKind = KindOf(bySquare[ply.from]);

	board &= ~BitAt(ply.from);
	board |= BitAt(ply.to);

	bySquare[ply.to] = bySquare[ply.from];
	bySquare[ply.from] = PieceNone;
	
	byColor[opp] &= ~BitAt(ply.from);
	byColor[opp] |= BitAt(ply.to);
	byColor[!opp] &= ~BitAt(ply.to);

	byKind[kind] &= ~BitAt(ply.from);
	byKind[kind] |= BitAt(ply.to);
	if (captureKind != PieceKindNone) {
		byKind[captureKind] &= ~BitAt(ply.to);
	}

	passant = SquareNone;

	currMove.byColor[opp] = ply;

	if (opp == White) {
		opp = Black;
	} else {
		opp = White;
		moves.push_back(currMove);
		currMove = {}; // unnecessary?
	}
}