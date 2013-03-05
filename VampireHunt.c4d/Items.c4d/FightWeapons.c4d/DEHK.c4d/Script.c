/*--- Hirschfänger ---*/

#strict 2
#include SWOR

local iBack;
local fBlock;
local pEnemy;

protected func Hit()
{
	BounceOff();
	Sound("SwordHitd*");
	return 1;
}

public func StrikeSound()
{
	return Sound("SwordHitd*");
}
public func GetStrikeDamage()
{
	return 2;
}
public func GetStrikeTime()
{
	return 1 + Random(2);
}
public func GetBloodLoss()
{
	return Min(Random(5) - 3, 0);
} //1/5

public func ControlLeft(object pController)
{
	ControlRight(pController, 1);
}
//speichern ob man anlauf nimmt
public func ControlRight(object pController, int i)
{
	if (iBack == 2 && GetDir(pController) == DIR_Left + i)
	{
		iBack = 1;
	}
}

public func ControlDigDouble(object pController)
{
	//action läuft noch
	if (GetEffect("Run", pController))
		return 0;
	//mehrfache Aktionen Blocken
	if (!fBlock)
		fBlock = 1;
	else
		return 0;
	//kein Anlauf
	if (iBack == 2)
		iBack = -1;
	//abfangen ob man anlauf nimmt
	if (iBack == 0)
	{
		// und prüfen ob eine Attacke möglich ist
		pEnemy = FindObject2(Find_ID(MEDC), Find_InRect(-40 * (!GetDir(pController)), -10, 40, 20), Find_Exclude(pController), Find_Hostile(GetOwner(pController)));
		if (!pEnemy)
		{
			fBlock = 0;
			return 0;
		}
		iBack = 2;
		ReturnControl(pController, 12);
	}
	//anlauf bearbeiten
	if (iBack == 1)
	{
		iBack = -2;
		return ReturnControl(pController, 10);
	}
	//angriff!
	if (iBack < 0)
	{
		if (iBack == -2)
			AddEffect("Run", pController, 100, 1, this, 0, pEnemy, 10);
		else
			AddEffect("Run", pController, 100, 1, this, 0, pEnemy, 0);
		return 0;
	}
}

private func ReturnControl(object pController, int i)
{
	fBlock = 0;
	Schedule(Format("ControlDigDouble(Object(%d))", ObjectNumber(pController)), i, 1, this);
}

private func FxRunStart(object pTarget, int iEffectNumber, int iTemp, object pEne, int iBonus)
{
	EffectVar(0, pTarget, iEffectNumber) = pEne;
	EffectVar(1, pTarget, iEffectNumber) = iBonus;
	EffectVar(2, pTarget, iEffectNumber) = true;
	//schneller laufen
	SetPhysical("Walk", GetPhysical("Walk", 1, pTarget) * 5 / 3, PHYS_Temporary, pTarget);
	//objekt deaktivieren
	SetCrewEnabled(false, pTarget);
	//loslaufen
	SetAction("Walk", pTarget);
	if (iBonus)
	{
		SetComDir(COMD_None, pTarget);
		SetDir(!GetDir(pTarget), pTarget);
	}
	if (GetDir(pTarget) == DIR_Left)
		SetComDir(COMD_Left, pTarget);
	else
		SetComDir(COMD_Right, pTarget);
}

private func FxRunTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	var pEne = EffectVar(0, pTarget, iEffectNumber);
	//Damit alles abprallt-> Dummyeffekt
	AddEffect("Dummy", pTarget, 1, 1, pTarget);
	//Attacke!
	if (Distance(GetX(pTarget), GetY(pTarget), GetX(pEne), GetY(pEne)) < 5 && EffectVar(2, pTarget, iEffectNumber) == true)
	{
		SetComDir(COMD_None, pTarget);
		EffectVar(2, pTarget, iEffectNumber) = iEffectTime;
		pEne->DoPain(30 + EffectVar(1, pTarget, iEffectNumber));
		SetPhysical("Walk", GetPhysical("Walk", 1, pTarget) * 3 / 5, PHYS_Temporary, pTarget);
		MakeCrewMember(pTarget, GetOwner(pTarget));
		SetCrewEnabled(true, pTarget);
		SelectCrew(GetOwner(pTarget), pTarget, true);
	}
	else
		if (iEffectTime > 120 && EffectVar(2, pTarget, iEffectNumber) == true)
		{
			SetPhysical("Walk", GetPhysical("Walk", 1, pTarget) * 3 / 5, PHYS_Temporary, pTarget);
			EffectVar(2, pTarget, iEffectNumber) = iEffectTime;
			SetComDir(COMD_None, pTarget);
			MakeCrewMember(pTarget, GetOwner(pTarget));
			SetCrewEnabled(true, pTarget);
			SelectCrew(GetOwner(pTarget), pTarget, true);
		}
	//Cooldown
	if (EffectVar(2, pTarget, iEffectNumber) != true)
	{
		if (iEffectTime - EffectVar(2, pTarget, iEffectNumber) > 60)
		{
			//und wieder von neuem
			fBlock = 0;
			iBack = 0;
			return -1;
		}
	}
}
