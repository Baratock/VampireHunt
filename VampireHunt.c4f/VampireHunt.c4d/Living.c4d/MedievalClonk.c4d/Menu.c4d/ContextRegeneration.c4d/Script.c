/*-- Regeneration --*/

#strict 2

public func IsAbility(){ return true; }

public func GetSpellCombo() { return "223"; }

public func CanUse(object pCaller)
{
	if (pCaller->Who() == Role_Lich || pCaller->Who() == Role_Vampire)
		if(!pCaller->LocalN("power") && pCaller->Blood() && GetEnergy(pCaller) < pCaller->MaxHealth())
			return true;
	if(pCaller->LocalN("power") == M_RE)
		return true;
}

public func Activate(object pCaller) //Blutsaugen
{
	if(pCaller->LocalN("power") == M_RE)
		pCaller->LocalN("power") = 0;
	else
    	pCaller->LocalN("power")  = M_RE;
    if (pCaller->Who() == Role_Vampire)
        pCaller->VampireForm();
    if (pCaller->Who() == Role_Lich)
        pCaller->LichForm();
} 