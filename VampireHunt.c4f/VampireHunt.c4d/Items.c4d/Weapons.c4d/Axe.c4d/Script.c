/*-- Axt --*/

#strict 2

//------------------!System-------------------------------

///---Callbacks---///

public func IsFightWeapon()   { return true; }
public func IsAnvilProduct()  { return true; }
public func IsPoisionCarrier(){ return !poision; }
public func GetFightAction()  { return "AxeFight"; }
public func GetStrikeAction() { return "AxeStrike"; }
public func StrikeSound()     { return Sound("AxeHit*"); }


///---Qualities---///

//locals
local poision;

protected func Initialize()
{
    poision = 0;
}

// The object leaving his container
protected func Departure(object pTarget)
{
    if(!CanBeThrown()) 
    { 
        return 1;
    }
    if(GetXDir(this) && GetOCF(pTarget) & OCF_Living)
    {
        Throw(GetDir(pTarget)*2-1);
    }
    //Andernfalls hängen manche Sachen in der Luft
    else 
    { 
        SetAction("Idle");
    }
}

//if someone throw this
private func Throw(int iDir)
{
    if(Contained(this))
    { 
        return SetAction("Idle");
    }
    //sound
    Sound("Catapult");
    //faster throw
    SetXDir(GetXDir(this)*4/3);
    //horizontal
    SetYDir(-10);
    //roation
    SetRDir(GetXDir(this));
}

//if it hit an clonk
public func BounceOff()
{
    SetRDir(-GetRDir(this)/3);
    SetYDir(GetYDir(this)/2);
    SetXDir(-GetXDir(this)/3);
}

public func HitEffect(object pClonk) 
{
    if(GetEffect("BLOCK",this))
    {
        return 1;
    }
    AddEffect("BLOCK",this,1,20,this);
    DoEnergy(-GetStrikeDamage()/RandomX(2,3),pClonk);
    pClonk->Pain(GetCausedPain()*RandomX(2,3));
    //target is still living?
    if(!pClonk)
        return 1;
    UsePoision(this,poision,pClonk);
    Hit();
}

protected func FxBLOCKTimer(){ return -1; }

public func MeleeEffect(object pTarget)
{
    DoEnergy(-GetStrikeDamage(),pTarget);
    if(!pTarget)
        return 1;
    pTarget->~LoseBlood(GetBloodLoss());
    pTarget->Pain(GetCausedPain());
    UsePoision(this,poision,pTarget);
}

public func Impregnate(id IDdef)
{
    if(!IDdef || !IDdef->IsPoision())
    { 
        return 0;
    }
    var dwClr = IDdef ->~ Color();
    SetClrModulation(dwClr,this);
    var szname = Format("%s-%s",GetName(0,IDdef),GetName(this));
    SetName(szname);
    poision = IDdef;
}

//--------------------------!Axe-----------------------------

protected func Hit()
{
    BounceOff();
    Sound("AxeHit*");
    return 1;
}

public func CanBeThrown()    { return true; }
public func GetStrikeDamage(){ return 9+Random(3); }
public func GetCausedPain()  { return 39+Random(13);}
public func GetStrikeTime()  { return 9-Random(5); }
public func GetBloodLoss()   { return 4; }
