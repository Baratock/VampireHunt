/*-- Wachen --*/

#strict 2

public func IsAbility(){ return true; }

public func GetSpellCombo() { return "515"; }

public func CanUse(object pCaller)
{
    if(!pCaller->LocalN("power") && pCaller->Job() == J_GD && pCaller->Who() == Role_Human && GetAction(pCaller) != "Dead")
        return true;
    return false;
}

public func Activate(object pCaller)
{
	if(GetEffect("J_GD_Guard",pCaller))
	{
		RemoveEffect("J_GD_Guard", pCaller);
	}
	else
	{
	    AddEffect("J_GD_Guard", pCaller, 102, 1, 0, GetID(this));
    }
	return 1;
}

protected func FxJ_GD_GuardTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
    if((GetProcedure(pTarget) != "WALK"
    && GetProcedure(pTarget) != "PULL"
    && GetProcedure(pTarget) != "LIFT"
    && GetProcedure(pTarget) != "FLIGHT"
    && GetProcedure(pTarget) != "NONE"
    && GetProcedure(pTarget) != "PUSH"
    && GetProcedure(pTarget) != "THROW")
    || LocalN("power", pTarget)
    || pTarget->Who() != Role_Human)
        return -1;
    //Nur alle 80 frames aufrufen
	if(!(iEffectTime%80))
		return true;
    //Schlafende Menschen finden
    var aSleepingClonks = [];
    var j;
    for(var pSleepingClonk in FindObjects(Find_ID(MEDC), Find_Distance(90, GetX(pTarget), GetY(pTarget)), Find_Exclude(pTarget), Find_Action("Dead")))
    {
        if(pSleepingClonk->Who() == Role_Human)
        {
            aSleepingClonks[j] = pSleepingClonk;
            j++;
        }
    }
    //Nicht schlafende Clonks  finden
    var b = FindObjects(Find_ID(MEDC), Find_Distance(110, GetX(pTarget), GetY(pTarget)), Find_Exclude(pTarget), Find_Not(Find_Action("Dead")));
    
    //berechnen und x-mal Erfahrungen geben
	for(var i = IsNight() + GetLength(aSleepingClonks)/GetLength(b) + 1; i > 0; i--)
		pTarget->~DoExperience();     
    return 1;    
}