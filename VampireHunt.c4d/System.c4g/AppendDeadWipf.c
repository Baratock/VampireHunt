#strict 2

#appendto DWPF

public func Eat(pBy){}

public func Activate(object pClonk)
{
	if (pClonk->GetVal(cForm) == cRoleVampire || pClonk->GetVal(cForm) == cRoleLich)
	{
		pClonk->Feed(25, true);
	}
	else
	{
		CreateContents(BONE, pClonk, 1);
		if (pClonk->GetRole() == J_HN)
			CreateContents(RMET, pClonk, 3);
		else
			CreateContents(RMET, pClonk, 2);
	}
	return RemoveObject(this);
}