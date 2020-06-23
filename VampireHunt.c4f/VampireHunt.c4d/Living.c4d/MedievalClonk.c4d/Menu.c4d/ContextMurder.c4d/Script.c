/*-- TÖTEN *cough* --*/

#strict 2

public func IsAbility(){ return true; }

public func GetSpellCombo() { return "555"; }

public func CanUse(object pCaller)
{
    if (pCaller->SleepingClonk() && pCaller->Weapon())
        return 1;
}

public func Activate(object pCaller)
{
    var pVictim = pCaller->SleepingClonk();
    var idWeapon = pCaller->Weapon();
    if (pVictim->Who() == Role_Vampire)
    {
        if (idWeapon == AXE1 || idWeapon == STAK || idWeapon == SBRE)
        {
				Message("Hasta la vista, %s", pCaller, GetPlayerName(GetOwner(pVictim)));
                Sound("Tap*",false,pCaller);
                pVictim->Death(idWeapon);
                return true;
        }
        else 
            return Message("$VHNotAble$", pCaller, GetName(0,idWeapon));
    }
    Message("Hasta la vista, %s", pCaller, GetPlayerName(GetOwner(pVictim)));
    Sound("Tap*",false,pCaller);
    pVictim->Death(idWeapon);
    return true;
}

