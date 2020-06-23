#strict 2

#appendto J_SM

public func Support(object pClonk)
{
    Init(pClonk);
    pClonk -> Sound("der amboss");
    if(GetPlayerTeam(GetOwner(pClonk)) == 2)
    {
	    var pHouse = FindObject(WRKS);
	    SetOwner(NO_OWNER,pClonk->House());
		pClonk->House(pHouse);
		Enter(pHouse,pClonk);
	}
}