/*-- Unsichtbar --*/

#strict 2

public func IsAbility(){ return true; }

public func GetSpellCombo() { return "355"; }

public func CanUse(object pCaller)
{
	if(pCaller->Who() == Role_Vampire && pCaller->Blood() > 50)
		if(!pCaller->LocalN("power") || pCaller->LocalN("power") == M_IV)
			return true;
}

public func Activate(object pCaller)
{
	if(pCaller->LocalN("power") == M_IV)
		pCaller->LocalN("power") = 0;
	else
    	pCaller->LocalN("power")  = M_IV;
    pCaller->VampireForm();
} 