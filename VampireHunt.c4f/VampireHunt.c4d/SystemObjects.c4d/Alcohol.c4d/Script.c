/*-- Alkohol aus Western geklaut, wird aber nicht includiert,
 da Western.c4d nicht benötigt werden sollte --*/

#strict 2

///---Global function---///

global func DoAlcohol(object pClonk, int Strength)  // trinkender Clonk, Stärke des Getränks
{
    // Alkoholeffekt hinzufügen
    AddEffect("Alcohol", pClonk, 101, 1, 0, ALCE, Strength);
}

///---Effects---///

public func FxAlcoholStart(target, no, temp, alcohol) // alcohol = Stärke des Getränks
{
    if(!temp)
    {
        EffectVar(0, target, no) = alcohol;
        EffectVar(1, target, no) = 0;
    }
}

public func FxAlcoholTimer25(target, no)
{
    // zu viel Alkohol zieht Lebensenergie ab
    if(EffectVar(0, target, no) >= 2500)
    {
        DoEnergy((2500-EffectVar(0, target, no))/50, target);
        //wenn tot -> beenden
        if(!target)
            return -1;
        EffectVar(0, target, no) = 2499;
    }
    //springt verschieden weit
    if(GetAction(target) == "Jump")
        SetXDir(GetXDir(target)+RandomX(-3,3), target);  
    // viel Alkohol -> manchmal lässt er Dinge fallen
    if(EffectVar(0, target, no) >= 1900 && !Random(25) && Contents(0, target))
        Exit(Contents(0), AbsX(GetX(target)), AbsY(GetY(target))+10, RandomX(-10,10), RandomX(-1,1), 0, RandomX(-10,10));
}

public func FxAlcoholTimer(target, no, time)
{
    if(EffectVar(0, target, no) <= 0) return -1;
    // Alkoholgehalt nimmt ab
    EffectVar(0, target, no) -= time - EffectVar(1, target, no);
    EffectVar(1, target, no) = time;

    if(!(time%25)) EffectCall(target, no, "Timer25", target, no);
    // Sichtschwankungen
    if(GetCursor(GetOwner(target)) == target)
    {
        SetViewOffset(GetOwner(target), Sin(time*10, EffectVar(0, target, no)/100));
    }
    else
    {
        SetViewOffset(GetOwner(target));
    }
}

public func FxAlcoholEffect(name)
{
    if(WildcardMatch(name, "*Alcohol*")) return -2;
}

public func FxAlcoholAdd(target, no, name, timer, alcohol)   // Alkoholeffekte überladen sich
{
    EffectVar(0, target, no) += alcohol;
}
