/*-- Wächter --*/

#strict 2

#include J_SM

public func OnlyOne()
{
	return true;
}
public func Health()
{
	return 120;
}
public func Pain()
{
	return 150;
} 
public func ManaType()
{
	return cMagicWill;
}
public func Mana()
{
	return 5;
}
public func Home()
{
	return CST2;
}
public func Things()
{
	return [BOW1, FARP, FARP, SPER, SWOR, SHIE, ARMR];
}
public func Knowledge()
{
	return [ARWP, STAK, FARP];
}
public func Graphics()
{
	return WTER;
}
public func Use()
{
	return [SHIE, ARMR];
}

public func Support(object pClonk)
{
	Init(pClonk);
	//ein modisches Schild mit Rüstung :>
	Sound("Vernichten wir", false, pClonk);
	SetGraphics(0, pClonk, WTER);
	FindObject2(Find_ID(SHIE), Find_Container(pClonk))->~Activate(pClonk);
	FindObject2(Find_ID(ARMR), Find_Container(pClonk))->~Activate(pClonk);
	PlayerMessage(GetOwner(pClonk), "$Guard$");
}
