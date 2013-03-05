/*-- Förster --*/
 
#strict 2

#include J_SM

public func Health()
{
	return 150;
}
public func Pain()
{
	return 100;
} 
public func ManaType()
{
	return cMagicWill;
}
public func Mana()
{
	return 15;
}
public func Home()
{
	return HUT2;
}
public func Things()
{
	return [BOW1, ARWP, ARWP, SPER];
}
public func Knowledge()
{
	return [PBAL, SPER, ARWP, UROI, URNT];
}
public func Graphics()
{
	return FORS;
}

public func Support(object pClonk)
{
	Init(pClonk);
	Sound("Ich werde ihn", false, pClonk);
	SetGraphics(0, pClonk, FORS);
	PlayerMessage(GetOwner(pClonk), "$Foster$");
}
