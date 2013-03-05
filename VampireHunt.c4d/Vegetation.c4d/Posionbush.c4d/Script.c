#strict 2

#include TREE

//special plants beware the species for the extinction...
local fOrg;

///---Callbacks---///

private func ReproductionAreaSize()
{
	return 80;
}
private func ReproductionRate()
{
	return 15;
}
private func MaxTreeCount()
{
	return 4;
}

protected func Damage(){}
protected func Incineration()
{
	if (fOrg)
	{
		return Extinguish(this);
	}
	SetCategory(C4D_Object);
}

public func IsStanding()
{
	return GetCategory(this) & C4D_StaticBack;
}
public func IsAlchemContainer()
{
	return false;
}

///---Qualities---///

public func GetOriginal()
{
	return fOrg;
}

public func SetOriginal(bool flag)
{
	fOrg = flag;
}


public func Construction()
{
	SetAction("Exist");
	//random animation (e.g. left or front)
	SetDir(Random(4));
	SetPosition(GetX(this), GetY(this) - 5);
	var dwRGB = HSL(Random(38), Random(128) + 127, Random(94) + 64);
	//some variation with the colour
	SetColorDw(dwRGB, this);
}

public func Reproduction()
{
	//only the original
	if (!fOrg)
	{
		return 1;
	}
	// enough space?
	var iR = ReproductionAreaSize() / 2;
	if (GetLength(FindObjects(Find_ID(GetID(this)), Find_Distance(iR))) < MaxTreeCount())
	{
		// OK, create plant
		InitObject();
		return 1;
	}
	// no space ;'(
	return 0;
}

private func InitObject()
{
	var ia = ReproductionAreaSize();
	var pShrm = CreateObject(GetID(this), 0, 0, NO_OWNER);
	SetXDir(Random(ia / 3) - ia / 6, pShrm);
	SetYDir(Random(ia / 3) - ia / 5, pShrm);
	SetCon(5, pShrm);
	pShrm->Pick();
	return pShrm;
}

public func ContextChop(object pClonk)
{
	[$TxtPick$|Image=MUSH|Condition=IsStanding]
	if (!fOrg)
	{
		Pick();
	}
	return 1;
}

protected func RejectEntrance()
{
	if (fOrg || GetCon(this) < 100)
	{
		return 1;
	}
}

// pick through entrance
public func Entrance()
{
	[$TxtPick$]
	if (IsStanding())
	{
		Pick();
	}
	return 1;
}

public func Pick()
{
	if (GetCon() >= 100)
	{
		Sound("Grab");
	}
	SetAction("Exist");
	SetDir(GetDir(this));
	SetCategory(C4D_Object, this);
}

protected func Hit()
{
	//antistuck
	if (GBackSolid(0, 5))
	{
		SetCategory(C4D_StaticBack | 32768); //C4D_SelectVegetation == 32768
		SetPosition(GetX(this), GetY(this) + 5);
	}
	if (GetCon(this) >= 100)
	{
		Sound("WoodHit*");
	}
}

protected func Existing()
{
	if (IsStanding())
	{
		Reproduction();
	}
}
