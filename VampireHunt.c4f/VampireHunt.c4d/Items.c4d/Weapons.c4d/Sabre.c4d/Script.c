/* - Sabre -*/

#strict 2
#include SWOR

protected func Hit()
{
    BounceOff();
    Sound("MetalHit*");
    return 1;
}

public func StrikeSound()        { return Sound("SabreHit*"); }
public func GetStrikeDamage(){ return 6+Random(1); }
public func GetBloodLoss()   { return 5+Random(2); }
public func GetCausedPain()  { return 14+Random(1); }
public func GetStrikeTime()  { return 5-Random(2); }
{
    //1 + 1/3
    return 3+Max(Random(2)-1,0);
}


