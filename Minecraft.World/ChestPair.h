#pragma once

namespace ChestPair
{
	struct CandidateRef
	{
		bool exists;
		int candidateX;
		int candidateZ;
	};

	struct CandidateDecision
	{
		bool prefersThisChest;
		bool committedElsewhere;
		bool matchesFacingData;
		bool available;
	};

	struct CandidateCheck
	{
		bool first;
		bool second;
		bool invertFirst;
	};

	struct PreferredNeighborInput
	{
		int firstX;
		int firstZ;
		int secondX;
		int secondZ;
		bool hasFirst;
		bool hasSecond;
		bool hasPlacementIntent;
		int intendedPartnerX;
		int intendedPartnerY;
		int intendedPartnerZ;
		int selfY;
		bool strictPlacementIntent;
		bool hasCachedPartner;
		int cachedPartnerX;
		int cachedPartnerZ;
		bool rightIsFirst;
	};

	bool chooseByPriority(const CandidateDecision &firstDecision, const CandidateDecision &secondDecision, bool &outChooseFirst);

	template <typename IsCommittedElsewhereFn, typename BuildCandidateDecisionFn>
	bool choosePreferredNeighbor(
		const PreferredNeighborInput &input,
		IsCommittedElsewhereFn &&isCommittedElsewhere,
		BuildCandidateDecisionFn &&buildCandidateDecision,
		int &outX,
		int &outZ)
	{
		CandidateRef candidates[2] =
		{
			{ input.hasFirst, input.firstX, input.firstZ },
			{ input.hasSecond, input.secondX, input.secondZ },
		};

		int candidateCount = 0;
		const CandidateRef *onlyCandidate = nullptr;
		for (const CandidateRef &candidate : candidates)
		{
			if (!candidate.exists) continue;
			candidateCount++;
			onlyCandidate = &candidate;
		}

		if (candidateCount == 0) return false;

		if (input.hasPlacementIntent && input.strictPlacementIntent)
		{
			for (const CandidateRef &candidate : candidates)
			{
				if (!candidate.exists) continue;
				if (input.intendedPartnerX != candidate.candidateX || input.intendedPartnerY != input.selfY || input.intendedPartnerZ != candidate.candidateZ) continue;

				if (isCommittedElsewhere(candidate.candidateX, candidate.candidateZ))
				{
					return false;
				}

				outX = candidate.candidateX;
				outZ = candidate.candidateZ;
				return true;
			}

			return false;
		}

		if (input.hasCachedPartner)
		{
			for (const CandidateRef &candidate : candidates)
			{
				if (!candidate.exists) continue;
				if (input.cachedPartnerX != candidate.candidateX || input.cachedPartnerZ != candidate.candidateZ) continue;

				outX = candidate.candidateX;
				outZ = candidate.candidateZ;
				return true;
			}
		}

		if (candidateCount == 1 && onlyCandidate != nullptr)
		{
			if (isCommittedElsewhere(onlyCandidate->candidateX, onlyCandidate->candidateZ)) return false;
			outX = onlyCandidate->candidateX;
			outZ = onlyCandidate->candidateZ;
			return true;
		}

		CandidateDecision firstDecision = buildCandidateDecision(input.firstX, input.firstZ);
		CandidateDecision secondDecision = buildCandidateDecision(input.secondX, input.secondZ);

		bool chooseFirstCandidate = false;
		if (chooseByPriority(firstDecision, secondDecision, chooseFirstCandidate))
		{
			outX = chooseFirstCandidate ? input.firstX : input.secondX;
			outZ = chooseFirstCandidate ? input.firstZ : input.secondZ;
			return true;
		}

		outX = input.rightIsFirst ? input.firstX : input.secondX;
		outZ = input.rightIsFirst ? input.firstZ : input.secondZ;
		return true;
	}
}