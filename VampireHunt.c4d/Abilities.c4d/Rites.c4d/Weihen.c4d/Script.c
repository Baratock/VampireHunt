/*-- Weihen --*/

#strict 2

//Wird als Gebet erkannt
public func IsPrayer()
{
	return true;
}

//Voraussetzungen
public func Requirements(object pCaller)
{
	//Nur Menschen + im Freien 
	if (pCaller->GetVal(cForm) != cRoleHuman || Contained(pCaller))
		return false;
	//+ laufen + Mana
	if (GetAction(pCaller) != "Walk" || GetMagicEnergy(pCaller) < 20)
		return false;
	return true;
}

public func Apply(object pCaller)
{
	SetComDir(COMD_None, pCaller);
	pCaller->SetAction("Pray");
	AddEffect("PrayerWeihen", pCaller, 148, 80, 0, GetID(this));
	return RemoveObject(this);
}

protected func FxPrayerWeihenStart(object pCaller, int iEffectNumber)
{
	EffectVar(0, pCaller, iEffectNumber) = 0;
}

protected func FxPrayerWeihenTimer(object pCaller, int iEffectNumber, int iEffectTime)
{
	var iPhase = EffectVar(0, pCaller, iEffectNumber);
	//Wenn er stirbt oder getroffen wird
	if (!pCaller || GetAction(pCaller) != "Pray")
		return -1;
	if (iPhase == 0)
		Message("In nomino dominus", pCaller);
	if (iPhase == 1)
		Message("Is res sacer redeo", pCaller);
	if (iPhase == 2)
	{
		pCaller->SetAction("KneelUp");
		DoMagicEnergy(-5, pCaller);
		CreateContents(PHWT, pCaller);
		CreateContents(PHWT, pCaller);
		Message("       AMEN       ", pCaller);
		return -1;
	}
	DoMagicEnergy(-5, pCaller);
	EffectVar(0, pCaller, iEffectNumber)++;
}
