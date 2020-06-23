/*-- Blutopfer --*/

#strict 2

public func IsAbility(){ return true; }

public func GetSpellCombo() { return "623"; }

public func CanUse(object pCaller)
{
    if (!pCaller->Blood() || pCaller->Who() != Role_Lich)
        return false;
    if (GetEnergy(pCaller) < 15)
        return false;
    for (var pClonk in FindObjects(Find_ID(MEDC), Find_Distance(150,GetX(pCaller),GetY(pCaller)))) 
    {
        if (pClonk->Who() == Role_Vampire && GetEnergy(pClonk) < pClonk->~MaxHealth())
			return true;
    }
    return false;
}

public func Activate(object pCaller)
{
    var pVamp = 0;
    for (pVamp in FindObjects(Find_ID(MEDC), Find_Distance(150,GetX(pCaller),GetY(pCaller)))) 
    {
        if (pVamp->Who() == Role_Vampire)
            break;
    }
	if(GetEffect("SacrificeHealPSpell",pCaller))
	{
    	RemoveEffect(0, this, GetEffect("SacrificeHealPSpell",pCaller));
		return 1;
	}
	else
    	AddEffect("SacrificeHealPSpell", pCaller, 101, 6, 0, GetID(this), pVamp);
} 

protected func FxSacrificeHealPSpellStart(object pTarget, int iEffectNumber, int iTemp, object pVamp)
{
    EffectVar(1, pTarget, iEffectNumber) = pVamp;
}

protected func FxSacrificeHealPSpellTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
    var pVamp = EffectVar(1, pTarget, iEffectNumber);
    //Erstmal sehen ob der Vampir noch da ist
    if (Distance(GetX(pTarget), GetY(pTarget), GetX(pVamp), GetY(pVamp)) > 120)
        return -1;
    if (!pTarget->Blood())
        return -1;
    if (GetEnergy(pTarget) < 15)
        return -1;
    if (GetEnergy(pVamp) >= pVamp->~MaxHealth())
        return -1;
    //dem Vampir leben Geben und dem Lich Blut abziehen
    DoEnergy(+2, pVamp);
    pTarget->Blood(-1);
    DoEnergy(-1, pTarget, false, FX_Call_EngDarkMagic);
    CreateParticle("Blood", GetX(pTarget)+RandomX(-2, 3), GetY(pTarget)+RandomX(-2, 3), RandomX(-3, 3), -3, 14, RGB(255, 0, 0));
    return true;
}
