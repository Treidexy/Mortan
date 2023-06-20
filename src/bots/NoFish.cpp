#include "bots/NoFish.h"

#include "Position.h"
#include "Piece.h"
#include "Util.h"

#include <iostream>

using namespace Mortan;

namespace {
	enum class Reason {
	#define REASON(x) x,
	#include "bots/NoFishReasons.h"
	};

	std::ostream &operator<<(std::ostream &os, Reason reason) {
		switch (reason) {
		#define REASON(x) case Reason::x: return os << #x;
		#include "bots/NoFishReasons.h"
		}

		return os << "Unknown";
	}

	struct MoveCmpResult {
		Reason reason;
		bool isWorse;
		bool isSame;
	};

	struct SquareInfo {
		Square square;
		PieceKind kind;
		int value;

		int defence;
		int preassure;

		int distFromCenter;
	};

	struct Move {
		Ply ply;
		
		bool enPassant;
		SquareInfo from;
		SquareInfo to;
	};

	Move GetMove(const Position &, Ply);
	Move BetterMove(Move, Move);
	int MaterialValue(PieceKind);

	SquareInfo GetSquareInfo(const Position &position, Square square) {
		PieceKind kind = KindOf(position.bySquare[square]);
		int value = MaterialValue(kind);

		int defence = position.Preassure(square, !position.opp);
		int preassure = position.Preassure(square, position.opp);

		int x = square % 8;
		int y = square / 8;

		int distFromCenter = std::min(x, 7 - x) + std::min(y, 7 - y);

		return {square, kind, value, defence, preassure, distFromCenter};
	}

	Move GetMove(const Position &position, SquareInfo from, Square to) {
		return {Ply {from.square, to, Queen}, from.kind == Pawn && to == position.passant, from, GetSquareInfo(position, to)};
	}

	MoveCmpResult CmpMove(Move a, Move b) {
		if (a.enPassant && !b.enPassant) {
			return { Reason::EnPassant, true, false };
		} else if (b.enPassant && !a.enPassant) {
			return { Reason::EnPassant, false, false };
		}

		if (a.to.value > 0 && b.to.value > 0) {
			int aScore = a.to.value;
			if (a.to.preassure) {
				aScore -= a.from.value;
			}

			int bScore = b.to.value;
			if (b.to.preassure) {
				bScore -= b.from.value;
			}

			if (aScore > bScore) {
				return {Reason::BetterCapture, true, false};
			} else if (bScore > aScore) {
				return {Reason::BetterCapture, false, false};
			}
		}

		if (!a.to.preassure && !b.to.preassure) {
			if (a.to.value && !b.to.value) {
				return {Reason::IAteIt, true, false};
			} else if (b.to.value && !a.to.value) {
				return {Reason::IAteIt, false, false};
			}
		}

		if (a.to.preassure && b.to.preassure) {
			if (a.from.value < b.from.value) {
				return {Reason::BetterTrade, true, false};
			} else if (b.from.value < a.from.value) {
				return {Reason::BetterTrade, false, false};
			}
		}

		if (!a.to.preassure && b.to.preassure) {
			return {Reason::NoHang, true, false};
		} else if (!b.to.preassure && a.to.preassure) {
			return {Reason::NoHang, false, false};
		}

		if (a.from.preassure && !b.from.preassure) {
			return {Reason::InDanger, true, false};
		} else if (b.from.preassure && !a.from.preassure) {
			return {Reason::InDanger, false, false};
		}

		//if (b.to.distFromCenter < a.to.distFromCenter) {
		//	return {Reason::CenterControl, true, false};
		//} else if (a.to.distFromCenter < b.to.distFromCenter) {
		//	return {Reason::CenterControl, false, false};
		//}

		return {Reason::Equal, true, true};
	}

	int MaterialValue(PieceKind kind) {
		switch (kind) {
		case King:
			return 69;
		case Queen:
			return 9;
		case Rook:
			return 5;
		case Bishop:
			return 3;
		case Knight:
			return 3;
		case Pawn:
			return 1;

		default:
			return 0;
		}
	}
}

std::ostream &operator<<(std::ostream &os, Square s) {
	return os << char(s % 8 + 'a') << char(s / 8 + '1');
}

Ply NoFish::MakeMove(const Position &position) {
	BitBoard pieces = position.byColor[position.opp];

	std::vector<Reason> reasons = {};
	std::vector<Move> bestMoves;
	std::vector<Ply> secondBestPlys;

	while (pieces) {
		Square square = PopWeak(&pieces);
		SquareInfo info = GetSquareInfo(position, square);

		BitBoard bb = PieceAttacks(position, square) | PieceQuites(position, square);
		while (bb) {
			Square to = PopWeak(&bb);
			Move contender = GetMove(position, info, to);

			if (bestMoves.empty()) {
				bestMoves.push_back(contender);
			} else {
				MoveCmpResult result = CmpMove(bestMoves.front(), contender);
				if (result.isSame) {
					bestMoves.push_back(contender);
					// should be same reason but returns Equal
				} else if (!result.isWorse) {
					secondBestPlys.clear();
					for (const Move &bestMove : bestMoves) {
						secondBestPlys.push_back(bestMove.ply);
					}

					bestMoves.clear();
					bestMoves.push_back(contender);
					if (std::find(reasons.cbegin(), reasons.cend(), result.reason) == reasons.cend()) {
						reasons.push_back(result.reason);
					}
				} else {
					if (std::find(reasons.cbegin(), reasons.cend(), result.reason) == reasons.cend()) {
						reasons.push_back(result.reason);
					}
				}
			}
		}
	}

	std::cout << "Chosen move(s): ";
	for (const Move &bestMove : bestMoves) {
		std::cout << bestMove.ply.from << bestMove.ply.to << ", ";
	}
	std::cout << "\nOver move(s): ";
	for (const Ply &ply: secondBestPlys) {
		std::cout << ply.from << ply.to << ", ";
	}
	std::cout << "\nReasons: ";
	for (const Reason &reason : reasons) {
		std::cout << reason << ", ";
	}
	std::cout << "\n";

	return bestMoves.empty() ? Ply::None : bestMoves[rand() % bestMoves.size()].ply;
}