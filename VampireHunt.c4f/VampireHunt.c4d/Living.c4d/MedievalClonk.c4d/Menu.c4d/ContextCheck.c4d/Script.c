/*-- Überprüfen --*/

#strict 2

public func IsAbility(){ return true; }

public func GetSpellCombo() { return "315"; }

public func CanUse(object pCaller)
{
	if(pCaller->SleepingClonk())
		return true;
}

public func Activate(object pCaller)
{
    var pVictim = pCaller->SleepingClonk();
	if (!pVictim)
		return 0;
	var szWho   = "";
	var szGhoul = "";
	var szBlood = "";
	var szPain  = "";
    //Info sammeln
    szWho = GetPlayerName(GetOwner(pVictim));
    if (pVictim->Blood() < 40)
        szBlood = "$VHCheckBlood1$";
    if (pVictim->Blood() < 20)
        szBlood = "$VHCheckBlood2$";
    if (pVictim->Pain()  > 40)
        szPain  = "$VHCheckPains1$";
    if (pVictim->Pain()  > 60)
        szPain = "$VHCheckPains2$";
    if (pVictim->Who() == Role_Slave)
		szGhoul = "$VHCheckGhul$";
	//nachricht
    PlayerMessage(GetOwner(pCaller), "$VHCheckName$", pCaller,szWho,szBlood,szPain,szGhoul);
}




