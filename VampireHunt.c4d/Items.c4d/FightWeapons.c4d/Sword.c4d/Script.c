/*--- Schwert ---*/

#strict 2
#include AXE1

protected func Hit()
{
	BounceOff();
	Sound("SwordHit*");
	return 1;
}

// Schlagaktion
public func GetStrikeAction()
{
	if (Random(2))
		return "SwordThrust";
	else
		return "SwordStrike";
}

public func GetFightAction()
{
	return "SwordFight";
}
public func StrikeSound()
{
	return Sound("SwordHit*");
}
public func GetStrikeDamage()
{
	return 4;
}
public func GetStrikeTime()
{
	return 2;
}
public func GetBloodLoss()
{
	return 2;
}
public func GetRevaluationCombo()
{
	return KNIG;
}
