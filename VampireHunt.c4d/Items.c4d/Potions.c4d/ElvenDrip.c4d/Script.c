/*-- Elfentau --*/

#strict 2

public func IsPotion()
{
	return true;
}
public func Basic()
{
	return [ELRT];
}
public func Number()
{
	return [4];
}
public func Intensity()
{
	return 100;
}

protected func Activate(object pClonk)
{
	// Sound
	Sound("Drink");
	// Action
	pClonk->~Drink(this);
	var s = Format("AddEffect(\"PotionEffect\",this,210,1,0,%i,%d);", GetID(this), Intensity());
	Schedule(s, 80, 0, pClonk);
	RemoveObject(this);
	return 1;
}

protected func Hit()
{
	Sound("Crystal*");
}

public func Glow(int iTime)
{
	return RGB(80 + Cos(Sin(iTime, 200), 35), 60 + Sin(Cos(iTime, 300), 25), 80 - Cos(iTime * 2, 30));
}

public func Spark()
{
	return RGBa(0, 64 + Random(3) * 64, 255, 110);
}

private func FxPotionEffectStart(object pTarget, int iNumber, int iTemp, int iIntensity)
{
	if (iTemp)
	{
		return 0;
	}
	EffectVar(0, pTarget, iNumber) = iIntensity;
}
  
private func FxPotionEffectTimer(object pTarget, int iNumber, int iTime)
{
	if (!(iTime % 5))
	{
		EffectVar(0, pTarget, iNumber)--;
		if (!EffectVar(0, pTarget, iNumber))
		{
			return -1;
		}
		pTarget->DoMagicEnergy(1);
		pTarget->DoVal(cSleep, -5);
	}
	CreateSparks(pTarget, iTime);
	return 1;
}

private func CreateSparks(object pTarget, int iTime)
{
	var iX, iY;
	if (!Contained(pTarget))
	{
		for (var i = 0; i < 12; i++)
		{
			var iAng = Random(360);
			iX = Sin(iAng, 15);
			iY = Cos(iAng, 15);
			CreateParticle("PSpark", GetX(pTarget) + iX, GetY(pTarget) + iY, -iX / 7 + Random(6) - 3, -iY / 7 + Random(6) - 3, 25 + Random(15), Glow(iTime) | 20 << 24, pTarget);
		}
	}
}

private func FxPotionEffectEffect(string szNewEffectName)
{
	// gleicher effekt wird aufsummoert (z.B. wenn man zwei Tränke trinkt)
	if (szNewEffectName == "ManaPotion")
		return -2;
}

private func FxPotionEffectAdd(object pTarget, int iNumber, string szNewEffectName, int iNewEffectTimer, int iTime)
{
	// Effekt übernehmen
	EffectVar(0, pTarget, iNumber) += iTime;
}    
