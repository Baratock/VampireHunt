/*-- Gitfflasche --*/

#strict 2

protected func Hit()
{
	Sound("Crystal*");
}

protected func Activate(object pClonk)
{
	//Sound
	Sound("Drink");
	//Menschen bekommt Gitf sch... nicht sehr gut
	if (pClonk->GetVal(cForm) == cRoleHuman)
	{
		DoEnergy(-25, pClonk);
		pClonk->DoPain(40);
	}
	else
	{
		DoEnergy(50, pClonk);
		pClonk->DoVal(cPain, -50);
		pClonk->DoVal(cSleep, -30);
	}
	//Action
	pClonk->~Drink(this);
	RemoveObject(this);
}

//Imprägnierung, erhöht Empfindlichkeit gegenüber Schaden
public func Affect(object pTarget, object pPoision)
{
	if (pPoision->~IsArrow() || pPoision->~IsBolt())
		AddEffect("PoisionFern", pTarget, 1, 37 * 4, 0, GetID(this));
	else
		AddEffect("PoisionNah", pTarget, 1, 37 * 7, 0, GetID(this));
}

protected func FxPoisionNahTimer()
{
	return -1;
}
protected func FxPoisionFernTimer()
{
	return -1;
}

public func IsPoision()
{
	return true;
}
public func Color()
{
	return RGBa(100, 255, 100);
}
