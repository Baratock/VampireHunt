/*-- Gift --*/

#strict 2

public func IsAbility(){ return true; }

public func GetSpellCombo() { return "261"; }

public func CanUse(object pCaller)
{
    var c = ContentsCount(0,pCaller);
    var conA, conB;
    while (c--)
    {
        if (Contents(c,pCaller)->~IsPoision())
            conA = 1;
        if (Contents(c,pCaller)->~IsPoisionCarrier())
            conB = 1;
    }
    if (conA && conB)
        return true;
}

public func Activate(object pCaller)
{
    var carrier;
    var c = ContentsCount(0,pCaller);
    while (c--)
    {
        if (Contents(c,pCaller)->~IsPoisionCarrier())
            carrier = Contents(c,pCaller);
    }
    CreateMenu(GetID(carrier),pCaller,pCaller);
    var c = ContentsCount(0,pCaller);
    while (c--)
    {
        if (Contents(c,pCaller)->~IsPoision())
        {
			AddMenuItem("$VHDoping$","Poision",GetID(Contents(c,pCaller)), pCaller, 0, carrier);
        }
    }
}

