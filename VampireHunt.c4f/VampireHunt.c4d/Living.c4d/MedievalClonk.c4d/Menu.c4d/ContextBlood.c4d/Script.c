/*-- Blutflasche --*/

#strict 2

public func IsAbility(){ return true; }

public func GetSpellCombo() { return "165"; }

public func CanUse(object pCaller)
{
    if ((pCaller->Who() == Role_Vampire || pCaller->Who() == Role_Lich) && pCaller->Blood() > 25)
        return true;
}

public func Activate(object pCaller) //Blutsaugen
{
	pCaller->Blood(-25);
    CreateContents(PBLD,pCaller);
    PlayerMessage(GetOwner(pCaller), "$VHFillBlood$", pCaller);
}


