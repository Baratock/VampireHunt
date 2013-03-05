/*-- Schlafen --*/

#strict 2

public func IsAbility()
{
	return true;
}

public func GetSpellCombo()
{
	return "332";
}

public func CanUse(object pCaller)
{
	if (pCaller->GetVal(cSleep) < pCaller->GetMaxVal(cSleep) - 30)
		if (pCaller->GetVal(cPain) < pCaller->GetMaxVal(cPain) - 30 && pCaller->GetAction() == "Dead")
			return 1;
	if (GetID(Contained(pCaller)) != SELE && pCaller->GetAction() == "Walk")
		return 1;
}

public func Activate(object pCaller, bool fForce)
{
	if (pCaller->GetAction() == "Dead")
	{
		if (fForce)
			PlayerMessage(GetOwner(pCaller), "$VHWhat$", pCaller);
		Sound("heartbeat", false, pCaller, 0, GetOwner(pCaller) + 1, -1);
		pCaller->SetAction("FlatUp");
	}
	else
	{
		if (pCaller->GetVal(cPain))
		{
			var volume = pCaller->GetVal(cPain);
			if (volume > 100)
				volume = 100;
			Sound("heartbeat", false, pCaller, volume, GetOwner(pCaller) + 1, 1);
		}
		if (!pCaller->GetVal(cPain) && pCaller->GetVal(cSleep))
			PlayerMessage(GetOwner(pCaller), "$VHSleep$", pCaller);
		if (pCaller->GetVal(cPain) > pCaller->GetMaxVal(cPain))
		{
			PlayerMessage(GetOwner(pCaller), "Argh...", pCaller);
			if (pCaller->IsGhulForm() && pCaller->GetRole() == cRoleGhul)
				M_SS->Activate(pCaller);
		}
		pCaller->SetAction("Dead");
	}
}
