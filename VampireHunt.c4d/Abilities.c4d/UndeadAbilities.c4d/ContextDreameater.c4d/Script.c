/*-- Traum fressen --*/

#strict 2

public func IsAbility()
{
	return true;
}

public func GetSpellCombo()
{
	return "411";
}

public func CanUse(object pCaller)
{
	if (pCaller->GetRole() != cRoleLich)
		return false;
	if (pCaller->GetVal(cBlood) < 10 || !IsNight() || GetAction(pCaller) != "Walk")
		return false;
	var aClonks = FindObjects(Find_ID(MEDC), Find_Distance(100, GetX(pCaller), GetY(pCaller)), Find_Action("Dead"), Find_Exclude(pCaller));
	if (GetLength(aClonks))
		return true;
}

public func Activate(object pCaller)
{
	var aClonks = FindObjects(Find_ID(MEDC), Find_Distance(100, GetX(pCaller), GetY(pCaller)), Find_Action("Dead"), Find_Exclude(pCaller));
	for (var pClonk in aClonks) 
	{
		if (pCaller->Blood() < 8)
			break;
		pCaller->DoVal(cBlood, -8);
		pCaller->DoVal(cSleep, -19);
		DoEnergy(-GetEnergy(pClonk) / 4, pClonk, false, FX_Call_EngDarkMagic);
		pClonk->~DoVal(cSleep, 30);
		if (Random(3))
			M_SL->Activate(pCaller, true);
		var angle = Angle(GetX(pClonk), GetY(pClonk), GetX(), GetY());
		for (var i = 0; i < 2; i++)
			CreateParticle("PSpark", RandomX(-3, 3), -1, Sin(RandomX(-3, 3) + angle, 10), -Cos(RandomX(-3, 3) + angle, 10), RandomX(40, 80), RGB(90, 0, 200), pClonk);
		for (var i = 0; i < 2; i++)
			CreateParticle("PSpark", RandomX(-3, 3), 2, Sin(RandomX(-3, 3) + angle, 10), -Cos(RandomX(-3, 3) + angle, 10), RandomX(40, 80), RGB(90, 0, 200), pClonk);
	}
	return true;
}
