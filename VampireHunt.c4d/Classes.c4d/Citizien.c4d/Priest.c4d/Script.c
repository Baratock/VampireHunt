/*-- Priester --*/

#strict 2

#include J_SM

public func OnlyOne()
{
	return true;
}
public func Health()
{
	return 100;
}
public func Pain()
{
	return 80;
} 
public func ManaType()
{
	return cMagicForce;
}
public func Mana()
{
	return 50;
}
public func Home()
{
	return CHUR;
}
public func Things()
{
	return [STAK, BAYO, BAYO];
}
public func Knowledge()
{
	return [STAK, PBAL, PELF, PR_B, PR_W, PR_G, PRSP];
}
public func Graphics()
{
	return PRIS;
}

public func Support(object pClonk)
{
	Init(pClonk);
	Sound("mein glaube", false, pClonk);
	SetGraphics(0, pClonk, PRIS);
	PlayerMessage(GetOwner(pClonk), "$Priest$");
}

//Regeneration:Heilige Orte
public func Magic(object pClonk)
{
	if (pClonk->GetVal(cForm) == cRoleHuman && Contained(pClonk))
		if (Contained(pClonk)->~IsHolyPlace())
			DoMagicEnergy(1, pClonk);
}
