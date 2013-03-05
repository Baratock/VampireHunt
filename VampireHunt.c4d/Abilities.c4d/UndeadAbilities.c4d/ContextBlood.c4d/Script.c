/*-- Blutflasche --*/

#strict 2

public func IsAbility()
{
	return true;
}

public func GetSpellCombo()
{
	return "165";
}

public func CanUse(object pCaller)
{
	if (pCaller->GetVal(cBlood) > 30)
		if (pCaller->GetRole() == cRoleVampire || pCaller->GetRole() == cRoleLich)
			return true;
}

public func Activate(object pCaller)
{
	pCaller->DoVal(cBlood, -25);
	pCaller->LoseBlood(3);
	CreateContents(PBLD, pCaller);
	PlayerMessage(GetOwner(pCaller), "$VHFillBlood$", pCaller);
}


