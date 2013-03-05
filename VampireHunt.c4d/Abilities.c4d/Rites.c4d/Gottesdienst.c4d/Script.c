/*-- Gottesdienst --*/

#strict 2

//Wird als Gebet erkannt
public func IsPrayer()
{
	return true;
}

//Voraussetzungen
public func Requirements(object pCaller)
{
	//pCaller vorhanden + in einem Gebäude?
	if (!pCaller || !Contained(pCaller))
		return false;
	//in einem heiligen Gebäude?
	if (!Contained(pCaller)->~IsHolyPlace())
	{
		return false;
	}
	//Nur Menschen + 2 >= Weihwasser in Kirche
	if (pCaller->GetVal(cForm) != cRoleHuman || ContentsCount(PHWT, Contained(pCaller)) < 2)
	{
		return false;
	}
	//Zwei andere Menschen bitte
	var iCnt = 0;
	for (var pClonk in FindObjects(Find_Container(Contained(pCaller)), Find_ID(GetID(pCaller)))) 
		if (pClonk->GetVal(cForm) == cRoleHuman)
			iCnt++;
	if (iCnt < 2)
		return false;
	return true;
}

public func Apply(object pCaller)
{
	var pChurch = Contained(pCaller);
	pChurch->SetEntrance(0);
	RemoveObject(FindContents(PHWT, pChurch));
	RemoveObject(FindContents(PHWT, pChurch));
	AddEffect("PrayerService", pCaller, 63, 120, 0, GetID(this));
	return RemoveObject(this);
}

protected func FxPrayerServiceStart(object pPriest, int iEffectNumber)
{
	EffectVar(0, pPriest, iEffectNumber) = 0;
}

protected func FxPrayerServiceTimer(object pPrist, int iEffectNumber, int iEffectTime)
{
	var iPhase = EffectVar(0, pPrist, iEffectNumber);
	if (!pPrist)
		return -1;
	if (iPhase == 0)
		Message("@Scriptus insignis!", pPrist);
	if (iPhase == 1)
		Message("@Filii tui te salutant", pPrist);
	if (iPhase == 2)
		Message("@et sacrificium peterent!", pPrist);
	if (iPhase == 3)
		Message("@te uus vis et virtua debere", pPrist);
	if (iPhase == 4)
		Message("@et nocta infinis a terra is redinere requimus", pPrist);
	if (iPhase == 5)
	{
		for (var pObj in FindObjects(Find_Container(Contained(pPrist)))) 
		{
			//Alle Items in der Kirche weihen
			if (pObj->~IsPoisionCarrier())
			{
				pObj->Impregnate(PHWT);
				continue;
			}
			//Mana auffüllen
			if (pObj->~GetRole()->ManaType() == cMagicWill && pObj->GetVal(cForm) == cRoleHuman)
				DoMagicEnergy(100, pObj, true);
		}
		//Eingang öffnen
		Contained(pPrist)->SetEntrance(1);
		Message("Amen.", pPrist);
		return -1;
	}
	DoMagicEnergy(3, pPrist, true);
}