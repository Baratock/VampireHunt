/*-- Regeneration --*/

#strict 2

public func IsAbility()
{
	return true;
}

public func GetSpellCombo()
{
	return "565";
}

public func CanUse(object pCaller)
{
	//nur Lichs können Dunkle Auren wirken
	if (pCaller->GetRole() != cRoleLich)
		return false;
	//nur Nachts und mit genug Blut
	if (IsNight() && pCaller->GetVal(cBlood) >= 10)
		return true;
}

public func Activate(object pCaller) //Blutsaugen
{
	if (GetEffect("DarkAura", pCaller))
	{
		RemoveEffect(0, pCaller, GetEffect("DarkAura", pCaller));
	}
	else
	{
		AddEffect("DarkAura", pCaller, 102, 4, 0, GetID(this));
	}
}

//Effect für die Schattenaura
protected func FxDarkAuraTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	//Wenn es kein Blut gibt abbrechen und wenn doch welches abziehen
	if (pTarget->GetVal(cBlood) < 5)
		return -1;
	if ((iEffectTime % 12) < 4)
		pTarget->DoVal(cBlood, -1);
	//Tadaa:der Optische Effect
	if ((iEffectTime % 8) < 3)
		for (var i = 0; i < 360; i += 8)
		{
			var iSin = Sin(i, 300);
			var iCos = Cos(i, 300);
			CreateParticle("PSpark", GetX(pTarget) + iSin * 2 / 5 + RandomX(-1, 1), GetY(pTarget) + iCos * 2 / 5 + RandomX(-1, 1), iSin / RandomX(60, 85), iCos / RandomX(60, 85), RandomX(220, 260), RGBa(90, 0, 200, 110));
			CreateParticle("PSpark", GetX(pTarget) + iSin / 3 + RandomX(-1, 1), GetY(pTarget) + iCos / 3 + RandomX(-1, 1), iSin / RandomX(40, 55), iCos / RandomX(40, 55), RandomX(160, 190), RGBa(90, 0, 200, 110));
		}
	return true;
}
