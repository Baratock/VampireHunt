/*-- Verband --*/

#strict 2

protected func Activate(object pCaller)
{
	//stimmt die Klasse?
	if (pCaller->GetRole() != J_AL)
		return 0;
	var pTarget = GetTarget(pCaller);
	//läuft/steht und Ziel ist da
	if (GetAction(pCaller) != "Walk" || pTarget == 0)
		return 0;
	Affect(pTarget);
	return RemoveObject(this);
}

private func Affect(object pTarget)
{
	AddEffect("DecBloodLose", pTarget, 122, 10, 0, GetID(this));
	pTarget->DoVal(cBlood, 5);
}

protected func FxDecBloodLose(object pTarget, int iEffectNumber, int iEffectTime)
{
	if (iEffectTime > 400 || pTarget->GetVal(cBlood) == pTarget->GetMaxVal(cBlood))
		return -1;
	pTarget->DoVal(cBlood, 1);
}

//Liefert das Ziel
private func GetTarget(object pCaller)
{
	var pClonks = FindObjects(Find_ID(GetID(pCaller)), Find_Not(Find_Hostile(GetOwner(pCaller))), Find_Distance(15), Find_Action("Dead"), Find_Exclude(pCaller));
	//Element mit dem größten blutverlust
	var iBloodlose = 0;
	//Größten Blutverlust finden
	for (var i = 0; i < GetLength(pClonks); i++)
	{
		if (pClonks[i]->GetVal(cBlood) < pClonks[iBloodlose]->GetVal(cBlood))
		{
			iBloodlose = i;
		}
	}
	return pClonks[iBloodlose];
}