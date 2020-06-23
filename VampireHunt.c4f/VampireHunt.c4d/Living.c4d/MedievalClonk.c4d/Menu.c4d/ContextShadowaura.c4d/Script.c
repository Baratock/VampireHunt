/*-- Regeneration --*/

#strict 2

public func IsAbility(){ return true; }

public func GetSpellCombo() { return "565"; }

public func CanUse(object pCaller)
{
    //nur Lichs können Dunkle Auren wirken
    if (pCaller->Who() != Role_Lich)
        return false;
    //nur Nachts und mit genug Blut
    if (IsNight() && pCaller->Blood() >= 10)
		return true;
}

public func Activate(object pCaller) //Blutsaugen
{
    if (GetEffect("DarkAura", pCaller))
    {
        RemoveEffect(0, pCaller, GetEffect("DarkAura", pCaller));
        return 1;
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
    if (!pTarget->Blood())
        return -1;
    if ((iEffectTime % 12) < 5)
        pTarget->Blood(-1);
    //Tadaa:der Optische Effect
    var i = 0;
    if ((iEffectTime % 8) < 3)
        while (i < 360)
        {
            i += 8;
            var iSin = Sin(i, 300);
            var iCos = Cos(i, 300);
            CreateParticle("PSpark",GetX(pTarget)+ iSin * 2 / 5 + RandomX(-1, 1), GetY(pTarget) +  iCos * 2 / 5 + RandomX(-1, 1), iSin / RandomX(60, 85), iCos / RandomX(60, 85), RandomX(220, 260), RGBa(90, 0, 200, 110));
            CreateParticle("PSpark",GetX(pTarget)+  iSin / 3 + RandomX(-1, 1),GetY(pTarget) +  iCos / 3 + RandomX(-1, 1), iSin / RandomX(40, 55), iCos / RandomX(40, 55), RandomX(160, 190), RGBa(90, 0, 200, 110));
        }
    return true;
}