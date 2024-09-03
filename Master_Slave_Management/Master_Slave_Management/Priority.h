enum class Priority
{
	Low,        // Low priority - tasks that are not urgent and can be addressed later.
	MediumLow,  // Medium-Low priority - tasks that are less urgent than medium priority tasks.
	Medium,     // Medium priority - tasks that are somewhat urgent but not the highest priority.
	MediumHigh, // Medium-High priority - tasks that are more urgent and require timely attention.
	High        // High priority - tasks that are the most urgent and require immediate attention.
};

int getPriorityIndex(Priority priority) {
	switch (priority) {
	case Priority::Low:        return 0;
	case Priority::MediumLow:  return 1;
	case Priority::Medium:     return 2;
	case Priority::MediumHigh: return 3;
	case Priority::High:       return 4;
	default:                   return -1;
	}
}