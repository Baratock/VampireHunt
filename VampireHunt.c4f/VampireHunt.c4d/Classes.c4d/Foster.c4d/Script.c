/*-- Förster --*/
 
#strict 2

#include J_VG

public func OnlyOne()     { return true;}
public func MaxLevel()    { return 4;}
public func Health()      { return [170,175,180,185];}
public func Pain()        { return [110,115,120,125];} 
public func ManaType()    { return Magic_Type_Will;  }
public func MaxMana()     { return [20,20,20,20]; }
public func Requirements(){ return HUT2;}
public func Things()      { return [BOW1,ARWP,ARWP,SPER];}
public func Knowledge()   { return [PBAL,SPER,ARWP,WEQ7,URNT,POIS];}

public func Support(object pClonk)
{
    Init(pClonk);
    pClonk -> Sound("ich werde ihn");
    SetGraphics(0,pClonk,FORS);
    PlayerMessage (GetOwner(pClonk),"$Foster$");
}
