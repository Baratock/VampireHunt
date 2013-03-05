/*-- Beistand --*/

#strict 2

//Wird als Gebet erkannt
public func IsPrayer()
{
	return true;
}

//Voraussetzungen
public func Requirements(object pCaller)
{
	//Nur laufende Menschen
	if (pCaller->GetVal(cForm) != cRoleHuman || GetAction(pCaller) != "Walk")
		return false;
	//Nicht genug Mana
	if (GetMagicEnergy(pCaller) < 10)
		return false;
	return true;
}

public func Apply(object pCaller)
{
	AddEffect("PrayerBeistand", pCaller, 128, 70, 0, GetID(this));
	return RemoveObject(this);
}

protected func FxPrayerBeistandStart(object pCaller, int iEffectNumber)
{
	EffectVar(0, pCaller, iEffectNumber) = 0;
}

protected func FxPrayerBeistandTimer(object pCaller, int iEffectNumber, int iEffectTime)
{
	var iPhase = EffectVar(0, pCaller, iEffectNumber);
	//Wenn er stirbt zb
	if (!pCaller)
		return -1;
	//Klappe zu? Treffer, Abtauchen, Kämpfe...
	if (GetAction(pCaller) != ("Walk" || "Climb" || "Hangle" || "Jump"))
		return -1;
	if (iPhase == 0)
		Message("Scriptus magnus!", pCaller);
	if (iPhase == 1)
		Message("Servos tuas in poenas clamunt", pCaller);
	if (iPhase == 2)
		Message("Poenas a corpi expelle!", pCaller);
	if (iPhase == 3)
		Message("Corpus hostilis a ea tormenti eras!", pCaller);
	if (iPhase == 4)
	{
		//Schmerzen tilgen
		var iPain;
		for (var pClonk in FindObjects(Find_ID(GetID(pCaller)), Find_Distance(120, GetX(pCaller), GetY(pCaller)))) 
		{
			iPain = pClonk->GetVal(cPain);
			if (pClonk->GetVal(cForm) == cRoleHuman)
			{
				pClonk->DoVal(cPain, -iPain / 2);
				if (GetAction(pClonk) == "Dead")
					M_SL->Activate(pClonk, true);
				Message("AMEN", pClonk);
			}
		}
		return -1;
	}
	pCaller->DoMagicEnergy(-2);
	EffectVar(0, pCaller, iEffectNumber)++;
}

