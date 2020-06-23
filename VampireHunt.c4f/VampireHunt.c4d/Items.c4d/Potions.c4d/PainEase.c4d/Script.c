/* Baldrianextrakt */

#strict 2

//imprägnierung
#include PHWT
//trank
#include PELF


//Trankteil

public func Basic() { return M_B2; }
public func Number(){ return 3;    }

private func FxPotionEffectTimer(object pTarget, int iNumber, int iTime) 
{
	if(!(iTime%5))
	{
		if(!--EffectVar(0,pTarget,iNumber)) 
			return -1;
		pTarget->~RestoreSleep(-1);
		pTarget->~RestorePain(1);
	}  
	CreateSparks(pTarget,iTime);  
	return 1;
}

//Imprägnierungsteil

public func Color()  { return(RGBa(107,107,107)); }

public func Affect(object pTarget)
{
	pTarget ->~ RestoreSleep(-8);
	pTarget ->~ RestorePain(8);
}