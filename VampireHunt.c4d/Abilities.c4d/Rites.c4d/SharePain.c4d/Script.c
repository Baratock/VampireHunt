/*-- Schmerzen Teilen --*/

#strict 2

//Wird als Gebet erkannt
public func IsPrayer()
{
	return true;
}
public func GetPrayCooldown()
{
	return 50;
} //Abklingzeit

//Voraussetzungen
public func Requirements(object pClonk)
{
	//Nur Menschen mit Mana
	if (pClonk->GetVal(cForm) != cRoleHuman || GetAction(pClonk) != "Walk" || GetMagicEnergy(pClonk) < 30)
	{
		return false;
	}
	return true;
}

public func Apply(object pClonk)
{
	AddEffect("PrayerSharePain", pClonk, 150, 11, 0, GetID(this));
	return RemoveObject(this);
}

//Effekt für das Schmerzen teilen
protected func FxPrayerSharePainStart(object pTarget, int iEffectNumber, int iTemp)
{
	SetComDir(COMD_None, pTarget);
	ObjectSetAction(pTarget, "Pray");
}

protected func FxPrayerSharePainTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	if (GetAction(pTarget) != "Pray")
		return -1;
	if (iEffectTime < 11 * 7)
	{
		//Mana abziehen: in diesem Fall Glauben
		DoMagicEnergy(-3, pTarget);
	}
	else
	{
		//Wenn der Priester zu Ende beten konnte, dann andere Clonks heilen und selbst Schmerz beziehen
		var aClonks = FindObjects(Find_ID(GetID(pTarget)), Find_Distance(50, GetX(pTarget), GetY(pTarget)), Find_Allied(GetOwner(pTarget)), Find_Exclude(pTarget));
		for (var pClonk in aClonks) 
		{
			//Schmerz errechnen
			var iPain;
			iPain = pClonk->GetVal(cPain) * 40 / 100;
			//Abziehen und dem Priester hinzufügen
			pClonk->DoVal(cPain, -iPain);
			pTarget->~DoPain(iPain / 2);
		}
		ObjectSetAction(pTarget, "FlatUp");
		return -1;
	}
}
