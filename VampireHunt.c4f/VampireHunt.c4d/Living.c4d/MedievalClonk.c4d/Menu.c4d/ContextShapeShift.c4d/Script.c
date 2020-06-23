/*-- Wachen --*/

#strict 2

public func IsAbility(){ return true; }

public func GetSpellCombo() { return "315"; }

public func CanUse(object pCaller)
{
    if (pCaller->Who() == Role_Slave)
        return true;
}

public func Activate(object pCaller)
{
    if (!pCaller->LocalN("form"))
    {
        pCaller->LocalN("form") = 1;
        Sound("Ghoul",false,pCaller);
        pCaller->GhoulForm();
    }
    else 
    {
        pCaller->LocalN("form") = 0;
        Sound("DeGhoul",false,pCaller);
        pCaller->HumanForm();
    }
	return 1;
}