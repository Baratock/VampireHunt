/*--  Öl, verursacht Schmerzen, vertreibt Müdigkeit und heilt --*/

#strict 2

#include PELF

public func Basic()
{
	return [ELRT, URNT];
}
public func Number()
{
	return [2, 2];
}
public func Intensity()
{
	return 60;
}
public func IsUrticaOil()
{
	return true;
} //für bestimmte Effekte im Clonk

private func FxPotionEffectStart(object pTarget, int iNumber, int iTemp, int iIntensity)
{
	if (iTemp)
	{
		return 0;
	}
	Sound("healing", false, pTarget);
	EffectVar(0, pTarget, iNumber) = iIntensity;
	pTarget->DoPain(25);
}

private func FxPotionEffectTimer(object pTarget, int iNumber, int iTime)
{
	var iInts = EffectVar(0, pTarget, iNumber);
	var iInts2 = EffectVar(1, pTarget, iNumber);
	if (iInts >= Intensity() - 10)
	{
		if (!Random(10))
		{
			pTarget->DoPain(2);
			EffectVar(0, pTarget, iNumber)--;
		}
		return 1;
	}
	if (iInts2 >= Intensity() - 10)
	{
		if (!Random(10))
		{
			pTarget->DoPain(2);
			EffectVar(1, pTarget, iNumber)--;
		}
		return 1;
	}
	else
	{
		EffectVar(0, pTarget, iNumber) += EffectVar(1, pTarget, iNumber);
		EffectVar(1, pTarget, iNumber) -= EffectVar(1, pTarget, iNumber);
		if (!(iTime % 5))
		{
			EffectVar(0, pTarget, iNumber)--;
			if (!EffectVar(0, pTarget, iNumber) || pTarget->~GetVal(cSleep) < 5 || GetEnergy(pTarget) == pTarget->~GetMaxVal(cHealth))
			{
				return -1;
			}
			DoEnergy(Random(2), pTarget);
			pTarget->~DoVal(cSleep, -1);
		}
	}
	CreateSparks(pTarget, iTime);
	return 1;
}

private func FxPotionEffectAdd(object pTarget, int iNumber, string szNewEffectName, int iNewEffectTimer, int iTime)
{
	// Effekt übernehmen
	EffectVar(1, pTarget, iNumber) += iTime;
}