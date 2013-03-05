/*-- Heiltrank --*/

#strict 2

public func IsPotion()
{
	return true;
}
public func Basic()
{
	return [MUSH];
}
public func Number()
{
	return [2];
}

protected func Hit()
{
	Sound("Crystal*");
}

protected func Activate(object pCaller)
{
	//stimmt die Klasse?
	if (pCaller->GetRole() != J_AL)
		return 0;
	var pTarget = GetTarget(pCaller);
	//läuft/steht und Ziel ist da
	if (GetAction(pCaller) != "Walk")
		return 0;
	// Sound
	Sound("Drink");
	// Action
	if (pTarget == pCaller)
		pTarget->~Drink(this);
	var s = Format("AddEffect(\"PotionEffect\",this,210,1,0,%i);", GetID(this));
	Schedule(s, 80, 0, pTarget);
	return RemoveObject(this);
}

//Liefert das Ziel
private func GetTarget(object pCaller)
{
	var pClonks = FindObjects(Find_ID(GetID(pCaller)), Find_Not(Find_Hostile(GetOwner(pCaller))), Find_Distance(15), Find_Action("Dead"));
	//Element mit dem größten Schaden
	var iMax = 0;
	//Größten Schaden finden
	for (var i = 0; i < GetLength(pClonks); i++)
	{
		if (pClonks[i]->GetMaxVal(cHealth) - GetEnergy(pClonks[i]) > pClonks[iMax]->GetMaxVal(cHealth) - GetEnergy(pClonks[iMax]))
		{
			iMax = i;
		}
	}
	return pClonks[iMax];
}

private func FxPotionEffectStart(object pTarget, int iNumber, int iTemp)
{
	if (iTemp)
	{
		return 0;
	}
	EffectVar(0, pTarget, iNumber) = GetEnergy(pTarget);
	Sound("healing", false, pTarget);
}

private func FxPotionEffectTimer(object pTarget, int iNumber, int iTime)
{
	var iEnergyChange = GetEnergy(pTarget) - EffectVar(0, pTarget, iNumber);
	//mehr als 1/3 geheilt oder volle Energie
	if (iEnergyChange > pTarget->~GetMaxVal(cHealth) / 3 || GetEnergy(pTarget) == pTarget->~GetMaxVal(cHealth))
	{
		return -1;
	}
	DoEnergy(Random(2), pTarget);
	return 1;
}
