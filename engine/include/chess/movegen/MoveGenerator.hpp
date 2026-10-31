#pragma once

#include <vector>

#include "core/Position.hpp"
#include "core/Move.hpp"

class MoveGenerator {
public:
    std::vector<Move> generatePseudoLegalMoves(
        const Position& position,
        int from
    ) const;
};
