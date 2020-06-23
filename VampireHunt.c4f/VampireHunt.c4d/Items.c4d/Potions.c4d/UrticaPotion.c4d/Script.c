/* Urticapotion */

#strict 2
//trank
#include PELF

public func Basic()    { return URNT; }
public func Number()   { return 3;    }
public func Intensity(){ return 50;   }

public func Glow(iTime)
{ 
    return(RGB(
    20+Cos(Sin(iTime,200),35),
    10+Sin(Cos(iTime,300),25),
    250-Cos(iTime*2,30)
    ));
}

private func FxPotionEffectStart (object pTarget, int iNumber, int iTemp, int iIntensity)
{
    if(iTemp)
    {
        return 0;
    }
    Sound("healing",false,pTarget);
    EffectVar(0,pTarget,iNumber) = iIntensity;
    pTarget->~Pain(30);
}

private func FxPotionEffectTimer(object pTarget, int iNumber, int iTime) 
{
    if(!(iTime%5))
    {
        EffectVar(0,pTarget,iNumber)--;
        if(!EffectVar(0,pTarget,iNumber) || GetEnergy(pTarget) == pTarget->~MaxHealth())
        { 
            return -1;
        }
        DoEnergy(1,pTarget);
    }  
    CreateSparks(pTarget,iTime);
    return 1;
}
