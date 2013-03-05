/*-- Löschobjekt --*/

#strict 2

public func Check()
{
	if (GetAction(this) != "Idle")
	{
		return 0;
	}
	for (var pObj in FindObjects(Find_OCF(OCF_OnFire))) 
	{
		if (GetAlive(pObj) && !GetEffect("Extinguisher", pObj))
		{
			AddEffect("Extinguisher", pObj, 20, 5, 0, GetID(this));
		}
	}
	return 1;
}

public func FxExtinguisherTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	if (!GetAlive(pTarget))
	{
		return -1;
	}
	if (iEffectTime > 105 && !Random(4))
	{
		Extinguish(pTarget);
		return -1;
	}
}

protected func Activate(int iForPlr)
{
	MessageWindow(GetDesc(this), iForPlr);
	return 1;
}

