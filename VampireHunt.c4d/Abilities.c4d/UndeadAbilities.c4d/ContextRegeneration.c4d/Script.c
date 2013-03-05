/*-- Regeneration --*/

#strict 2

public func IsAbility()
{
	return true;
}

public func GetSpellCombo()
{
	return "223";
}

public func CanUse(object pCaller)
{
	if (pCaller->GetRole() == cRoleLich || pCaller->GetRole() == cRoleVampire)
		if (!GetEffect("Power*", pCaller) && pCaller->GetVal(cBlood) > 15 && GetEnergy(pCaller) < pCaller->GetMaxVal(cHealth))
			return true;
	if (GetEffect("PowerRegeneration", pCaller))
		return true;
}

public func Activate(object pCaller)
{
	if (GetEffect("PowerRegeneration", pCaller))
		RemoveEffect("PowerRegeneration", pCaller);
	else
		AddEffect("PowerRegeneration", pCaller, 21, 50, 0, GetID(this));
}

protected func FxPowerRegenerationTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	if (pTarget->GetVal(cBlood) < 10 && GetEnergy(pTarget) == pTarget->GetMaxVal(cHealth))
	{
		return -1;
	}
	DoEnergy(2, pTarget);
	pTarget->DoVal(cBlood, -1);
}