/*-- Schmerzwelle --*/

#strict 2

public func IsAbility()
{
	return true;
}

public func GetSpellCombo()
{
	return "423";
}

public func CanUse(object pCaller)
{
	if (pCaller->IsDownCooled())
		if (pCaller->GetRole() == cRoleVampire && IsNight() && pCaller->GetVal(cBlood) > 25)
			return true;
}

public func Activate(object pCaller)
{
	//Weiterleiten an Apply
	pCaller->Pray(GetID(this));
} 

public func Apply(object pCaller)
{
	//Magiesound und Kosten
	Sound("Magic*", false, pCaller);
	pCaller->DoVal(cBlood, -25);
	//Opfer erleiden Schmerzen und Schreien
	for (var pVictim in FindObjects(Find_ID(GetID(pCaller)), Find_Distance(100))) 
	{
		if (pVictim->GetVal(cForm) == cRoleHuman)
		{
			Sound("Die", false, pVictim);
			pVictim->DoPain(30 + Random(25));
		}
	}
	//Partikel
	for (var x = 180; x >= -180; x -= 2)
	{
		CreateParticle("NoGravSpark", 0, 0, Sin(x, 20, 1), Cos(x, 20, 1), 50, RGBa(255, 55, 55, 127));
	}
	Sound("PainWave", false, pCaller);
	return RemoveObject(this);
}

public func GetPrayCooldown()
{
	return 30;
}