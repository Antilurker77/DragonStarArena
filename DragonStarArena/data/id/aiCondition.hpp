// ================================================================
//
// aiCondition.hpp
// 
// List of conditions required for a tactic to trigger.
//
// ================================================================

#pragma once

enum class AICondition {
	Undefined = 0,
	None,
	HPPercentLessThan,
	HPPercentGreaterThan
};