/*--- Bajonett ---*/

#strict 2
#include SWOR

local ialpha;

///---Callbacks---///

public func CarryLimit()     { return -1; }
public func GetStrikeTime()  { return  3-Random(1); }
public func GetStrikeDamage(){ return  4+Random(4); }
public func GetCausedPain()  { return  11-Random(1); }

///---Timercall---///

protected func CheckRemoval()
{
    if(Contained(this) && !(GetOCF(Contained(this)) & OCF_Living))
    {
        RemoveObject(this); 
    }
    if(Contained(this))
    {
        return 1;
    }
    ialpha+=3;
    SetRGBaValue(GetColorDw(this),ialpha*2, 0);
    if(ialpha > 65)
    {
        RemoveObject(this);
        return 1;
    } 
}

protected func Hit()
{
    Sound("MetalHit*");
    SetRDir(0,this);
    SetXDir(0,this);
    SetYDir(0,this);
    return 1;
}

private func Throw(int iDir)
{
    //up
    SetPosition(GetX(this) + 10*iDir,GetY(this) + 7);
    //sound
    Sound("Catapult");
    //faster throw
    SetXDir(GetXDir(this)*8);
    //horizontal
    SetYDir(-10);
    //roation
    SetRDir(GetXDir(this));
}


