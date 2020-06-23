/*--  Oil from Urticaessel --*/

#strict 2
#include PELF

public func Basic()    { return [ELRT,URNT];}
public func Number()   { return [2,2];      }
public func Intensity(){ return 60;  		}
//for special effects in the clonk
public func IsUrticaOil(){ return true; }

private func FxPotionEffectTimer(object pTarget, int iNumber, int iTime) 
{
	var iInts = EffectVar(0,pTarget,iNumber);
	var iInts2= EffectVar(1,pTarget,iNumber);
	if(iInts >= Intensity()-10)
	{
		if(!Random(10))
		{
			pTarget->~Pain(3);
			EffectVar(0,pTarget,iNumber)--;
		}
		return 1;
	}
	if(iInts2 >= Intensity()-10)
	{
		if(!Random(10))
		{
			pTarget->~Pain(3);
			EffectVar(1,pTarget,iNumber)--;
		}
		return 1;		
	}
	else
	{
		EffectVar(0,pTarget,iNumber) += EffectVar(1,pTarget,iNumber);
		EffectVar(1,pTarget,iNumber) -= EffectVar(1,pTarget,iNumber);
		if(!(iTime%5))
		{
			EffectVar(0,pTarget,iNumber)--;
			if(!EffectVar(0,pTarget,iNumber) || pTarget->~RestoreSleep() < 5)
			{ 
				return -1;
			}
			pTarget->~RestoreSleep(1);
		}
	}  
	CreateSparks(pTarget,iTime);
	return 1;
}

private func FxPotionEffectAdd(object pTarget, int iNumber, string szNewEffectName, int iNewEffectTimer, int iTime)
{
	// Effekt übernehmen
	EffectVar(1,pTarget,iNumber) += iTime;
} 