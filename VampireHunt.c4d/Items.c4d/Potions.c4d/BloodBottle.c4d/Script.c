/*-- Blutflasche --*/

#strict 2

protected func Initialize()
{
	SetClrModulation(RGB(255, 127, 127));
}

protected func Hit()
{
	Sound("Crystal*");
}

protected func Activate(object pClonk)
{
	// Sound
	Sound("Drink");
	// ungesund für Menschen
	if (pClonk->GetVal(cForm) == cRoleHuman)
	{
		DoEnergy(-3, pClonk);
		pClonk->DoPain(30);
	}
	//gut für ghuls
	if (pClonk->GetRole() == cRoleGhul)
	{
		pClonk->Feed(50);
	}
	// Vampire und Lichs bekommem mehr Blut
	if (pClonk->GetVal(cForm) == cRoleVampire || pClonk->GetVal(cForm) == cRoleLich)
	{
		pClonk->Feed(25, true);
	}
	//Action
	pClonk->~Drink(this);
	RemoveObject(this);
	return 1;
}

//Imprägnierung, extra Schmerzen für Menschen
public func Affect(object pTarget)
{
	if (pTarget->GetVal(cForm) == cRoleHuman)
	{
		pTarget->DoPain(10);
	}
}

public func IsPoision()
{
	return true;
}
public func Color()
{
	return RGBa(230, 127, 127);
}
