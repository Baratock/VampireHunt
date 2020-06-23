/*-- Antisleep --*/

#strict 2

public func IsAbility(){ return true; }

public func GetSpellCombo() { return "512"; }

public func CanUse(object pCaller)
{
	if(GetEffect("AntiSleep",pCaller) || pCaller->Job() != J_VH)
	{
		return false;
	}
    return true;
}

public func Activate(object pCaller)
{
	AddEffect("AntiSleep",pCaller,100,10,0,GetID(this));
}

protected func FxAntiSleepStart(object pTarget, int iEffectNumber, int iTemp)
{
    //- 30 müdigkeit
    pTarget->RestoreSleep(30);
    EffectVar(0,pTarget,iEffectNumber) = pTarget->RestoreSleep();
    //müdigkeit löschen
    pTarget->RestoreSleep(200);
    EffectVar(1,pTarget,iEffectNumber) = 0; 
}

protected func FxAntiSleepTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
    if(iEffectTime < 30*37)
    {
        CreateParticle("PSpark",GetX(pTarget),GetY(pTarget),0,0,180,RGBa(120,255,120,120),pTarget,true);
    }
    else if(EffectVar(1,pTarget,iEffectNumber) == 0)
    {
        EffectVar(1,pTarget,iEffectNumber)++;
        //müdigkeit wieder löschen
        pTarget->RestoreSleep(200);
        //schlaf dann herstellen
        pTarget->RestoreSleep(-EffectVar(0,pTarget,iEffectNumber));
    }
    //cooldown eine Minute
    if(iEffectTime > 90*36)
        return -1;  
}
