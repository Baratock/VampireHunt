/*-- Mittelalter-Clonk --*/

#strict 2

#include KNIG

//---------------------Überladenes----------------------------------

//Allerlei locals
local vampire; //Typ: 0=Mensch, 1=Der Vampir, 2=Ghul 3=Lich
local blood; //Blut
local sleep; //Müdigkeit
local pain; //Schmerz
local hunger; //HUNGER
local job; //ID der Rolle
local house; //Behausung
local partner,form,human; //Ghul bzw Meister des Clonks, Form des ghouls
local power,prayer; //Aktive Kraft bzw Gebet
local iExperience, iLevel; //Erfahrungs und Stufensystemvariablen
local Cooldown, Cooltimer; //Abklingzeit in 12 Frame schritten und Momentane Zeit
local iPlayers; //Anzahl der Spieler bei Start

//ini mit blut

/*Idee:
*   -Soll Verlorene Lebensenergie als Schmerz weiterleiten
*   -Aber nicht alle, also manche Typen nicht, wie zum Beispiel Schaden durch die Sonne. Siehe VampireForm() Oder LichForm()
*   -Sollte bei manchen Typen weniger oder mehr Schmerz bekommen.
*/

public func Initialize()
{
    blood = 100;
    iLevel = 1;
    ResetCoolDown();
    iPlayers = GetPlayerCount();
    var hud = CreateObject(SHUD,0,0,NO_OWNER);
    hud->SetClonk(this);
    AddEffect("CalcPain", this, 1, 0, this, 0);
    AddEffect("IntCoolDown", this, 1, 12, this, 0);
    return _inherited();
}

protected func RejectCollect(id idObject, object pObject)
{
    if(GetEffect("FadeOut",pObject))
    {
        return true;
    }
    else
    {
        return inherited(idObject,pObject); 
    }
}

//Schmerzeffect
static const FX_Call_EngSun = 41;               //Schaden durch Sonnenbrand
static const FX_Call_EngDarkMagic = 42; //Schaden durch Schwarze Magie
static const FX_Call_EngHoly = 43;          //HeiligerSchaden

protected func FxCalcPainDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause)
{
    //DebugLogs
    //Log("%v",iDmgEngy);
    
    //schadensfaktor für vamp abhängig von der spielerzahl
    if(pTarget->Who() == Role_Vampire)
        iDmgEngy = iDmgEngy*CalcPlayerValue(iDmgEngy)/50;
        
    //schaden bei bes. effekten  
    iDmgEngy = iDmgEngy * DamageMod() / 100;
       
    //Schmerz auf Extra Variable legen um Bugs zu verhindern
    var iPain = iDmgEngy;
        
    //der Vampir darf nicht Sterben wenn er zu wenig leben hat
    if(pTarget->~Who() == Role_Vampire && iCause != FX_Call_EngSun && iCause != FX_Call_EngFire)
        if(-iDmgEngy/1000 >= GetEnergy(pTarget))
            iDmgEngy = 0;
    //Bei Objhits keinen Schaden
    if(iCause == FX_Call_EngObjHit)
    {
        return 0;
    }    
    //der Vampire und der Lich sollte bei Feuer mehr schmerz erhalten
    if(iCause == FX_Call_EngFire && pTarget->~Who() != Role_Human)
    {
        if(Random(2) == 0)
            pTarget->Pain(-iPain/840,true);
        return iDmgEngy*1/4;
    }
    else if (iCause == FX_Call_EngFire)
    {
        if(Random(4) == 0)
            pTarget->Pain(-iPain/840,true);
        return iDmgEngy*1/6;
    }
	
    //Beim Ersticken
    if(iCause == FX_Call_EngAsphyxiation)
    {
        if(pTarget->~Who() != Role_Human)
            return 0;
        else
        {
            pTarget->Pain(-iPain/1500,true);
            return iDmgEngy*1/3;
        }		
    }
    //Schmerz und Schaden durch Sonne
    if(iCause == FX_Call_EngSun && (pTarget->~Who() == Role_Vampire || pTarget->~Who() == Role_Lich))
    {
        return iDmgEngy;
    }
    	
    //Schwarze Magie macht Menschen mehr schaden
    if(iCause == FX_Call_EngDarkMagic)
    {
        if(pTarget->~Who() == Role_Human)
        {
            pTarget->~Pain(-iPain/700,true);
            return iDmgEngy*6/5;
        }
        return iDmgEngy;
    }
    return iDmgEngy;
}

// -------------------------  Helfer für das Schadenssystem ---------------------------

private func DamageMod()
{
    if(CheckAntiSleep(false))
        return 130;
    if(GetEffect("PoisionFern",this) || GetEffect("PoisionNah",this) )
    {
        // Von Effektzahl abwärts zählen, da Effekte entfernt werden
        var iN = GetEffectCount("PoisionNah", this);
        var iF = GetEffectCount("PoisionFern", this);
        return Max(100+iN*27+iF*11,160);
    }
    //versucht einen angriff mit dem hirschfänger
    if(GetEffect("Run",this))
        return 140;
    //sprintet
    if(GetEffect("Dash",this))
        return 140;      
    return 100;
} 

private func CanStartAntiSleep()
{
    if(Job() == J_VH)
        return !CheckAntiSleep(false);
}

private func CheckAntiSleep(bool fActiv)
{
    if(fActiv)
    {
        if(GetEffect("AntiSleep",this,,6) > 30*37)
        {
            return false;
        }
        return true;
    }
    if(GetEffect("AntiSleep",this))
        return true;
    return false;
}

//1. Entf. 2. 150% 3. 130% 4. 110% 5. 90% 6. 80% 7. 75% 8. 70% 9. 65% 10. 60% >11. 55%
private func CalcPlayerValue(int iValue)
{
    //spielerzahl
    var x = iPlayers;
    if(x == 1)
    	x = 90;
    //minimale Anzahl an Dorfler finden
    if(x == 2)
        x = 120;
    else if(Inside(x,3,5))
        x = 140 - 20*x;
    else if(Inside(x,6,11))
        x = 105 - 5*x;
    else if(x > 11)
        x = 70;
    //nun zurückgeben
    return x;
}


// ------------------------- -------------------------------------

