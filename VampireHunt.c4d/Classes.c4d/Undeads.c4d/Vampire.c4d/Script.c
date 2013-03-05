/*-- Vampir --*/

#strict 2

#include J_SM

public func IsCitizien()
{
	return false;
}
public func Health()
{
	return 180;
}
public func Pain()
{
	return 180;
}
public func ManaType()
{
	return cMagicBlood;
}
public func Mana()
{
	return Blood();
}
public func Blood()
{
	return 200;
}
public func Home()
{
	return BED1;
}
public func Things()
{
	return [TBLD, BOW1, ARWP];
}
public func Knowledge()
{
	return [BARP, ARWP, TBLD];
}
public func Graphics()
{
	return VAMP;
}

public func Support(object pClonk)
{
	Init(pClonk);
	DoEnergy(100, pClonk);
	SetWealth(GetOwner(pClonk), 2);
	Sound("euer tod4", false, pClonk);
	SetGraphics(0, pClonk, VAMP);
	PlayerMessage(GetOwner(pClonk), "$Vamp$");
}

//Blutmengenberechnung
public func Magic(object pClonk)
{
	var iChange = pClonk->GetVal(cBlood) - GetMagicEnergy(pClonk);
	DoMagicEnergy(iChange, pClonk);
	return 1;
}
