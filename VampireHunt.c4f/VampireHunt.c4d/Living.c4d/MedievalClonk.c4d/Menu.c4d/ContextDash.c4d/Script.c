/*-- Sprinten --*/

#strict 2

public func IsAbility(){ return true; }

public func GetSpellCombo() { return "111"; }

public func CanUse(object pCaller)
{
    if(pCaller->Job() == 0)
        return false;
    if(pCaller->Job()->~ManaType() != 0)
        return false;
    if(!pCaller->Job()->~CanUseDash())
        return false;
    if(pCaller->LocalN("power") == M_WL)
        return false;
    if(GetAction(pCaller) == "Walk" || GetAction(pCaller) == "Dead")
    	if(GetMagicEnergy(pCaller) && !GetEffect("Dash",pCaller))
    		if(pCaller->RestoreSleep() > (pCaller->MaxSleepnes()*3/4) || pCaller->Pain() > (pCaller->MaxPain()*3/4) || pCaller->Blood() < 30)
				return true;
}

public func Activate(object pCaller)
{
    pCaller->CntextAwake();
    AddEffect("Dash",pCaller,300,20,pCaller);
}

protected func FxDashStart(object pTarget, int iEffectNumber, int iTemp)
{
    //alle negativen Effekte beseitigen :)
    EffectVar(0,pTarget,iEffectNumber) = pTarget->Pain();
    EffectVar(1,pTarget,iEffectNumber) = pTarget->RestoreSleep();
    EffectVar(2,pTarget,iEffectNumber) = pTarget->Hunger();
    pTarget->RestorePain(EffectVar(0,pTarget,iEffectNumber));
    pTarget->RestoreSleep(EffectVar(1,pTarget,iEffectNumber));
    pTarget->Hunger(-EffectVar(2,pTarget,iEffectNumber));
    //aktualisieren
    pTarget->HumanForm();
    //UND LOS!
    SetPhysical("Walk",GetPhysical("Walk",1,0,GetID(pTarget))*5/3,PHYS_Temporary,pTarget);
}

protected func FxDashTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
    if(!GetMagicEnergy(pTarget))
    {
        //wieder normale geschw.
        SetPhysical("Walk",GetPhysical("Walk",1,0,GetID(this))*3/5,PHYS_Temporary,pTarget);
        //effekte zurücksetzen
        pTarget->RestorePain(-EffectVar(0,pTarget,iEffectNumber));
        pTarget->RestoreSleep(-EffectVar(1,pTarget,iEffectNumber));
        pTarget->Hunger(EffectVar(2,pTarget,iEffectNumber));
        return -1;
    }
    DoMagicEnergy(-1,pTarget);
    return true;
}



