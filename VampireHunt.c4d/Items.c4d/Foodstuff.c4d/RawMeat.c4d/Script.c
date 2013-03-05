/*-- Fleisch --*/

#strict 2

public func IDCooked()
{
	return MEAC;
}
public func GetFeedValue()
{
	return 50;
}
public func CanBeCooked()
{
	return true;
}

protected func Hit()
{
	Sound("WoodHit*");
}

public func Eat(object pEater)
{
	if (pEater)
	{
		pEater->~Feed(GetFeedValue());
	}
	RemoveObject(this);
}

public func Activate(object pClonk)
{
	[$TxtEat$]
	Eat(pClonk);
	return 1;
}

// Kochen
public func Cook()
{
	if (!CanBeCooked())
		return false;
	ChangeDef(IDCooked());
	SetAlive(false, this);
	return 1;
}
