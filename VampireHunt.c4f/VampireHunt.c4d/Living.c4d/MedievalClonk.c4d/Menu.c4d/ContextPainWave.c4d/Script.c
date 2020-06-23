/*-- Schmerzwelle --*/

#strict 2

public func IsAbility(){ return true; }

public func GetSpellCombo() { return "423"; }

public func CanUse(object pCaller)
{
    if (!pCaller->LocalN("prayer"))
        if (pCaller->Who() == Role_Vampire && IsNight() && pCaller->Blood() > 25)
			return true;
}

public func Activate(object pCaller)
{
    //Magiesound und Kosten
    Sound("Magic*",false,pCaller);
	pCaller->Blood(-25);
    //Opfer erleiden Schmerzen und Schreien
	for(var pVictim in FindObjects(Find_ID(MEDC),Find_Distance(100,GetX(pCaller),GetY(pCaller))))
	{
		if(pVictim->Who() == Role_Human)
		{
	        Sound("Die",false,pVictim);
	        pVictim->Pain(30 + Random(25));
	    }
    }
    //Partikel
    for(var x = 180; x >= -180; x-=2)
    {
        CreateParticle("NoGravSpark", GetX(pCaller), GetY(pCaller), Sin(x, 20, 1), Cos(x, 20, 1), 50, RGBa(255, 55, 55, 127));
    }
    //Platzhalter, blockiert kurz die Kraft...
    pCaller->Pray(P_PB);
    Sound("PainWave",false,pCaller);
    //Ende
    return 1;
} 