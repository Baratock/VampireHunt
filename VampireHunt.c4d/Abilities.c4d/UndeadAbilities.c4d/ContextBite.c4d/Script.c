/*-- Beißen --*/

#strict 2

public func IsAbility()
{
	return true;
}

public func GetSpellCombo()
{
	return "623";
}

public func CanUse(object pCaller)
{
	if (pCaller->SleepingClonk() && pCaller->GetVal(cBlood) < pCaller->GetMaxVal(cBlood) && pCaller->GetRole() == cRoleVampire)
		if (pCaller->SleepingClonk()->GetVal(cBlood))
			return true;
}

 //Blutsaugen
public func Activate(object pCaller)
{
	var pVictim = pCaller->SleepingClonk();
	var iNeed = pCaller->GetMaxVal(cBlood) - pCaller->GetVal(cBlood);
	if (pVictim->GetVal(cBlood) < iNeed)
		iNeed = pVictim->GetVal(cBlood);
	if (iNeed > 50)
		iNeed = 50;
	PlayerMessage(GetOwner(pCaller), "$VHDrinkBlood$", pCaller, iNeed, GetPlayerName(GetOwner(pVictim)));
	pVictim->DoVal(cBlood, -iNeed);
	pCaller->Feed(iNeed, true);
}