#include "stdafx.h"
#include "ChestPair.h"

namespace ChestPair
{
	bool chooseByPriority(const CandidateDecision &firstDecision, const CandidateDecision &secondDecision, bool &outChooseFirst)
	{
		CandidateCheck checks[] =
		{
			{ firstDecision.prefersThisChest, secondDecision.prefersThisChest, false },
			{ firstDecision.committedElsewhere, secondDecision.committedElsewhere, true },
			{ firstDecision.matchesFacingData, secondDecision.matchesFacingData, false },
			{ firstDecision.available, secondDecision.available, false },
		};

		for (const CandidateCheck &check : checks)
		{
			if (check.first == check.second) continue;

			outChooseFirst = check.invertFirst ? !check.first : check.first;
			return true;
		}

		return false;
	}
}