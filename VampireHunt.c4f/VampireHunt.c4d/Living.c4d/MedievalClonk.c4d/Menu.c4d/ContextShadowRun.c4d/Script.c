/*-- Schattenlauf --*/

#strict 2

public func IsAbility(){ return true; }

public func GetSpellCombo() { return "363"; }

public func CanUse(object pCaller)
{
    if (!pCaller->LocalN("prayer"))
		if(!pCaller->LocalN("Power*"))
        if (pCaller->Who() == Role_Vampire && IsNight() && pCaller->Blood() > 30)
			return true;
}

public func Activate(object pCaller)
{
    //Magiesound und Kosten
    Sound("Magic*",false,pCaller);
	pCaller->Blood(-30);
	//Clonks hinter dem Vampir schaden
	var iDir = GetDir(pCaller)*2 - 1;
	var pClonks = FindObjects(Find_ID(GetID(pCaller)),Find_Hostile(GetOwner(pCaller)),Find_InRect(0,-20,40*iDir,40));
	for(var pVictim in FindObjects(Find_ID(MEDC),Find_Distance(100,GetX(pCaller),GetY(pCaller))))
	{
	if(pVictim->Who() == Role_Human)
	{
		pVictim->Pain(34 + Random(15));
	
	}
	AddEffect("ShadowRun",pCaller,260,1,0,GetID(this));
}
}
protected func FxShadowRunStart(object pTarget, int iEffectNumber, int iTemp)
{
	//3-fache Laufgeschwindigkeit und losrennen
	if(GetDir(pTarget) == DIR_Left)
		SetComDir(COMD_Left,pTarget);
	else
		SetComDir(COMD_Right,pTarget);
}

protected func FxShadowRunTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	//Rücksetzen
	if(iEffectTime > 50)
	{
		SetCrewEnabled(true,pTarget);
		SelectCrew(GetOwner(pTarget),pTarget,true);
		pTarget->SetVal(cSlowDown,100);
		pTarget->SetVal(cCooldown,100); 		
		return -1;
	}
	//Ziele finden
	var iDir = GetDir(pTarget)*2 - 1;
	var x = GetX(pTarget);
	var y = GetY(pTarget);
	var pVictims = FindObjects(Find_ID(GetID(pTarget)),Find_Action("Walk"),Find_Hostile(GetOwner(pTarget)),Find_InRect(x-10*(!GetDir(pTarget)),y-10,10,30));
	for(var pVictim in pVictims)
	{
		//wegschleudern
		Fling(pVictim,-iDir*2,-3);
		//schaden
		var t = 50 + iEffectTime;
		pVictim->DoPain(36*t/100);
		pVictim->DoVal(cSleep,-30*t/100);			
	}
	CreateParticle("MSpark",x,y,iDir*4,0,140,RGB(80,0,80));
}

