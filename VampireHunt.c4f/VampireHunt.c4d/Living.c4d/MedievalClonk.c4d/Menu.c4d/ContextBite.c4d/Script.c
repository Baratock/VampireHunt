/*-- Beißen --*/

#strict 2

public func IsAbility(){ return true; }

public func GetSpellCombo() { return "623"; }

public func CanUse(object pCaller)
{
    if (pCaller->SleepingClonk() && pCaller->Blood() < 200 && pCaller->Who() == Role_Vampire)
		if (pCaller->SleepingClonk()->Blood())
			return true;
}

public func Activate(object pCaller) //Blutsaugen
{
    var victim = pCaller->SleepingClonk();
    var need = 200 - pCaller->Blood();
    if (victim->Blood() < need)
        need = victim->Blood();
    if (need > 50)
        need = 50;
    PlayerMessage(GetOwner(pCaller), "$VHDrinkBlood$", pCaller, need, GetPlayerName(GetOwner(victim)));
    victim->Blood(-need);
    pCaller->Feed(need, 1);
} 