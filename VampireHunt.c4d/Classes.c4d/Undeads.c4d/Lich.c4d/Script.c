/*-- Lich --*/

#strict 2

#include J_VA

public func Blood()
{
	return 300;
}
public func Pain()
{
	return 150;
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
	return LICH;
}

public func Support(object pMaster, object pClonk)
{
	Init(pClonk);
	SetGraphics("", pClonk, LICH);
	DoMagicEnergy(-100, pClonk, true);
	//Das Blut wirkt
	//DoEnergy(75, pClonk);
	pClonk->SetVal(cPain, 0);
	pClonk->SetVal(cSleep, 0);
	M_SL->Activate(pClonk);
	Sound("BecomeLich", false, pClonk);
	//Message
	PlayerMessage(GetOwner(pClonk), "$VHNewLife$", 0, GetPlayerName(GetOwner(pMaster)));
}