// Allgemeine Waffen
public func CanUse(ID)
{
    //Alles was sich als Nahkampfwaffe ausweist.
    if (ID->~IsFightWeapon())
        return true;
    // Bogen 
    if (ID == BOW1)
        return true;
	// Kurzbogen
	if (ID == BOW2)
		return true;
    // Schild 
    if (ID == SHIE)
        return true;
    //Alles was sich als Speer ausweist.
    if (ID->~IsSpear())
        return true;
    //KLassenspezielle Gegenstände
    if(Job()->~CanUse(ID))
        return true;
    return 0;
} 

//Mehr Inventarplätze
private func MaxContentsCount()
{
    return 5 - 1 * HasArmor();
} 

// --------------------------Altes Kontextmenü weg----------------------------------

public func Blocked()
{
    return false;
} //Zum deaktivieren des normalen kontexts

public func ContextUnbuckle()
{
    [$CtxUnbuckleDesc$|Image=KMSU|Condition=Blocked]
    ShowUnbuckleMenu();
    return 1;
}

public func ContextAttack()
{
    [$CtxAttackDesc$|Image=MI_A|Condition=Blocked]
    // Nächstbeste feindliche Einheit angreifen 
    SetOrderAttack(FindEnemyUnit());
    return 1;
} 
 
public func ContextDefend()
{
    [$CtxDefendDesc$|Image=MI_D|Condition=Blocked]
    // Diese Position verteidigen 
    SetOrderDefend(GetX(), GetY());
    return 1;
} 
 
public func ContextGuard()
{
    [$CtxGuardDesc$|Image=MI_A|Condition=Blocked]
    // Diese Position bewachen 
    SetOrderGuard(GetX(), GetY());
    return 1;
} 
 
public func ContextArm()
{
    [$CtxArmDesc$|Image=KMIA|Condition=Blocked]
    // Waffen suchen und anlegen 
    SetOrderArm();
    return 1;
} 

//------------------------------Timer Call---------------------------------------

public func Check()
{
    if (GetID(Contained()) == SELE)
        return SetPlrViewRange(500);
    if (GetID(Contained()) == VHRG)
        return SetPlrViewRange(0);
    if (!vampire)
    {
        HumanForm();
    }
    if (vampire == Role_Lich)
    {
        LichForm();
    }
    if (vampire == Role_Slave)
    {
        if (form)
            GhoulForm();
        else 
            HumanForm();
    }
    if (vampire == Role_Vampire)
    {
        VampireForm();
    }
    //Magieverarbeitung
    if(Job())
        job->~Magic(this);
    //Blut regenerieren (Vamp nicht)
    if (vampire != Role_Vampire && vampire != Role_Lich)
        Blood(1);
    //Hungersteigerung
    if (Hunger() <= 1000)
        Hunger(1 + Who());
    else 
        DoEnergy(-1);
    if (GetAction() == "Fly")
    {
        if (GetContact(this, -1) & CNAT_Left || GetContact(this, -1) & CNAT_Right)
            SetAction("Scale");
        if (GetContact(this, -1) & CNAT_Top)
            SetAction("Hangle");
    }
    return 1;
}

//-------------------------------------Clonk----------------------------------------

public func HumanForm()
{
    //Sichtweiten
    var range = 300;
    if (IsNight())
        range = range / 2;
    if (!GBackSky())
        range = range / 2;
    //Verlangsamung (100 = normal)
    var slowdown = 100;
    //Im Schlaf:
    if (GetAction() == "Dead")
    {
        SetPlrViewRange(0);
        //Müdigkeit regenerieren
        RestoreSleep(3);
        //Keine Schmerzen -> Leben
        if (RestorePain() < MaxPain() / 3)
            DoEnergy(1);
        //Kräfte aus.
        if (power)
            power = 0;
        //Häh wieso ist der Bildschirm schwarz? (Zitat ende)
        if (sleep | pain)
            range = 0;
    }
    else 
        sleep = sleep + 1 * IsNight() * EffectSleepMod(GetAction() == "Dead");
    //Schmerz klingt ab
    if(CanRestorePain())
        RestorePain(1 + Random(2));
    //Wille unnötig? ausschalten...
    if (power == M_WL && sleep < MaxSleepnes() && pain < MaxPain() && blood > 20)
        power = 0;
    //Die Folgen von Schlafmangel, Schmerz, Hunger und Blutverlust
    var div = 1;
    if (pain | sleep | blood < 100 | hunger > 500)
    {
        //Blutleer -> Tot und 1000 hunger -> auch tot 
        if (blood <= 0)
            Death();
        if (hunger >= 999)
            Death();
        //Warnungen
        if (pain > 10)
            var painmes = Format("$VHPain$", pain, MaxPain());
        else 
            var painmes = " ";
        if (sleep > 50)
            var sleepmes = Format("$VHTiredness$", sleep, MaxSleepnes());
        else 
            var sleepmes = " ";
        if (hunger > 500)
            var hungermes = Format("$VHHunger$", hunger);
        else 
            var hungermes = " ";
        if (blood < 100)
            var bloodmes = Format("$VHLsotBlood$", 100 - blood);
        else 
            var bloodmes = " ";
        if (!prayer)
            PlayerMessage(GetOwner(), "%s%s%s%s", this, sleepmes, painmes, hungermes, bloodmes);
        //Ohnmacht
        if ((sleep > MaxSleepnes() | pain > MaxPain() | blood < 20) && GetAction() != "Dead" && power != M_WL && !GetEffect("Dash",this))
        {
            CntextSleep();
        }
        //Schwäche
        if (sleep > MaxSleepnes() * 2 / 3 | hunger > 870 | pain > (MaxPain() / 3) * 2 | blood < 60)
        {
            slowdown = slowdown / 2;
            range = range / 2;
        }
    }
    
        //Besondere Masßnahmen für die KLassen
    //Wächter
    if(Job() == J_GD)
    {
        //Für das Wachen
            if(GetEffect("J_GD_Guard", this))
            {
                range = range*2;
                if(GBackSky() && !IsNight())
                    range = range*2/3;
                slowdown = slowdown*4/6;
            }
    }
    
    //Endgültig setzen
    SetPlrViewRange(range);
    //Mit Wille Schmerz und Schlaf ignorieren
    if (power == M_WL)
    {
        if (GetMagicEnergy())
        {
            DoMagicEnergy(-1);
            slowdown = 120;
        }
        else 
            power = 0;
    }
    
    
    //Erscheinung und physicals
    SetClrModulation(); //Normaloclonk
    SetCon(100);
    //Physicals
    //bei bestimmten Effekten ignorieren, damit das sich nicht beißt
    if(!GetEffect("Run",this) && !GetEffect("Dash",this))
        SetPhysical("Walk", 70000 * slowdown / 100, 2);
    SetPhysical("Energy", MaxHealth() * 1000, 2);
    SetPhysical("Magic", MaxMana() * 1000, 2);
    SetPhysical("Breath", 100000, 2);
    SetPhysical("Swim", 50000 * slowdown / 100, 2);
    SetPhysical("Jump", 40000, 2);
    SetPhysical("Scale", 30000 * slowdown / 100, 2);
    SetPhysical("Hangle", 30000 * slowdown / 100, 2);
    SetPhysical("Fight", 50000 * slowdown / 100, 2);
}

