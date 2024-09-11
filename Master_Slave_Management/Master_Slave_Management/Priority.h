#pragma once
enum class Priority
{
    Low,        // Low priority - tasks that are not urgent and can be addressed later.
    MediumLow,  // Medium-Low priority - tasks that are less urgent than medium priority tasks.
    Medium,     // Medium priority - tasks that are somewhat urgent but not the highest priority.
    MediumHigh, // Medium-High priority - tasks that are more urgent and require timely attention.
    High        // High priority - tasks that are the most urgent and require immediate attention.
};
