/*-- fressen --*/

#strict 2

public func IsAbility(){ return true; }

public func GetSpellCombo() { return "141"; }

public func CanUse(object pCaller)
{
    if (pCaller->SleepingClonk() && pCaller->Who() == Role_Slave)
        return 1;
}

public func Activate(object pCaller)
{
    var pVictim = pCaller->SleepingClonk();
    if (pVictim->Who() == Role_Vampire)
    {
        PlayerMessage(GetOwner(pCaller), "$VHCrazy$");
    }
    else 
    {
        pCaller->Pain(-20);
        pCaller->Hunger(-40);
        pCaller->Feed(20, 1);
        pCaller->DoEnergy(15);
        PlayerMessage(GetOwner(pCaller), "$VHFeelBetter$");
        PlayerMessage(GetOwner(pCaller),"$VHMoreMeat$", pCaller, GetPlayerName(GetOwner(pVictim)));
        pVictim->Death2(pCaller->Weapon());
    }
}



