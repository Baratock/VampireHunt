/*-- Bolzen --*/

#strict

#include ARRW

/* Paketeigenschaften */
public func PackTo()
{
	return BOTP;
}
public func IsBolt()
{
	return 1;
}
public func IsArrow()
{
	return 0;
}
public func GetCalcedPain()
{
	return 30;
}

public func BoltStrength()
{
	return 12 + Random(6);
}

private func InFlight()
{
	inherited();
	SetR(Angle(0, 0, GetXDir(), GetYDir()));
}

public func HitEffect(object pTarget) //Direkte Wirkung beim Treffer
{
	DoEnergy(-BoltStrength(), pTarget);
	pTarget->~Pain(GetCalcedPain());
	if (GetID(pTarget) == WIPF)
	{
		pTarget->Kill();
	}
	UsePoision(this, poision, pTarget);
	RemoveObject(this);
}

