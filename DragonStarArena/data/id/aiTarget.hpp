// ================================================================
//
// aiTarget.hpp
// 
// List of conditions required for a tactic to select a target.
//
// ================================================================

#pragma once

enum class AITarget {
	Undefined = 0,
	Any,
	HPPercentLessThan,
	HPPercentGreaterThan
};