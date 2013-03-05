/*-- Unsichtbar --*/

#strict 2

public func IsAbility()
{
	return true;
}

public func GetSpellCombo()
{
	return "355";
}

public func CanUse(object pCaller)
{
	if (pCaller->GetRole() == cRoleVampire && pCaller->GetVal(cBlood) > 50)
		if (!GetEffect("Power*", pCaller) || GetEffect("PowerUnVis", pCaller))
			return true;
}

public func Activate(object pCaller)
{
	if (GetEffect("PowerUnVis", pCaller))
	{
		RemoveEffect("PowerUnVis", pCaller);
	}
	else
	{
		AddEffect("PowerUnVis", pCaller, 12, 0, 0, GetID(this));
	}
}

protected func FxPowerUnVisStart(object pTarget, int iEffectNumber)
{
	SetClrModulation(RGBa(255, 255, 255, 240), pTarget);
}

protected func FxPowerUnVisStop(object pTarget, int iEffectNumber)
{
	SetClrModulation(RGBa(255, 255, 255, 0), pTarget);
} 