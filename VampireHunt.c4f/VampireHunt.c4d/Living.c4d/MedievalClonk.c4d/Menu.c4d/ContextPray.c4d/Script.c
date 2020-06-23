/*-- Beten --*/

#strict 2

public func IsAbility(){ return true; }

public func GetSpellCombo() { return "423"; }

public func CanUse(object pCaller)
{
    if (pCaller->Job()->~ManaType() != 1)
        return false;
    if (pCaller->LocalN("prayer") || !pCaller->IsDownCooled())
        return false;
	var def, i;
	while (def = GetPlrKnowledge(GetOwner(pCaller), 0, i++, C4D_StaticBack))
		if (def->~IsPrayer())
            if (def->~Requirements(pCaller))
                return true;
}

public func Activate(object pCaller)
{
    CreateMenu(M_PW,pCaller,pCaller);
    var def, i;
    while (def = GetPlrKnowledge(GetOwner(pCaller), 0, i++, C4D_StaticBack))
        if (def->~IsPrayer())
            if (def->~Requirements(pCaller))
                AddMenuItem("%s", "Pray", def, pCaller);
    //Ende
    return true;
}


