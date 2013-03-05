/*-- Baldrianextrakt --*/

#strict 2

//imprägnierung
#include PHWT
//trank
#include PELF

public func Basic()
{
	return [M_B2];
}
public func Number()
{
	return [3];
}

private func FxPotionEffectTimer(object pTarget, int iNumber, int iTime)
{
	if (!(iTime % 5))
	{
		if (!--EffectVar(0, pTarget, iNumber))
			return -1;
		pTarget->DoVal(cSleep, 1);
		pTarget->DoVal(cPain, -1);
	}
	CreateSparks(pTarget, iTime);
	return 1;
}

//Imprägnierungsteil

public func Color()
{
	return RGBa(107, 107, 107);
}

public func Affect(object pTarget)
{
	pTarget->DoVal(cSleep, 9);
	pTarget->DoVal(cPain, -6);
}