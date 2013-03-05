/*-- Sabre -*/

#strict 2
#include SWOR

protected func Hit()
{
	BounceOff();
	Sound("MetalHit*");
	return 1;
}

public func StrikeSound()
{
	return Sound("SabreHit*");
}
public func GetStrikeDamage()
{
	return 3;
}
public func GetBloodLoss()
{
	return 2;
}
public func GetStrikeTime()
{
	//1 + 1/3
	return 1 + Max(Random(3) - 1, 0);
}


