/*-- Bayonett --*/

#strict 2

public func IsAbility()
{
	return true;
}

public func GetSpellCombo()
{
	return "225";
}

public func CanUse(object pCaller)
{
	if (pCaller->GetRole() == J_PR && GetMagicEnergy(pCaller) >= 5 && !ContentsCount(BAYO, pCaller))
		return true;
}

public func Activate(object pCaller) //Blutsaugen
{
	CreateContents(BAYO, pCaller, 2);
	DoMagicEnergy(-10, pCaller);
	Sound("SwordHit*", false, pCaller);
}

