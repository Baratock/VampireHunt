/*-- Draum fressen --*/

#strict 2

public func IsAbility(){ return true; }

public func GetSpellCombo() { return "411"; }

public func CanUse(object pCaller)
{
    if (pCaller->Who() != Role_Lich)
        return false;
    if (pCaller->Blood() < 10 || !IsNight() || GetAction(pCaller) != "Walk")
        return false;
    var aClonks = FindObjects(Find_ID(MEDC), Find_Distance(100,GetX(pCaller),GetY(pCaller)), Find_Action("Dead"), Find_Exclude());
    if (GetLength(aClonks))
        return true;
}

public func Activate(object pCaller)
{
    var aClonks = FindObjects(Find_ID(MEDC), Find_Distance(100,GetX(pCaller),GetY(pCaller)), Find_Action("Dead"), Find_Exclude());
    for (var pClnk in aClonks) 
    {
        if(pCaller->Blood() < 8)
            break;
        pCaller->Blood(-8);
        pClnk->~Pain(15 + Random(15));
        pCaller->RestorePain(19);
        DoEnergy(-GetEnergy(pClnk) / 4, pClnk, false, FX_Call_EngDarkMagic);
        pClnk->~RestoreSleep(30);
        if (Random(3))
            pClnk->CntextAwake();
        var angle = Angle(GetX(pClnk), GetY(pClnk), GetX(), GetY());
        pClnk->CreateParticle("PSpark", RandomX(-3, 3), -1, Sin(RandomX(-3, 3) + angle, 10), -Cos(RandomX(-3, 3) + angle, 10), RandomX(40, 80), RGB(90, 0, 200));
        pClnk->CreateParticle("PSpark", RandomX(-3, 3), -1, Sin(RandomX(-3, 3) + angle, 10), -Cos(RandomX(-3, 3) + angle, 10), RandomX(40, 80), RGB(90, 0, 200));
        pClnk->CreateParticle("PSpark", RandomX(-3, 3), 2, Sin(RandomX(-3, 3) + angle, 10), -Cos(RandomX(-3, 3) + angle, 10), RandomX(40, 80), RGB(90, 0, 200));
        pClnk->CreateParticle("PSpark", RandomX(-3, 3), 2, Sin(RandomX(-3, 3) + angle, 10), -Cos(RandomX(-3, 3) + angle, 10), RandomX(40, 80), RGB(90, 0, 200), pClnk);
    }
    return true;
}
