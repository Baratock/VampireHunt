/*-- Schmied --*/

#strict 2

//Basics

public func IsCitizien()
{
	return true;
}
public func OnlyOne()
{
	return false;
}
public func Health()
{
	return 100;
}
public func Pain()
{
	return 100;
} 
public func Sleep()
{
	return 100;
}
public func Blood()
{
	return 100;
}
public func ManaType()
{
	return cMagicWill;
}
public func Mana()
{
	return 10;
}
public func Home()
{
	return HUT3;
}
public func Graphics()
{
	return SMTH;
}
public func Things()
{
	return [];
}
public func Knowledge()
{
	return [];
}
public func Use()
{
	return [];
}

//Allgemeine Standard Funktionen

public func Magic(object pClonk)
{
	if (pClonk && GetAction(pClonk) == "Dead" && pClonk->GetVal(cPain) < Pain())
	{
		DoMagicEnergy(1, pClonk);
	}
	return 1;
}
public func Check()
{
	var f = true;
	for (var i = 0; i < GetPlayerCount(C4PT_User); i++)
		if (GetCursor(GetPlayerByIndex(i, C4PT_User))->GetRole() == GetID(this))
			f = false;
	if (!OnlyOne())
		f = true;
	return f;
}

public func Init(object pClonk)
{
	if (Contained(pClonk))
	{
		Contained(pClonk)->Exit(pClonk);
	}
	//In ein zufälliges Haus
	if (Home())
	{
		var pHouse = FindObjects(Find_ID(Home()))[Random(GetLength(FindObjects(Find_ID(Home()))))];
		Enter(pHouse, pClonk);
	}
	//Wissen	
	for (var IDdef in Knowledge()) 
		SetPlrKnowledge(GetOwner(pClonk), IDdef);
	//Items
	for (var IDdef in Things()) 
		CreateContents(IDdef, pClonk);
	//Physicals + Energie
	SetPhysical("Energy", 1000 * Health(), PHYS_Temporary, pClonk);
	SetPhysical("Magic", 1000 * Mana(), PHYS_Temporary, pClonk);
	DoEnergy(100, pClonk);
}

public func CanUse(id idDef)
{
	var idObjs = Use();
	if (!idObjs)
		return false;
	for (var idObj in idObjs) 
		if (idDef == idObj)
			return true;
}

public func Things()
{
	return [METL, METL, COAL, COAL, WOOD, WOOD];
}
public func Knowledge()
{
	return [ARWP, STAK, SWOR, BOW1, AXE1, SPER, CRBW, SBRE, BOTP, SHIE, ARMR, BRMR];
}

public func Support(object pClonk)
{
	Init(pClonk);
	Sound("der amboss", false, pClonk);
	SetGraphics(0, pClonk, SMTH);
}
