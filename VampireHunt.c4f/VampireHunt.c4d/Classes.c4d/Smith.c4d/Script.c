/*-- Schmied --*/

#strict 2

#include J_VG

public func MaxLevel()    { return 4;}
public func Health()      { return [105,110,112,115];}
public func Pain()        { return [105,110,115,117];} 
public func ManaType()    { return Magic_Type_Will;}
public func MaxMana()     { return 0;}
public func Requirements(){ return HUT3;}
public func Things()      { return [METL,METL,COAL,COAL,WOOD,WOOD];}
public func Knowledge()   { return [ARWP,STAK,SWOR,BOW1,FARP,AXE1,SPER,CRBW,SBRE,BOTP];}
public func SmithExp()      { return 3;}

public func Support(object pClonk)
{
    Init(pClonk);
    pClonk -> Sound("der amboss");
    SetGraphics(0,pClonk,SMTH);
}

//Regeneration:Keine
public func Magic(object pClonk){ return 1;}

