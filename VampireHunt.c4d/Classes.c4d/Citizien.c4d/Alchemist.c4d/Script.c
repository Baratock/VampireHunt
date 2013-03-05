/*-- Alchemist --*/
 
#strict 2

#include J_SM

public func OnlyOne()
{
	return true;
}
public func Health()
{
	return 90;
}
public func Pain()
{
	return 90;
} 
public func ManaType()
{
	return cMagicWill;
}
public func Mana()
{
	return 10;
}
public func Requirements()
{
	return HUT3;
}
public func Things()
{
	return [WDST, HEPT, HEPT, BNDG, CHRM];
}
public func Knowledge()
{
	return [WDST, HEPT, BNDG];
}
public func Graphics()
{
	return ALCH;
}

public func Support(object pClonk)
{
	Init(pClonk);
	SetGraphics(0, pClonk, ALCH);
}


public func IsCitizien()
{
	return false;
}
