/*-- Sprinten --*/

#strict 2

public func IsAbility()
{
	return true;
}

public func GetSpellCombo()
{
	return "111";
}

public func CanUse(object pCaller)
{
	if (!pCaller->GetRole()->~CanUseDash())
		return false;
	if (pCaller->GetVal(cSleep) > pCaller->GetMaxVal(cSleep) * 2 / 3 || pCaller->GetVal(cPain) > pCaller->GetMaxVal(cPain) * 2 / 3 || pCaller->GetVal(cBlood) < 50)
		if (GetAction(pCaller) == "Walk" || GetAction(pCaller) == "Dead")
			if (GetMagicEnergy(pCaller) && !GetEffect("PowerDash", pCaller))
				return true;
}

public func Activate(object pCaller)
{
	//Aufwachen
	if (GetAction(pCaller) == "Dead")
		M_SL->Activate(pCaller, true);
	AddEffect("PowerDash", pCaller, 300, 20, 0, GetID(this));
}

protected func FxPowerDashStart(object pTarget, int iEffectNumber, int iTemp)
{
	//alle negativen Effekte beseitigen :)
	EffectVar(0, pTarget, iEffectNumber) = pTarget->GetVal(cPain);
	EffectVar(1, pTarget, iEffectNumber) = pTarget->GetVal(cSleep);
	EffectVar(2, pTarget, iEffectNumber) = pTarget->GetVal(cHunger);
	pTarget->SetVal(cPain, 0);
	pTarget->SetVal(cSleep, 0);
	pTarget->SetVal(cHunger, 0);
	pTarget->SetVal(cSlowDown, 140);
}

protected func FxPowerDashTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	if (!GetMagicEnergy(pTarget))
	{
		//wieder normale geschw.
		pTarget->SetVal(cSlowDown, 100);
		//effekte zurücksetzen
		pTarget->DoVal(cPain, EffectVar(0, pTarget, iEffectNumber));
		pTarget->DoVal(cSleep, EffectVar(1, pTarget, iEffectNumber));
		pTarget->DoVal(cHunger, EffectVar(2, pTarget, iEffectNumber));
		return -1;
	}
	DoMagicEnergy(-1, pTarget);
	return true;
}