private func CanRestorePain()
{
    if(GetEffect("Potion",this,0,5))
        if(GetEffect("Potion",this,0,5)->~IsUrticaOil())
            return false;
    if(GetEffect("AteNettle",this))
        return false;
    return true;
}

private func EffectSleepMod(bool fIsSleeping)
{
    if(GetEffect("Potion",this,0,5))
    {
        if(GetEffect("Potion",this,0,5)->~IsUrticaOil())
            if(fIsSleeping)
                return Max(1,Random(5)-2);
            else
                return 0;
    }
    return 1;
}

//-------------------------------------Ghoul----------------------------------------

public func GhoulForm()
{
    //Sichtweiten
    var range = 200;
    //Verlangsamung (120 = normal)
    var slowdown = 120;
    //Im Schlaf:
    if (GetAction() == "Dead")
    {
        SetPlrViewRange(0);
        //Müdigkeit regenerieren
        RestoreSleep(5);
        //Keine Schmerzen -> Leben
        if (RestorePain() < MaxPain() / 3)
            DoEnergy(1);
        //Kräfte aus.
        if (power)
            power = 0;
        //Häh wieso ist der Bildschirm schwarz? (Zitat ende)
        if (sleep | pain)
            range = 0;
    }
    //Die Folgen von Schlafmangel, Schmerz, Hunger und Blutverlust
    var div = 1;
    if (pain | sleep | blood < 100 | hunger > 500)
    {
        //Blutleer -> Tot 1000 hunger -> auch tot
        if (blood <= 0)
            Death();
        if (hunger >= 999)
            Death();
        //Warnungen
        if (pain > 10)
            var painmes = Format("$VHPain$", pain, MaxPain());
        else 
            var painmes = " ";
        if (sleep > 50)
            var sleepmes = Format("$VHTiredness$", sleep, MaxSleepnes());
        else 
            var sleepmes = " ";
        if (hunger > 500)
            var hungermes = Format("$VHHunger$", hunger);
        else 
            var hungermes = " ";
        if (blood < 100)
            var bloodmes = Format("$VHLsotBlood$", 100 - blood);
        else 
            var bloodmes = " ";
        if (!prayer)
            PlayerMessage(GetOwner(), "%s%s%s%s", this, sleepmes, painmes, hungermes, bloodmes);
        //Ohnmacht
        if ((sleep > MaxSleepnes() | pain > MaxPain() | blood < 20) && GetAction() != "Dead" && power != M_WL)
        {
            CntextSleep();
            range = 0;
        }
        //Schwäche
        if (sleep > MaxSleepnes() * 2 / 3 | hunger > 870 | pain > (MaxPain() / 3) * 2 | blood < 60)
        {
            slowdown = slowdown / 2;
            range = range / 2;
        }
    }
    //Nachts Vorteile
    if (IsNight())
    {
        //Blitzregeneration
        if (blood < 100)
            Blood(1);
        if (pain < 20)
            DoEnergy(3);
        //Schmerz und Schlaf regenerieren schneller
        RestorePain(2);
        RestoreSleep(3);
        //Noch schneller
        slowdown = slowdown + 30;
    }
    //Endgültig setzen
    SetPlrViewRange(range);
    //Aussehen
    SetClrModulation(RGBa(127, 170, 127, 0)); //Grünlich und groß
    SetCon(110);
    //Physicals
    SetPhysical("Walk", 70000 * slowdown / 100, 2);
    SetPhysical("Energy", MaxHealth() * 1000, 2);
    SetPhysical("Magic", MaxMana() * 1000, 2);
    SetPhysical("Breath", 100000, 2);
    SetPhysical("Swim", 50000 * slowdown / 100, 2);
    SetPhysical("Jump", 50000, 2);
    SetPhysical("Scale", 30000 * slowdown / 100, 2);
    SetPhysical("Hangle", 30000 * slowdown / 100, 2);
    SetPhysical("Fight", 50000 * slowdown / 100, 2);
}
//------------------------------------Lich--------------------------------------

