/*-- Brauen --*/

#strict 2

public func IsAbility(){ return true; }

public func GetSpellCombo() { return "561"; }

public func CanUse(object pCaller)
{
    for(var IDdef = 0, i = 0; IDdef = GetPlrKnowledge(GetOwner(pCaller), 0, i, C4D_Object); i++)
        if(IDdef->~IsPotion() && GetAction(pCaller) == "Walk" && (pCaller->Who() == Role_Human || pCaller->Who() == Role_Slave))
            return 1;
}

public func Activate(object pCaller)
{
    CreateMenu(PELF,pCaller,pCaller);
    for(var IDdef = 0, i = 0; IDdef = GetPlrKnowledge(GetOwner(pCaller), 0, i, C4D_Object); i++)
        if (IDdef->~IsPotion())
            AddMenuItem("$VHProduce$", "MakePotion", IDdef, pCaller);
}
