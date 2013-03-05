/*-- Reiner Wille --*/

#strict 2

public func IsAbility()
{
	return true;
}

public func GetSpellCombo()
{
	return "365";
}

public func CanUse(object pCaller)
{
	if (pCaller->GetRole()->~ManaType() != cMagicWill || GetEffect("Power*", pCaller))
		return false;
	if (GetAction(pCaller) == "Dead" && GetMagicEnergy(pCaller))
	{
		if (pCaller->GetVal(cSleep) > pCaller->GetMaxVal(cSleep) * 2 / 3 || pCaller->GetVal(cPain) > pCaller->GetMaxVal(cPain) * 2 / 3 || pCaller->GetVal(cBlood) < 50)
		{
			return true;
		}
	}
}

public func Activate(object pCaller)
{
	var r = Random(2);
	if (r == 0)
		PlayerMessage(GetOwner(pCaller), "$VHDontGiveUp1$", pCaller);
	if (r == 1)
		PlayerMessage(GetOwner(pCaller), "$VHDontGiveUp2$", pCaller);
	if (r == 2)
		PlayerMessage(GetOwner(pCaller), "$VHDontGiveUp3$", pCaller);
	M_SL->Activate(pCaller);
	AddEffect("PowerWill", pCaller, 300, 30, 0, GetID(this));
}

protected func FxPowerWillStart(object pTarget, int iEffectNumber, int iTemp)
{
	//alle negativen Effekte beseitigen :)
	EffectVar(0, pTarget, iEffectNumber) = pTarget->GetVal(cPain);
	EffectVar(1, pTarget, iEffectNumber) = pTarget->GetVal(cSleep);
	EffectVar(2, pTarget, iEffectNumber) = pTarget->GetVal(cHunger);
	EffectVar(3, pTarget, iEffectNumber) = pTarget->GetVal(cBlood);
	pTarget->SetVal(cPain, 0);
	pTarget->SetVal(cSleep, 0);
	pTarget->SetVal(cHunger, 0);
	pTarget->SetVal(cBlood, pTarget->GetMaxVal(cBlood));
}

protected func FxPowerWillTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	if (!GetMagicEnergy(pTarget))
	{
		//effekte zurücksetzen
		pTarget->DoVal(cPain, EffectVar(0, pTarget, iEffectNumber));
		pTarget->DoVal(cSleep, EffectVar(1, pTarget, iEffectNumber));
		pTarget->DoVal(cHunger, EffectVar(2, pTarget, iEffectNumber));
		pTarget->DoVal(cBlood, EffectVar(3, pTarget, iEffectNumber) - pTarget->GetMaxVal(cBlood));
		return -1;
	}
	DoMagicEnergy(-Random(2), pTarget);
	return true;
}

