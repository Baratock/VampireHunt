/*-- Jäger --*/
 
#strict 2

#include J_SM

public func OnlyOne()
{
	return true;
}
public func Health()
{
	return 110;
}
public func Pain()
{
	return 120;
}
public func Mana()
{
	return 20;
}
public func ManaType()
{
	return cMagicWill;
}
public func Home()
{
	return HUT2;
}
public func Things()
{
	return [CRBW, BOTP, DEHK, SHIE];
}
public func Knowledge()
{
	return [];
}
public func CanUseDash()
{
	return true;
}
public func Graphics()
{
	return HUNT;
}
public func Use()
{
	return [CRBW, SHIE];
}

public func Support(object pClonk)
{
	Init(pClonk);
	FindContents(SHIE, pClonk)->Activate(pClonk);
	SetGraphics(0, pClonk, HUNT);
}
