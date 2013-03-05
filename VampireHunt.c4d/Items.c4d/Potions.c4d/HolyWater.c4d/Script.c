/*-- Heiliges Wasser --*/

#strict 2

protected func Initialize()
{
	SetClrModulation(RGBa(200, 200, 200, 90));
}

protected func Hit()
{
	Sound("Crystal*");
}

protected func Activate(object pClonk)
{
	// Sound
	Sound("Drink");
	// Gesund für Menschen
	if (pClonk->GetVal(cForm) == cRoleHuman)
	{
		DoEnergy(25, pClonk);
		DoMagicEnergy(5, pClonk, true);
	}
	else
	{
		DoEnergy(-9, pClonk, false, FX_Call_EngFire);
		pClonk->DoPain(30);
	}
	//Action
	pClonk->~Drink(this);
	RemoveObject(this);
	return 1;
}

//Imprägnierung, macht extra Schaden an allem Untoten 
public func Affect(object pTarget)
{
	if (pTarget->GetVal(cForm) != cRoleHuman)
	{
		pTarget->DoPain(12);
	}
}

public func IsPoision()
{
	return true;
}
public func Color()
{
	return RGBa(230, 230, 230);
}