protected func LichForm()
{
    //Sichtweiten
    var range = 300;
    //Verlangsamung (100 = normal)
    var slowdown = 100;
    //Die Folgen von Schlafmangel, Schmerz und Hunger
    var div = 1;
    if (pain | sleep | hunger > 500)
    {
        //Warnungen
        if (pain > 10)
            var painmes = Format("$VHPain$", pain, MaxPain());
        else 
            var painmes = " ";
        if (sleep > 50)
            var sleepmes = Format("$VHTiredness$", sleep, MaxSleepnes());
        else 
            var sleepmes = " ";
        if (hunger > 500)
            var hungermes = Format("$VHHunger$", hunger);
        else 
            var hungermes = " ";
        if (!prayer)
            PlayerMessage(GetOwner(), "%s%s%s", this, sleepmes, painmes, hungermes);
        //Ohnmacht
        if ((sleep > MaxSleepnes() | pain > MaxPain()) && GetAction() != "Dead" && power != M_WL)
        {
            CntextSleep();
            range = 0;
        }
        //Schwäche
        if (sleep > MaxSleepnes() * 2 / 3 | pain > (MaxPain() / 3) * 2)
        {
            slowdown = slowdown / 2;
            range = range / 2;
        }
    }
    if (Contained() == House())
    {
        if (GetEnergy() < MaxHealth())
        {
            range = 0;
            DoEnergy(3);
        }
        if (sleep | pain)
        {
            range = 0;
            RestoreSleep(3);
            RestorePain(2);
        }
    }
    //Fähigkeit Regeneration
    if (power == M_RE)
    {
        if (GetEnergy() >= MaxHealth())
            power = 0;
        if (!Blood())
            power = 0;
        var slowdown = slowdown / 3;
        Blood(-2);
        DoEnergy(5);
    }
    //Im Dunklen Vorteile, im Licht Nachteile
    if (IsNight() | !GBackSky())
    {
        //Schmerz regenerieren
        RestorePain(1+Random(2)+!Random(5));
        //Noch schneller
        slowdown = slowdown + 10;
    }
    else if (!Contained())
    {
        DoEnergy(-12000, this, true, FX_Call_EngSun, NO_OWNER);
        Smoke(0, 0, 5);
        Smoke(0, 0, 5);
        Smoke(0, 0, 5);
        range = range / 6;
    }
    // Vom Blut nähren
    if (Hunger() > 50)
    {
        Hunger(-50);
    }
    //Kein Atem
    if (GBackLiquid())
        SetPhysical("BreatheWater", 1, 2);
    else 
        SetPhysical("BreatheWater", 0, 2);
    SetPlrViewRange(range);
    //Physicals
    SetPhysical("Walk", 70000 * slowdown / 100, 2);
    SetPhysical("Energy", MaxHealth() * 1000, 2);
    SetPhysical("Magic", MaxMana() * 1000, 2);
    SetPhysical("Swim", 50000 * slowdown / 100, 2);
    SetPhysical("Jump", 45000, 2);
    SetPhysical("Scale", 30000 * slowdown / 100, 2);
    SetPhysical("Hangle", 30000 * slowdown / 100, 2);
    SetPhysical("Fight", 50000 * slowdown / 100, 2);
}
//-------------------------------------Vampir-----------------------------------

public func VampireForm() //Vampir sein
{
    //Sichtweiten
    var range = 300;
    //Verlangsamung (100 = normal)
    var slowdown = 100;
    var slowdown2 = 5;
    if (FindDarkAura())
    {
        slowdown2 = 7;
        slowdown = 120;
    }
    //Die Folgen von Schlafmangel, Schmerz und Hunger
    var div = 1;
    if (pain | sleep | hunger > 500)
    {
        //Warnungen
        if (pain > 10)
            var painmes = Format("$VHPain$", pain, MaxPain());
        else 
            var painmes = " ";
        if (sleep > 50)
            var sleepmes = Format("$VHTiredness$", sleep, MaxSleepnes());
        else 
            var sleepmes = " ";
        if (hunger > 500)
            var hungermes = Format("$VHHunger$", hunger);
        else 
            var hungermes = " ";
        if (!prayer)
            PlayerMessage(GetOwner(), "%s%s%s", this, sleepmes, painmes, hungermes);
        //Ohnmacht
        if ((sleep > MaxSleepnes() | pain > MaxPain()) && GetAction() != "Dead" && power != M_WL)
        {
            CntextSleep();
            range = 0;
        }
        //Schwäche
        if (sleep > MaxSleepnes() * 2 / 3 | pain > (MaxPain() / 3) * 2)
        {
            slowdown = slowdown / 2;
            range = range / 2;
        }
    }
    //Fliegen
    if (GetAction() == "Fly")
    {
        if (Blood() > 1)
            Blood(-2);
        else 
            SetAction("Jump");
    }
    //Im Sarg regenerieren
    if (Contained() == House())
    {
        if (GetEnergy() < MaxHealth())
        {
            range = 0;
            DoEnergy(2);
        }
        if (sleep | pain)
        {
            range = 0;
            RestoreSleep(3);
            RestorePain(3);
        }
    }
    //Fähigkeit Regeneration
    if (power == M_RE)
    {
        if (GetEnergy() >= MaxHealth())
            power = 0;
        if (!Blood())
            power = 0;
        var slowdown = slowdown / 3;
        Blood(-1);
        DoEnergy(1);
    }
    //Schmerz regenerieren 1.875
    RestorePain(2-(Random(8)+1)/8);
    //Im Dunklen Vorteile, im Licht Nachteile
    if (IsNight() | !GBackSky())
    {
        //Noch schneller
        slowdown = slowdown + 20;
    }
    else if (!Contained())
    {
        DoEnergy(-5000, this, true, FX_Call_EngSun, NO_OWNER);
        Smoke(0, 0, 5);
        range = range / 6;
    }
    // Vom Blut nähren
    if (Hunger() > 50)
    {
        Blood(-1);
        Hunger(-50);
    }
    //Kein Atem
    if (GBackLiquid())
        SetPhysical("BreatheWater", 1, 2);
    else 
        SetPhysical("BreatheWater", 0, 2);
    //Endgültig setzen
    SetPlrViewRange(range);
    //Aussehen
    if (power == M_IV)
    {
        var vis = 200;
        if (!Blood())
            power = 0;
        else 
            Blood(-1);
    }
    SetClrModulation(RGBa(255, 255, 255, vis)); //Leichenfarbe (Alphawert falls unsichtbar)
    SetCon(100);
    //Physicals
    SetPhysical("Walk", 70000 * slowdown / 100, 2);
    SetPhysical("Energy", MaxHealth() * 1000, 2);
    SetPhysical("Magic", MaxMana() * 1000, 2);
    SetPhysical("Breath", 100000, 2);
    SetPhysical("Swim", 50000 * slowdown / 100, 2);
    SetPhysical("Jump", 40000, 2);
    SetPhysical("Scale", 30000 * slowdown / 100, 2);
    SetPhysical("Hangle", 30000 * slowdown / 100, 2);
    SetPhysical("Fight", 50000 * slowdown * slowdown2 / 500, 2);
}

// -------------------- Fliegen (na endlich) ---------------------------------

protected func ControlUpDouble() //Anfangen
{
    if (Who() == Role_Vampire && Blood() && GetAction() == "Jump")
        SetAction("Fly");
    return _inherited();
}

protected func ControlDownDouble() //Aufhören
{
    if (Who() == Role_Vampire && Blood() && GetAction() == "Fly")
        SetAction("Jump");
    return _inherited();
}

