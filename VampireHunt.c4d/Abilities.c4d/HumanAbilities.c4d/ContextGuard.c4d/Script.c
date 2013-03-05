/*-- Wachen --*/

#strict 2

public func IsAbility()
{
	return true;
}

public func GetSpellCombo()
{
	return "515";
}

public func CanUse(object pCaller)
{
	//Nutzt eine andere Fertigkeit als diese
	if (!GetEffect("Power*", pCaller) || GetEffect("PowerGuard", pCaller))
		if (pCaller->GetRole() == J_GD && GetAction(pCaller) == "Walk")
			return true;
	return false;
}

public func Activate(object pCaller)
{
	if (GetEffect("PowerGuard", pCaller))
	{
		RemoveEffect("PowerGuard", pCaller);
	}
	else
	{
		pCaller->DoVal(cViewRange, 200);
		pCaller->DoVal(cSlowDown, -20);
		AddEffect("PowerGuard", pCaller, 102, 30, 0, GetID(this));
	}
	return 1;
}

protected func FxPowerGuardTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	if (GetAction(pTarget) == "Death" || GetEffectCount("Power*", pTarget) > 1 || pTarget->GetVal(cForm) != cRoleHuman)
		return -1;
	return 1;
}

protected func FxPowerGuardStop(object pTarget)
{
	pTarget->DoVal(cSlowDown, 20);
	pTarget->DoVal(cViewRange, -200);
}