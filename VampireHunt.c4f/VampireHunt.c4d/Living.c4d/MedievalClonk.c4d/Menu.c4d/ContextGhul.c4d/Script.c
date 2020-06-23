/*-- Diener erschaffen --*/

#strict 2

public func IsAbility(){ return true; }

public func GetSpellCombo() { return "242"; }

public func CanUse(object pCaller)
{
    if (pCaller->Who() == Role_Vampire && pCaller->SleepingClonk())
    	if(!pCaller->LocalN("partner") && pCaller->Blood() > 75)
        	return 1;
}

public func Activate(object pCaller)
{
    CreateMenu(VAMP, pCaller, pCaller, 0, "$VHMakeSlave$", 0, 1);
    AddMenuItem("$VHMakeGhul$", "MakeSlave(1)", VAMP, pCaller);
    AddMenuItem("$VHMakeLich$", "MakeSlave(2)", LICH, pCaller);
    return true;
}