protected func ControlLeft()
{
    if (GetAction() == "Fly")
    {
        SetDir(DIR_Left);
        SetComDir(COMD_Left);
    }
    return _inherited();
}

protected func ControlRight()
{
    if (GetAction() == "Fly")
    {
        SetDir(DIR_Right);
        SetComDir(COMD_Right);
    }
    return _inherited();
}

protected func ControlDown()
{
    if (GetAction() == "Fly")
        SetComDir(COMD_Down);
    return _inherited();
}

protected func ControlUp()
{
    if (GetAction() == "Fly")
        SetComDir(COMD_Up);
    return _inherited();
}


// ------------------------------------------------------------------------------------
// ---------------------------Kontext: Spezielle Aktionen------------------------------
// ------------------------------------------------------------------------------------

//------------------------ Priester: Bajonette-------------------------

//Menü zum Nutzen vom ganzen Spezialzeugs, oder so

protected func ContextAbility()
{
    [$VHAbility$|Image=MEDC|Condition=CanUseAbi]
    CreateSpellMenu(this,this,true);
}

protected func CanUseAbi()
{
    if(CBMU->GetAbilitys(this)[0] != 0)
        return true;
    return false;
}

// --------------------------- Alle: Waffen imprägnieren ---------------------------

public func Poision(ID, carrier)
{
    carrier->~Impregnate(ID);
    RemoveObject(FindContents(ID));
    PlayerMessage(GetOwner(), "$VHDoped$", this, GetName(0, ID), GetName(0, GetID(carrier)));
}

// ------------  Alle: Alchemie ------------------

public func MakePotion(id IDdef)
{
    var IDs = IDdef->Basic();
    //convert to array
    if(GetType(IDs) != C4V_Array)
        IDs = [IDs];
    var Numers = IDdef->Number();
    //convert to array
    if(GetType(Numers) != C4V_Array)
        Numers = [Numers];
    //go through the arrays and check if you can make this poision
    for(var i = 0; i < GetLength(IDs); i++)
    {
        if (ContentsCount(IDs[i]) >= Numers[i])
        {
            continue;
        }
        PlayerMessage(GetOwner(), "$VHNotEnough$", this, GetName(0,IDs[i]));
        return 1;
    }
    //remove what you need
    for(var i = 0; i < GetLength(IDs); i++)
    {
        Sound("Alchem*");
        var v = Numers[i];
        while (v--)
            RemoveObject(FindContents(IDs[i]));         
    }
    //make it
    CreateContents(IDdef);
}




// -----------------------------Vampir/Diener: Diener erschaffen--------------------------------

protected func MakeSlave(int iType) //Versklavung
{
	   //Nochmal nachüberprüfen falls unser OPfer aufgestanden ist
    if (!CanCreateSlaves())
        return false;
    if (GetPlayerCount() > 10)                   
    {
        if (GetWealth(GetOwner(this)) < 6)
        {
            partner = SleepingClonk();
            blood = blood - 75;
            SetWealth(GetOwner(partner), 1);
            PlayerMessage(GetOwner(this), "$VHMadeSlave$", 0, GetPlayerName(GetOwner(partner)));
            if (iType == 1)
                partner->BecomeGhoul(this);
            else 
                partner->BecomeLich(this);
        }
        else 	
            PlayerMessage(GetOwner(this), "$VHToMuchSlaves1$");
    }

    else if (GetPlayerCount() > 8)
    {
        if (GetWealth(GetOwner(this)) < 5)
        {
            partner = SleepingClonk();
            blood = blood - 75;
            SetWealth(GetOwner(partner), 1);
            PlayerMessage(GetOwner(this), "$VHMadeSlave$", 0, GetPlayerName(GetOwner(partner)));
            if (iType == 1)
                partner->BecomeGhoul(this);
            else 
                partner->BecomeLich(this);
        }
        else 
            PlayerMessage(GetOwner(this), "$VHToMuchSlaves2$");
    }
    else if (GetPlayerCount() > 6)
    {
        if (GetWealth(GetOwner(this)) < 4)
        {
            partner = SleepingClonk();
            blood = blood - 75;
            SetWealth(GetOwner(partner), 1);
            PlayerMessage(GetOwner(this), "$VHMadeSlave$", 0, GetPlayerName(GetOwner(partner)));
            if (iType == 1)
                partner->BecomeGhoul(this);
            else 
                partner->BecomeLich(this);
        }
        else 
            PlayerMessage(GetOwner(this), "$VHToMuchSlaves3$");
    }
    else if (GetPlayerCount() > 4)
    {
        if (GetWealth(GetOwner(this)) < 3)
        {
            partner = SleepingClonk();
            blood = blood - 75;
            SetWealth(GetOwner(partner), 1);
            PlayerMessage(GetOwner(this), "$VHMadeSlave$", 0, GetPlayerName(GetOwner(partner)));
            if (iType == 1)
                partner->BecomeGhoul(this);
            else 
                partner->BecomeLich(this);
                
        }
        else 
            PlayerMessage(GetOwner(this), "$VHToMuchSlaves4$");
    }
    else 
        PlayerMessage(GetOwner(this), "$VHNoSlaves$");
} 
 

protected func CanCreateSlaves() // Diener erschaffen
{
 if(vampire ==  Role_Vampire && blood > 75 && SleepingClonk())  
	 return 1;
}


public func BecomeGhoul(master) //Ghul werden
{
    vampire = 2; //Ghulstatus
    partner = master; //Meister merken
    //FormWandlung
    LichForm();
    //Das Blut wirkt
    DoEnergy(75);
    pain = 0;
    sleep = 0;
    CntextAwake();
    DoWealth(GetOwner(partner), 1);
    Sound("meiter");
    //Message
    PlayerMessage(GetOwner(this), "$VHNewLife$", 0, GetPlayerName(GetOwner(partner)));
}

