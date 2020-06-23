/*-- Bürger --*/

#strict 2

///----- Allgemeine Funktionalität zum Weitervererben -----///

//--- Prototypen ---//

public func IsJob()       { return true;}
public func OnlyOne()	  	{ return false;}
public func MaxLevel()    { return 4;}
public func Health()      { return [105,110,115,120];}
public func Pain()	      { return [120,125,127,131];} 
public func ManaType()    { return Magic_Type_Will;  }
public func MaxMana()     { return [15,15,15,15]; }
public func Requirements(){ return HUT3;}
public func Things()	  	{ return [BOW2,ARWP,ARWP];}
public func Knowledge()	  { return [ARWP,STAK,WEQ7];}
public func SmithExp()		{ return 1;}

//--- Helfer ---//

public func MaxHealth(iLevel){	return Health()[iLevel]; }
public func PainLimit(iLevel){	return Pain()[iLevel];   }

//--- Allgemein geschriebene Standard Funktionen ---//

public func Magic(object pClonk)
{
	if(pClonk && GetAction(pClonk) == "Dead" && pClonk-> RestorePain() < Pain()[0])
	{
		DoMagicEnergy(1,pClonk);
	}
	return 1;
}
public func Check()
{
	var f = true;
	for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
		if(GetCursor(GetPlayerByIndex(i,C4PT_User))->~Job() == GetID()) f = false;
	if(!OnlyOne())
		f = true;
	return f;
}

public func Init(object pClonk)
{
	if(Contained(pClonk))
	{
		Contained(pClonk)-> Exit(pClonk);
	}
	pClonk -> House(FindObjectOwner(Requirements(),NO_OWNER));
	Enter(pClonk ->~ House(),pClonk);
	
	for(var IDdef in Knowledge())
		SetPlrKnowledge(GetOwner(pClonk),IDdef);
	for(var IDdef in Things())
		CreateContents(IDdef,pClonk);
	
	if(IsJob())
	{
		pClonk -> HumanForm();
		DoEnergy(120,pClonk);
	}
	else
	{
		pClonk->BecomeVampire();
		DoEnergy(200,pClonk);
		SetWealth(GetOwner(pClonk),2);
	} 
}


public func Support(object pClonk)
{
	Init(pClonk);
	pClonk -> Sound("bürger");
	SetGraphics(0,pClonk,KLIK);
}
