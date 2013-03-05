/*-- Urticanettle --*/

#strict 2

#include ELRT

public func Construction()
{
	SetAction("Exist");
	SetPosition(GetX(this), GetY(this) - 5);
}

protected func Eat(object pClonk)
{
	var x = GetEffectCount("IntAteNettle", pClonk);
	pClonk->~Feed(6);
	pClonk->~Pain(15 * 2 ** x);
	DoEnergy(25 / 2 ** (x / 2), pClonk);
	AddEffect("IntAteNettle", pClonk, 1, 37 * 5, 0, GetID(this));
	RemoveObject(this);
	return 1;
}

protected func FxAteNettleTimer(object pTarget, int iNumber, int iTime)
{
	return -1;
}