//Vampirwerdung
public func BecomeVampire()
{
    vampire = 1;
    VampireForm();
}
/*Lichwerdung*/
protected func BecomeLich(object pMaster) //Lich werden
{
    vampire = 3; //Lichstatus
    partner = pMaster; //Meister merken
    house = partner->House();
    SetGraphics(0, this, LICH);
    Job(J_LI);
    DoMagicEnergy(-100, this, true);
    LichForm();
    //Das Blut wirkt
    DoEnergy(75);
    pain = 0;
    sleep = 0;
    CntextAwake();
    DoWealth(GetOwner(partner), 1);
    Sound("BecomeLich");
    //Message
    PlayerMessage(GetOwner(this), "$VHNewLife$", 0, GetPlayerName(GetOwner(partner)));
}



// -----------------------------Alle: Beten--------------------------------

public func Pray(ID)
{
    prayer = CreateContents(ID);
    prayer->~Apply(this);
    PrayCooldown(ID);
        DoExperience();
        DoExperience();
}

public func PrayCooldown(id idPrayer)
{
    if(!idPrayer->~GetPrayCooldown())
        SetCoolDown(45);
    else
        SetCoolDown(idPrayer->~GetPrayCooldown());
    return true;
}

//-------------------------------Lich: Fähigkeiten------------------------------

//Funktion ZUmmFinden der Schatten aura
protected func FindDarkAura()
{
    var aClonks;
    aClonks = FindObjects(Find_ID(MEDC), Find_Distance(150, 0, 0));
    for (var pClnk in aClonks) 
    {
        if (GetEffect("DarkAura", pClnk))
            return true;
    }
}
//----------------------------Aufstehen und Hinlegen----------------------------

public func CntextSleep()
{
    M_SL->Activate(this);
} 

public func CntextAwake() //Aufstehen
{
    M_SL->Activate(this);
} 

//---------------------------------Suchläufe--------------------------

public func SleepingClonk() // schlafende Clonks
{
    var clonk = FindObject(MEDC, -7, -9, 14, 18, 0, "Dead");
    if (clonk == this)
        return false;
    if (GetAction() == "Walk" || GetAction() == "Swim")
        return clonk;
}

public func Weapon() // Mordwaffen
{
    if (vampire == Role_Lich)
        return MEDC;
    if (vampire == Role_Slave && power)
        return MEDC;
    if (vampire == Role_Vampire)
        return M_BT;
    if (ContentsCount(STAK))
        return STAK;
    if (ContentsCount(AXE1))
        return AXE1;
    if (ContentsCount(SWOR))
        return SWOR;
}

//------------------------------------------------------------------------------------
//--------------------------------Schaden und Tod-------------------------------------
//------------------------------------------------------------------------------------

protected func Death(ID) //Tod Durch...
{
    // den Beutel fallenlassen
    if (GetAlchemBag())
        GetAlchemBag()->~Loose();
    //Haus verwaist
    if (house)
        house->SetOwner(-1);
    //Vampir tot -> Der Ghoul stirbt auch.
    if (vampire == Role_Vampire && partner)
        partner->Death(ID);
    Sound("Die");
    DeathAnnounce();
    var h;
    if (GetAction() != "Death")
        h = 1;
    var corpse = CreateObject(CP_2, 0, 1 + 7 * h, -1);
    LocalN("name", corpse) = GetName();
    if (ID == AXE1 | ID == MEDC)
        corpse->Head();
    // Letztes Mannschaftsmitglied tot: Script benachrichtigen
    GameCall("RelaunchPlayer", GetOwner());
    RemoveObject();
    //if (!GetCrew(GetOwner()))
}

protected func Death2(ID) //Tod Durch...
{
    // den Beutel fallenlassen
    if (GetAlchemBag())
        GetAlchemBag()->~Loose();
    //Haus verwaist
    if (house)
        house->SetOwner(-1);
    //Vampir tot -> Der Ghoul stirbt auch.
    if (vampire == Role_Vampire && partner)
        partner->Death(ID);
    Sound("Die");
    DeathAnnounce();
    var h;
    if (GetAction() != "Death")
        h = 1;
    var corpse = CreateObject(CP_1, 0, 1 + 7 * h, -1);
    LocalN("name", corpse) = GetName();
    if (ID == AXE1 | ID == MEDC)
        corpse->Head();
    // Letztes Mannschaftsmitglied tot: Script benachrichtigen
    GameCall("RelaunchPlayer", GetOwner());
    RemoveObject();
}

//-------------------------------Neues Treffersystem-----------------------------

protected func QueryCatchBlow(pObj)
{
    if (GetEffect("Dummy", pObj))
        return 0;
    AddEffect("Dummy",pObj,1,20,pObj);
    //bei Pfeilen wird "HitEffect" über HitTarget aufgerufen, ein "normaler" Hit
    //darf keinen Schaden verursachen, ansonsten imba Pfeile!
    if(!pObj->~IsArrow())
        pObj->~HitEffect(this); //Wurfwaffen und Pfeile haben diese Funktion
    if (!pObj || Contained(pObj))
        return 1;
    if (pObj->~BounceOff())
        return 1;
    //Macht aua aber sonst nix...
    Sound("ClonkHurt*");
    SetAction("Tumble");
    return 0;
} 

/* Während des Kämpfens */ 
private func Fighting(pEnemy)
{
        if(!pEnemy)
            pEnemy = GetActionTarget(0,this);
    // Lanze verlieren 
    var pLance;
    if (pLance = HasLance())
        pLance->LNCA::Unbuckle();
    // Kampf mit Handwaffe 
    var pWeapon;
    if (pWeapon = GetFightWeapon())
    {
        // Erstmal Kampfaktion setzen 
        if (GetAction() != pWeapon->~GetFightAction())
            return SetAction(pWeapon->~GetFightAction(), pEnemy);
        // Je nach Waffengeschwindigkeit Schlag durchführen 
        if (!Random(pWeapon->~GetStrikeTime()))
        {
            if((!Who() && pEnemy->~Who()) || (Who() && !pEnemy->~Who()) )
            {
            DoExperience();
            DoExperience();
          }
            return SetAction(pWeapon->~GetStrikeAction());
        }
        // Weiterkämpfen 
        return 1;
    }
    // Keine Waffe: Gegner schlagen 
    if (!Random(4))
    {
      if((!Who() && pEnemy->~Who()) || (Who() && !pEnemy->~Who()) )
        DoExperience();
      SetAction("Punch");
    }
    // Weiterkämpfen 
    return 1;
} 

