/*--- Reiﬂklinge ---*/

#strict 2
#include SWOR

public func StrikeSound()
{
	return Sound("SwordHitp*");
}
public func GetStrikeDamage()
{
	return 6;
}
public func GetStrikeTime()
{
	return 2;
}
public func GetBloodLoss()
{
	return 2;
}
