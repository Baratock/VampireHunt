/*-- Reiner Wille --*/

#strict 2

public func IsAbility(){ return true; }

public func GetSpellCombo() { return "365"; }

public func CanUse(object pCaller)
{
    if (pCaller->Job()->~ManaType() != 0)
        return false;
    if(pCaller->Job()->~CantUseWill())
        return false;
    if (GetAction(pCaller) == "Dead" && GetMagicEnergy(pCaller))
    {
    	if(pCaller->RestoreSleep() > pCaller->MaxSleepnes() || pCaller->Pain() > pCaller->MaxPain() || pCaller->Blood() < 20)
    	{
			return true;
		}
	}
}

public func Activate(object pCaller)
{
    var r = Random(2);
    if (r == 0)
        PlayerMessage(GetOwner(pCaller),"$VHDontGiveUp1$", this);
    if (r == 1)
        PlayerMessage(GetOwner(pCaller),"$VHDontGiveUp2$", this);
    if (r == 2)
        PlayerMessage(GetOwner(pCaller),"$VHDontGiveUp3$", this);
    pCaller->SetAction("FlatUp");
    pCaller->LocalN("power") = M_WL; 
}