/* MedievelClonk:Schlag mit Handwaffe */ 
private func Striking()
{
    // Waffe bestimmen 
    var pWeapon;
    if (!(pWeapon = GetFightWeapon()))
        return 0;
    // Sound 
    pWeapon->~StrikeSound();
    // Schlagen!
    var pEnemy = GetActionTarget();
    pWeapon->~MeleeEffect(pEnemy);
    return 1;
}
 
//-------------------------Wertveränderung von außen und Wertfuncs ------------------------

public func Feed(int amount, isblood) // Nahrungsaufnahme
{
    if ((vampire == Role_Vampire && !isblood) || (vampire == Role_Lich && !isblood))
    {
        Message("$VHHumanFood$", this);
        return 0;
    }
    if ((isblood && vampire == Role_Vampire) || (isblood && vampire == Role_Lich))
    {
        Blood(amount);
        Sound("BloodSuck*");
        return 1;
    }
    else 
    {
        DoEnergy(amount / 4);
        Hunger(-amount * 10);
        Sound("KnightMunch");
        return 1;
    }
}

public func Pain(int amount, superior) //Schmerz erleiden
{
    //Aufschrei
    if (amount > 0)
        Sound("Hurt*");
    //Ohne Wert als Abfrage werten
    if (!amount)
        return RestorePain();
    amount = amount * DamageMod()/100;
    //Je nach Wesen umrechnen
    if (!vampire)
        return RestorePain(-amount);
    if(vampire == Role_Vampire)
    {
        return RestorePain(-amount*CalcPlayerValue(amount)/100);
    }
    if (vampire == Role_Vampire && superior)
        return RestorePain(-amount);
    if (vampire == Role_Slave)
        return RestorePain(-amount * 75 / 100);
    if (vampire == Role_Lich)
    {
            return RestorePain(-amount * 75 / 100);
    }
}

//Blutverlieren mit Optischen effektt
public func LoseBlood(int iAmount, int iY, bool fBack)
{
    var dir = -1 + 2 * GetDir();
    if (fBack)
        dir *= -1;
    var i = 0;
    Blood(-iAmount);
    if (Contained())
        return true;
    while (i < iAmount)
    {
        i++;
        CreateParticle("Blood", 3 * dir, RandomX(-2, 2) + iY, RandomX(6, 12) * dir, RandomX(-3, 1), 10, RGB(230, 10, 10));
        CreateParticle("Blood", 3 * dir, RandomX(-2, 2) + iY, RandomX(6, 12) * dir, RandomX(-3, 1), 10, RGB(230, 10, 10));
    }
    return true;
}

//-------------------------- Abfragen ------------------------------------


public func RestoreSleep(int amount)  //Wertveränderung/abfrage: Schlaf
{
    if (!amount)
        return sleep;
    sleep = sleep - amount;
    if (sleep < 0)
        sleep = 0;
}

public func RestorePain(int amount) //Wertveränderung/abfrage: Schmerz
{
    if (!amount)
        return pain;
    pain = pain - amount;
    if (pain < 0)
        pain = 0;
}

public func Blood(int amount) //Wertveränderung/abfrage: Blut
{
    if (!amount)
        return blood;
    blood = blood + amount;
    if (blood < 0)
        blood = 0;
    if (vampire == Role_Vampire || vampire == Role_Lich)
        if (blood > MaxMana())
            blood = MaxMana();
    if (vampire != Role_Vampire && vampire != Role_Lich)
        if (blood > 100)
            blood = 100;
}

protected func MaxBlood()
{
    if (vampire == Role_Vampire || vampire == Role_Lich)
        return MaxMana();
    if(vampire != Role_Vampire && vampire != Role_Lich)
        return 100;
}

public func Hunger(int amount) //Wertveränderung/abfrage: Hunger
{
    if (!amount)
        return hunger;
    hunger = hunger + amount;
    if (hunger < 0)
        hunger = 0;
    if (hunger > 1000)
        hunger = 1000;
}

public func Who(set) //Wesen : 0=Mensch , 1=Vampir, 2=Ghoul, 3=Lich
{
    if (set)
        vampire = set;
    return vampire;
}

public func Job(set) //ID des Jobs
{
    if (set)
        job = set;
    if (job)
        return job;
}

public func House(set) //Objekt: Behausung
{
    if (house)
        return house;
    if (set)
    {
        house = set;
        SetOwner(GetOwner(), house);
    }
}

public func MaxHealth()
{
        if(!job)
            return 90;
    var aValues = job->~Health();
    if(!aValues)
        return 90;
    var value = aValues[iLevel-1];
    if (!value)
        value = 90;
    return value;
}

public func MaxPain() //Fester Wert: Schmerzgrenze
{
        if(!job)
            return 100;
    var aValues = job->~Pain();
    if(!aValues)
        return 100;
    var value = aValues[iLevel-1];
    if (!value)
        value = 100;
    if (vampire == Role_Slave)
        value = value + 20 * form;
    return value;
}

public func MaxSleepnes() //Fester Wert: Müdigkeit
{
        if(!job)
            return 150;
    var aValues = job->~MaxSleepnes();
    if(!aValues)
        return 150;
    var value = aValues[iLevel-1];
    if (!value)
        value = 150;
    return value;
}

public func MaxMana() //Fester Wert:Maximale Energie
{
        if(!job)
            return 0;
    var aValues = job->~MaxMana();
    if(!aValues)
        return 0;
    var value = aValues[iLevel-1];
    return value;
}

//------Cooldown:Abklingzeit------

public func GetCoolDown()               { return Cooldown;  }
public func GetCoolDownTimer()  { return Cooltimer; }
public func IsDownCooled()          {   return Cooltimer <= 0;}

public func CoolDown()
{
    if(Cooltimer > 0)
        Cooltimer--;
    else
        return false;
    return Cooldown;
}

public func SetCoolDown(int cool)
{
    Cooldown = cool;
    Cooltimer = cool;
    return true;
}

public func ResetCoolDown()
{
    Cooldown = 0;
    Cooltimer = 0;
    return true;
}

protected func FxIntCoolDownTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
    if(!pTarget->~CoolDown() || pTarget->~CoolDown() <= 0)
        pTarget->~ResetCoolDown();
    return true;
}

//----Erfahrungssystem------

