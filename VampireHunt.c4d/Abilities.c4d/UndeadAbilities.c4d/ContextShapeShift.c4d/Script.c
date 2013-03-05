/*-- Verwandlung --*/

#strict 2

public func IsAbility()
{
	return true;
}

public func GetSpellCombo()
{
	return "315";
}

public func CanUse(object pCaller)
{
	if (pCaller->GetRole() == cRoleGhul)
		return true;
}

public func Activate(object pCaller)
{
	if (!pCaller->IsGhulForm())
	{
		pCaller->SetGhulForm(true);
		var idOldRole = pCaller->GetOldRole();
		Sound("Ghoul", false, pCaller);
		SetGraphics("", pCaller, pCaller->GetRole()->Graphics());
		
	}
	else
	{
		pCaller->SetGhulForm(false);
		Sound("DeGhoul", false, pCaller);
		SetGraphics("", pCaller, idOldRole->Graphics());
	}
	return 1;
}
