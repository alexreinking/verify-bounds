#include "Bound.h"

std::string Bound::ToString(z3::model &m) {
    if (type == BoundType::Unbounded) {
        return "_";
    } else {
        return m.eval(*expr).to_string();
    }
}

std::string Bound::ToStringSymbolic(bool print) {
    if ((type == BoundType::Unbounded && !print) || !expr) {
        return "_";
    }
    switch(restriction) {
        case Restriction::None: {
            return expr->to_string();
        }
        case Restriction::NonPositive: {
            return expr->to_string() + " <= 0";
        }
        case Restriction::NonNegative: {
            return expr->to_string() + " >= 0";
        }
        default: {
            std::cerr << "Could not identify restriction in Bound::ToStringSymbolic()!" << std::endl;
            return "IDK";
        }
    }
}

void apply_bound(z3::solver &solver, z3::expr &variable, Bound *bound) {
    switch(bound->type) {
        case BoundType::Unbounded: {
            return;
        }
        case BoundType::UpperBound: {
            solver.add(variable <= *bound->expr);
            return;
        }
        case BoundType::LowerBound: {
            solver.add(variable >= *bound->expr);
            return;
        }
        default: {
            std::cerr << "Could not identify BoundType in add_bound()!" << std::endl;
        }
    }
}

void apply_restriction(z3::solver &solver, Bound *bound) {
    switch(bound->restriction) {
        case Restriction::None: {
            return;
        }
        case Restriction::NonPositive: {
            solver.add(*bound->expr <= 0);
            return;
        }
        case Restriction::NonNegative: {
            solver.add(*bound->expr >= 0);
            return;
        }
        default: {
            std::cerr << "Could not identify restriction in apply_restriction()!" << std::endl;
        }
    }
}