/* bottle with blood */

#strict 2
#include PHWT

///---Callbacks---///

public func Color()     { return RGBa(230,127,127); }

///---Qualities---///

protected func Initialize()
{
    SetClrModulation(RGB(255,127,127));
}

protected func Activate(object pClonk)
{
    // Sound
    Sound("Drink");
    // unhealty for humans
    if(pClonk -> Who() == Role_Human)
    {
        DoEnergy(-3,pClonk);
        pClonk -> Pain(30);
    }
    // good for ghuls
    if(pClonk -> Who() == Role_Slave)
    {
        pClonk -> DoEnergy(25);
        pClonk -> Feed(100);
    }
    // Vampires & Lichs get their blood back
    if(pClonk -> Who() == Role_Vampire || pClonk -> Who() == Role_Lich)
    {
        pClonk -> Feed(25,true);
    }
    //Action
    pClonk->~Drink(this);
    RemoveObject(this);
    return 1;
}


//arrow-effects

public func Affect(object pTarget)
{
    //Extrapain for humans
    if(pTarget->~Who() == Role_Human)
    { 
        pTarget->~Pain(10);
    }
}
