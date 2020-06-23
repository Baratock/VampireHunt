/*-- Gottesdienst --*/

#strict 2

local pPrist;
local phase;
local pChurch;

//Wird als Gebet erkannt
public func IsPrayer() { return true; }

//Voraussetzungen
public func Requirements(caller)
{
	//in einem gebäude?
	if(!Contained(caller))
	{
		return false;
	}	
	//Nur Menschen + 2 > Weihwasser in Kirche + in Kirche
	if(caller ->~ Who() != Role_Human || ContentsCount(PHWT,Contained(caller)) <= 1 ||
		!(Contained(caller) ->~ IsHolyPlace()))
	{
		return false;
	}
	// 2 > Leute
	var iCnt = 0;
	for(var pClonk in FindObjects(Find_Container(Contained(caller)),Find_ID(MEDC)))
		if(pClonk -> Who() == Role_Human)
			iCnt++;
	if(iCnt < 2)
		return false;
	return true;
}

public func Apply()
{
	if(Contained())
	{
		pPrist = Contained(this);
		pChurch = Contained(pPrist);
		pPrist -> Exit(this);
		pChurch -> CanEntrance(false,false);
		RemoveObject(FindContents(PHWT,pChurch));
		RemoveObject(FindContents(PHWT,pChurch));
	}
	else return(RemoveObject(this));
}

private func Phase(int iMana)
{
	if(!phase)
		Message("@Scriptus insignis!",pPrist);
	if(phase == 1)
		Message("@Filii tui te salutant",pPrist);
	if(phase == 2)
		Message("@et sacrificium peterent!",pPrist);
	if(phase == 3)
		Message("@te uus vis et virtua debere",pPrist);
	if(phase == 4)
		Message("@et nocta infinis a terra is redinere requimus",pPrist);
	if(phase == 5)
		Message("Amen.",pPrist);
	pPrist->DoMagicEnergy(iMana);
	phase++;
}

//Ablauf
public func Check()
{
	var f = 4;
	if(!pPrist || Contained(pPrist) != pChurch)
		return RemoveObject(this);

	SetPosition(GetX(pPrist),GetY(pPrist));

	if(phase == 5)
	{
		for(var pObj in FindObjects(Find_Container(pChurch)))
		{
			//Alle Items in der Kirche weihen
			if(pObj ->~ IsPoisionCarrier())
			{
				pObj ->~ Impregnate(PHWT);
				continue;
			}
			if(GetID(pObj) == MEDC)
				if(pObj->~Job()->~ManaType() == Magic_Type_Will && pObj->~Who() == Role_Human)
					DoMagicEnergy(100,pObj,true);
		}
		Phase(f);
		pChurch -> CanEntrance(false,true);
		return RemoveObject(this);
	}
	Phase(f);
}