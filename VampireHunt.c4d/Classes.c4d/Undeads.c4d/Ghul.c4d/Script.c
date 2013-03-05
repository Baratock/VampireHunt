/*-- Ghul --*/

#strict 2

#include J_VA

public func Blood()
{
	return 100;
} 
public func Pain()
{
	return 180;
}
public func Things()
{
	return [];
}
public func Knowledge()
{
	return [];
}
public func Graphics()
{
	return GHUL;
}

public func Support(object pMaster, object pClonk)
{
	SetGraphics("", pClonk, GHUL);
	DoMagicEnergy(-100, pClonk, true);
	//Das Blut wirkt
	DoEnergy(75, pClonk);
	pClonk->SetVal(cPain, 0);
	pClonk->SetVal(cSleep, 0);
	M_SL->Activate(pClonk);
	Sound("meiter", false, pClonk);
	//Message
	PlayerMessage(GetOwner(pClonk), "$VHNewLife$", 0, GetPlayerName(GetOwner(pMaster)));
}
