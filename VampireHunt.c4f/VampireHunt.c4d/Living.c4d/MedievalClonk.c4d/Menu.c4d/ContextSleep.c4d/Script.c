/*-- Schlafen --*/

#strict 2

public func IsAbility(){ return true; }

public func GetSpellCombo() { return "332"; }

public func CanUse(object pCaller)
{
    if (pCaller->RestoreSleep() < pCaller->MaxSleepnes())
    	if(pCaller->RestorePain() < pCaller->MaxPain() && pCaller->GetAction() == "Dead")
        	return 1;
    if (GetID(Contained(pCaller)) != SELE && pCaller->GetAction() == "Walk")
        return 1;    
}

public func Activate(object pCaller)
{
    if(pCaller->GetAction() == "Dead")
    {
		if (!pCaller->RestorePain() && pCaller->RestoreSleep() > 50)
			PlayerMessage(GetOwner(pCaller),"$VHWhat$", pCaller);
	    Sound("Heartbeat", 0, pCaller, 0, GetOwner(pCaller) + 1, -1);
	    pCaller->SetAction("FlatUp");
	}
	else
	{
	    if (pCaller->RestorePain())
	    {
	        var volume = pCaller->Pain();
	        if (volume > 100)
	            volume = 100;
	        Sound("heartbeat", 0, pCaller, volume, GetOwner(pCaller) + 1, 1);
	    }
	    if (!pCaller->RestorePain() && pCaller->RestoreSleep())
	        PlayerMessage(GetOwner(pCaller),"$VHSleep$", pCaller);
	    if (pCaller->RestorePain() > pCaller->MaxPain())
	    {
	        PlayerMessage(GetOwner(pCaller),"Argh...", pCaller);
	        if (pCaller->LocalN("form") && pCaller->Who() == Role_Slave)
	            M_SS->Activate(pCaller);
	    }
	    pCaller->SetAction("Dead");
	}
}