protected func DoExperience()
{
    iExperience++;
    if (iExperience >= ExperienceToLevel(iLevel + 1) && iLevel < MaxLevel())
    {
        iLevel++;
        job->~OnLevelUp(iLevel);
    }
}
//maximallevel
protected func MaxLevel()
{
    //Die KLasse frageb welches MaxLevel sie hat wenn keins dann Standart zurückgeben
    if(!job)
        return 1;
    if (job->~MaxLevel())
        return job->~MaxLevel();
    return 2;
}

protected func ExperienceToLevel(int iLvl)
{
    //Falls die Klasse was anderes sagt dieses zurückgeben
    if (job->~ExperienceToLevel(iLvl))
        return job->~ExperienceToLevel(iLvl);
    //Lvl2
    if (iLvl == 2)
        return 123;
    //Lvl3
    if (iLvl == 3)
        return 267;
    //Lvl 4
    if (iLvl == 4)
        return 435;
}

//Levelabfrage fü andere Objecte
protected func GetLevel()       {   return iLevel;                              }

//Funktionen für den StatusHud-Expbalken
protected func IsMaxLevel() {   return MaxLevel() <= iLevel;    }

//Die Exp bis zum Nächsten Level in Tausentstel
protected func LevelExpPer2K()
{
    var next = ExperienceToLevel(iLevel+1);
    var exp = iExperience;
    for(var i = iLevel; i > 1; i--)
    {
        next-=ExperienceToLevel(i);
        exp-=ExperienceToLevel(i);
    }
    var j = 2000*exp/next;
    return j;
}

/*----Weitere Erfahrungsverteilung----*/

protected func OnSmithSuccess()
{
    var i = Job()->~SmithExp();
    while(i--)
        DoExperience();
    return true;
}

//############################# Armbrust - übernommen aus M&M Assa.

private func FireCrossbowAt(int iX, int iY, bool fForce) 
{ 
    // Zielt gar nicht: erst mal laden 
    if (!IsAiming())
    {
        LoadCrossbow(FindContents(CRBW), 1);  
        return 0;  
    }
    FireBowAt(iX,iY,fForce);
} 

/* Beginnt den Zielvorgang */  
public func AimCrossbow() 
{ 
    return SetAction("AimCrossbow");
}  
  
/* Beginnt den Ladevorgang */  
public func LoadCrossbow(pBow, fFirstTime) 
  { 
  // Nur mit Pfeil! 
  if (!GetBoltCount() && !FindObject(HOOK)) 
    { 
    // Kein Pfeil fürs erste Zielen: Meldung ausgeben 
    if (fFirstTime) 
      { 
      Sound("KnightFailure"); 
      PlayerMessage(GetController(), "$MsgNoArrow$", this); // Kein Pfeil 
      } 
    return 1; 
    } 
  // Bogen lieber wieder auswählen (GetArrow wählt gerne ab...) 
  ShiftContents(0,0,CRBW); 
  // Schon beim Nachladen? 
  if (IsLoading()) return 1;
  // War gerade beim Zielen: letzten Winkel sichern 
  if (IsAiming())  
    iLastAimAngle = GetPhase();  
  else  
    iLastAimAngle = 4;
  if(Contents(0,this)->IsRdy())
  {
    return SetAction("AimCrossbow");
  }
  Sound("CrossbowReload*"); 
  // Muss den Rückgabewert von SetAction zurückgeben, da das Setzen durch Überladungen fehlgeschlagen sein kann 
    return SetAction("LoadCrossbow");
  }
  
protected func LoadingCrossbow()
{
  // Mittleren Teil der Animation mehrfach abspielen
  if(GetPhase()==7)
  {
    if(Local()++<2)
      SetPhase(2);
    else Local() = 0;
  }
}
 
protected func BeginAiming()
{ 
    Contents()->~ActualizePhase(this);
    Contents()->~HasLoaded(true); 
    return SetPhase(iLastAimAngle); 
} 
  
//! Zielt der Clonk gerade mit dem Bogen? 
private func IsAiming() 
{ 
  return (GetAction() == "AimBow" || GetAction() == "AimCrossbow"); 
} 
 
//! Lädt der Clonk gerade den Bogen nach? 
private func IsLoading() 
{ 
  return (GetAction() == "LoadBow" || GetAction() == "LoadCrossbow"); 
}

/* Bolzen aus dem Inventar nehmen */ 
public func GetBolt(idID)
  { 
  // Einzelne Bolzen suchen 
  var pObj, pBolt; 
  for(var i = 0; pObj = Contents(i); i++) 
    if(pObj->~IsBolt() || pObj->~IsHook())
      // ID-Vergleich
      if(!idID || GetID(pObj)==idID)
        return pObj;
  // Bei Bolzen Pakete aufteilen 
  for(var i = 0; pObj = Contents(i); i++) 
    if(pObj->~IsBoltPack())
     // ID-Vergleich 
     if(!idID || pObj->~UnpackTo()==idID)
      // Bolzen aus Paket verwenden
      if(pBolt = pObj->~GetItem()) return pBolt;
  // Keine Bolzen gefunden 
  return 0; 
  }

/* Bolzen im Inventar zählen */ 
private func GetBoltCount() 
{
   return GetSpecialCount("IsBolt");
}

/* Überprüft ob der übergebene Typ ein Pfeil ist  */ 
private func CheckBolt(idDef) { return idDef->~IsBolt(); } 
 
/* Überprüft ob der übergebene Typ ein Pfeilpaket ist */ 
private func CheckBoltPack(idDef) { return idDef->~IsBoltPack(); } 

public func BeginLoading()
{
  AddEffect("IntProgress", this, 1, 1, this, 0,200);
}
protected func FxIntProgressStart(pTarget, iNumber, fTmp, iMaxTime)
{
  if(fTmp) return;
  // Aktion speichern
  EffectVar(0, pTarget, iNumber) = GetAction();
  // Zeit speichern
  EffectVar(2, pTarget, iNumber) = iMaxTime;
}
protected func FxIntProgressTimer(pTarget, iNumber, iTime)
{
  if(GetAction() != EffectVar(0, pTarget, iNumber)) return -1;
  if(iTime > EffectVar(2, pTarget, iNumber)) return;
}
protected func FxIntProgressStop(pTarget, iNumber, iReason, fTmp)
{
  if(fTmp) return;
}
