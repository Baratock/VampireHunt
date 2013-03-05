/*-- fressen --*/

#strict 2

public func IsAbility()
{
	return true;
}

public func GetSpellCombo()
{
	return "141";
}

public func CanUse(object pCaller)
{
	if (pCaller->SleepingClonk() && pCaller->GetRole() == cRoleGhul)
		return 1;
}

public func Activate(object pCaller)
{
	var pVictim = pCaller->SleepingClonk();
	if (pCaller->GetRole() == cRoleVampire)
	{
		PlayerMessage(GetOwner(pCaller), "$VHCrazy$");
	}
	else
	{
		pCaller->DoVal(cPain, -20);
		pCaller->DoVal(cHunger, -140);
		pCaller->Feed(20);
		pCaller->DoEnergy(15);
		PlayerMessage(GetOwner(pCaller), "$VHFeelBetter$");
		PlayerMessage(GetOwner(pCaller), "$VHMoreMeat$", pCaller, GetPlayerName(GetOwner(pVictim)));
		pVictim->Death(pCaller->Weapon(), CP_1);
	}
}



