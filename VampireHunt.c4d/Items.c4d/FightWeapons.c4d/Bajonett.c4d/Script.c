/*--- Bajonett ---*/

#strict 2
#include SWOR

local ialpha;

///---Callbacks---///

public func CarryLimit()
{
	return -1;
}
public func GetStrikeTime()
{
	return 1;
}
public func GetStrikeDamage()
{
	return 1 + Random(2);
}

///---Timercall---///

protected func CheckRemoval()
{
	if (Contained(this) && !(GetOCF(Contained(this)) & OCF_Living))
	{
		RemoveObject(this);
	}
	if (Contained(this))
	{
		return 1;
	}
	ialpha += 3;
	SetRGBaValue(GetColorDw(this), ialpha * 2, 0);
	if (ialpha > 65)
	{
		RemoveObject(this);
		return 1;
	}
}

protected func Hit()
{
	Sound("MetalHit*");
	SetRDir(0, this);
	SetXDir(0, this);
	SetYDir(0, this);
	return 1;
}

private func Throw(int iDir)
{
	//up
	SetPosition(GetX(this) + 10 * iDir, GetY(this) + 6);
	//sound
	Sound("Catapult");
	//faster throw
	SetXDir(GetXDir(this) * 4);
	//horizontal
	SetYDir(-10);
	//roation
	SetRDir(GetXDir(this));
}

public func HitEffect(object pClonk)
{
	DoEnergy(-8, pClonk);
	//target is still living?
	if (!pClonk)
		return 1;
	UsePoision(this, poision, pClonk);
	Hit();
	return 1;
}

public func BounceOff(){}


