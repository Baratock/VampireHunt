/*-- Mittelalter-Clonk --*/

#strict 2

//Menü zum Nutzen vom ganzen Spezialzeugs, ganz oben damit es auch ganz oben im Kontext ist
protected func ContextAbility()
{
	[$VHAbility$|Image=MEDC|Condition=CanUseAbi]
	CreateSpellMenu(this, this, true);
}

protected func CanUseAbi()
{
	if (CBMU->GetAbilitys(this)[0] != 0)
		return true;
	return false;
}

#include KNIG

//Allerlei Konstanten
static const cHunger = 1;
static const cPain = 2;
static const cSleep = 4;
static const cBlood = 8;
static const cCooldown = 16;
static const cJob = 32;
static const cHealth = 64;
static const cForm = 128;
static const cMana = 256;
static const cSlowDown = 512;
static const cViewRange = 1024;

static const cRoleHuman = 1; 
static const cRoleVampire = J_VA; 
static const cRoleGhul = J_GH; 
static const cRoleLich = J_LI;

static const cMagicWill = 1;
static const cMagicForce = 2;
static const cMagicMana = 4;
static const cMagicBlood = 8;

//Allerlei locals
local iBlood; //Blut
local iSleep; //Müdigkeit in %
local iPain; //Schmerz
local iHunger; //HUNGER
local idRole; //ID der Rolle
local idoldRole; //Alte Rolle
local fGhul; //Bei wahr, ist die Ghulform aktiv
local iCooldown; //Abklingzeit in 12er Frames
local iMaxCooldownTime; //Die gesamte Abklingzeit in 12er Frames
local iPlayers; //Anzahl der Spieler bei Start
local iSlower; //Slowdown
local iPlrViewRange; //Sichtweite

public func Initialize()
{
	SetRole(J_SM);
	SetVal(cBlood, 100);
	SetVal(cCooldown, 0);
	SetVal(cSlowDown, 100);
	SetVal(cViewRange, 400);
	iPlayers = GetPlayerCount();
	CreateObject(SHUD)->SetClonk(this);
	AddEffect("CalcPain", this, 1, 0, this, 0);
	AddEffect("IntCoolDown", this, 3, 12, this, 0);
	return _inherited();
}

protected func RejectCollect(id idObject, object pObject)
{
	if (GetEffect("FadeOut", pObject))
	{
		return true;
	}
	else
	{
		return inherited(idObject, pObject);
	}
}

//Schmerzeffect
static const FX_Call_EngSun = 41;               //Schaden durch Sonnenbrand
static const FX_Call_EngDarkMagic = 42; //Schaden durch Schwarze Magie
static const FX_Call_EngHoly = 43;          //HeiligerSchaden

protected func FxCalcPainDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause)
{
	if (iDmgEngy < 0)
	{
		var iPain = iDmgEngy * 3 / 2;
		iDmgEngy = iDmgEngy / 2;
		//der Vampir darf nicht sterben wenn er zu wenig Leben hat
		if (pTarget->GetVal(cForm) == cRoleVampire && iCause != FX_Call_EngSun && iCause != FX_Call_EngFire)
			if (-iDmgEngy / 1000 >= GetEnergy(pTarget))
				iDmgEngy = 0;
		//Bei Objhits keinen Schaden
		if (iCause == FX_Call_EngObjHit)
		{
			iPain = 0;
			iDmgEngy = 0;
		}
		//Untotes kann man mit Feuer gut bekämpfen
		if (iCause == FX_Call_EngFire && pTarget->GetVal(cForm) != cRoleHuman)
		{
			iPain = iPain * 2 / 5;
			iDmgEngy = iDmgEngy / 3;
		}
		else
			if (iCause == FX_Call_EngFire)
			{
				iDmgEngy = iDmgEngy / 4;
				iPain = iPain / 3;
			}
			
		//Beim Ersticken
		if (iCause == FX_Call_EngAsphyxiation)
		{
			if (pTarget->GetVal(cForm) != cRoleHuman)
			{
				iPain = 0;
				iDmgEngy = 0;
			}
			else
			{
				iDmgEngy = iDmgEngy / 3;
				iPain = 0;
			}
		}
		//Sonnenlicht soll die Untoten nicht direkt ausknocken
		if(iCause == FX_Call_EngSun)
			iPain = iPain * 3 / 4; 
		//Schwarze Magie macht Menschen mehr schaden
		if (iCause == FX_Call_EngDarkMagic)
		{
			if (pTarget->GetVal(cForm) == cRoleHuman)
			{
				iPain = iPain * 5 / 3;
				iDmgEngy = iDmgEngy * 6 / 5;
			}
		}
		DoPain(-iPain / 1000);
	}
	return iDmgEngy;
}

// -------------------------  Helfer für das Schadenssystem ---------------------------

private func DamageMod(iRole)
{
	if (!iRole)
		return 100;
	if (iRole == cRoleHuman)
	{
		if (GetEffect("PoisionFern", this) || GetEffect("PoisionNah", this))
		{
			// Von Effektzahl abwärts zählen, da Effekte entfernt werden
			var iN = GetEffectCount("PoisionNah", this);
			var iF = GetEffectCount("PoisionFern", this);
			return Max(100 + iN * 27 + iF * 11, 160);
		}
		//versucht einen angriff mit dem hirschfänger
		if (GetEffect("Run", this))
			return 140;
		//sprintet
		if (GetEffect("PowerDash", this))
			return 140;
	}
	else
	{
		return DarkSideDamageMod();
	}
	return 100;
} 

//1. Entf. 2. 160% 3. 130% 4. 110% 5. 90% 6. 80% 7. 75% 8. 70% 9. 65% 10. 60% >11. 55%
private func DarkSideDamageMod()
{
	//spielerzahl
	var x = iPlayers;
	if (x == 1)
		x = 100;
	//minimale Anzahl an Dorfler finden
	if (x == 2)
		x = 150;
	else
		if (Inside(x, 3, 5))
			x = 190 - 20 * x;
		else
			if (Inside(x, 6, 11))
				x = 110 - 5 * x;
			else
				if (x > 11)
					x = 55;
	//nun zurückgeben
	return x;
}

//------------------------------   Überladungen und was sonst noch anfällt ------------------

// Rüstungsstatus setzen
public func SetArmored(fnArmored, idnArmorType)
{
	// Rüstung angelegt 
	if (fArmored = fnArmored)
	{
		// Kein Rüstungstyp angegeben! 
		if (!idnArmorType)
		{
			fArmored = 0;
			return 0;
		}
		// Rüstungstyp speichern 
		idArmorType = idnArmorType;
		// Grafik setzen 
		var strArmorType = idArmorType->~GetGraphics();
		SetGraphics(strArmorType);
		// Beschränkte Physicals
		DoVal(cSlowDown, -30);
		DoVal(cViewRange, -100);
		// Ruestungsportrait 
		var iPortraitCount = 3;
		if (strArmorType == "BlackKnight")
			iPortraitCount = 1; // Ein GetPortraitCount("Name") wäre nett...     
		SetPortrait(Format("%s%d", strArmorType, 1 + Random(iPortraitCount)), this, GetID(), 0, 0);
	}
	else
	{
		// Rüstungstyp löschen 
		idArmorType = 0;
		// Normale Grafik 
		SetGraphics("");
		// Normale Physicals 
		DoVal(cSlowDown, 30);
		DoVal(cViewRange, 100);
		// Normales Portrait 
		SetPortrait(GetPortrait(this, 0, 1), this, GetPortrait(this, 1, 1), 0, 0);
	}
	// Ok 
	return 1;
}  

// Allgemeine Waffen
public func CanUse(id idDef)
{
	//Alles was sich als Nahkampfwaffe ausweist.
	if (idDef->~IsFightWeapon())
		return true;
	//Alles was sich als Speer ausweist.
	if (idDef->~IsSpear())
		return true;
	//Schild geht immer, genauso ein Bogen
	if (idDef == BOW1)
		return true;
	//Klassenspezielle Gegenstände
	if (GetRole()->~CanUse(idDef))
		return true;
} 

//Mehr Inventarplätze
private func MaxContentsCount()
{
	return 5 - 1 * HasArmor();
}

private func IsBlocked()
{
	return false;
} 

//Altes Kontextmenü teilweise entfernen
public func ContextUnbuckle()
{
	[$CtxUnbuckleDesc$|Image=KMSU|Condition=IsBuckled]
	ShowUnbuckleMenu();
	return 1;
}
public func ContextAttack()
{
	[$CtxAttackDesc$|Image=MI_A|Condition=IsBlocked]
} 
public func ContextDefend()
{
	[$CtxDefendDesc$|Image=MI_D|Condition=IsBlocked]
} 
public func ContextGuard()
{
	[$CtxGuardDesc$|Image=MI_A|Condition=IsBlocked]
}
public func ContextArm()
{
	[$CtxArmDesc$|Image=KMIA|Condition=IsBlocked]
} 

//------------------------------Timer Call---------------------------------------

public func Check()
{
	if (GetID(Contained()) == SELE)
		return SetPlrViewRange(500);
	if (GetID(Contained()) == VHRG)
		return SetPlrViewRange(0);
	var iMode = GetVal(cForm);
	var iViewRange = GetVal(cViewRange);
	var iSlowDown = GetVal(cSlowDown);
	//Menschen haben Nachteile in der Nacht
	if (iMode == cRoleHuman && (IsNight() || !GBackSky()))
		iViewRange = iViewRange / 2;
	//Ghuls Vorteile
	if (iMode == cRoleGhul && (IsNight() || !GBackSky()))
	{
		//Blitzregeneration
		if (GetVal(cBlood) < GetMaxVal(cBlood))
			DoVal(cBlood, 1);
		if (GetVal(cPain) < 20)
			DoEnergy(3);
		//Schmerz und Schlaf regenerieren schneller
		DoVal(cPain, -2);
		DoVal(cSleep, -3);
		//schneller
		iSlowDown += 15;
	}
	//Lichs haben auch Vorteile
	if (iMode == cRoleLich && (IsNight() || !GBackSky()))
	{
		//Schmerz regenerieren
		DoVal(cPain, -1 - Random(2) - !Random(5));
		//schneller
		iSlowDown += 10;
	}
	//Zuviel Sonne ist ungesund...
	if (!IsNight() && GBackSky() && (iMode == cRoleLich || iMode == cRoleVampire))
	{
		if (!Contained())
		{
			DoEnergy(-10000, this, true, FX_Call_EngSun, NO_OWNER);
			for (var i = 0; i < 3; i++)
				Smoke(0, 0, 5);
			iViewRange = iViewRange / 6;
		}
	}
	if (iMode == cRoleLich || iMode == cRoleVampire)
	{
		if (GBackLiquid())
			SetPhysical("BreatheWater", 1, PHYS_Temporary);
		else
			SetPhysical("BreatheWater", 0, PHYS_Temporary);
		// Vom Blut nähren
		if (GetVal(cHunger) > 50)
		{
			DoVal(cBlood, -1);
			DoVal(cHunger, -50);
		}
		//Im Heim heilen
		if (Contained()->~DarkHome())
		{
			if (GetEnergy() < GetMaxVal(cHealth))
			{
				iViewRange = 0;
				DoEnergy(3);
			}
			if (GetVal(cSleep) || GetVal(cPain))
			{
				iViewRange = 0;
				DoVal(cPain, -2);
				DoVal(cSleep, -3);
			}
		}
		//Ohnmacht
		if ((GetVal(cSleep) > GetMaxVal(cSleep) || GetVal(cPain) > GetMaxVal(cPain)) && GetAction() != "Dead")
		{
			M_SL->Activate(this);
			iViewRange = 0;
		}
	}
	//Ohnmacht und Schlaf von Ghul und Menschen
	if (iMode == cRoleGhul || iMode == cRoleHuman)
	{
		if ((GetVal(cSleep) > GetMaxVal(cSleep) - 10 || GetVal(cPain) > GetMaxVal(cPain) - 10 || GetVal(cBlood) < 20) && GetAction() != "Dead")
		{
			M_SL->Activate(this);
			iViewRange = 0;
		}
		if (GetAction() == "Dead")
		{
			SetPlrViewRange(0, this, true);
			//Müdigkeit regenerieren
			DoVal(cSleep, -3);
			//Keine Schmerzen -> Leben
			if (GetVal(cPain) < GetMaxVal(cPain) / 3)
				DoEnergy(1);
			if (GetVal(cPain) || GetVal(cSleep))
				iViewRange = 0;
		}
		else
			if (iMode == cRoleHuman && IsNight())
				DoVal(cSleep, IsNight() * EffectSleepMod(GetAction() == "Dead"));
	}
	//Hungersteigerung
	if (GetVal(cHunger) <= GetMaxVal(cHunger))
		DoVal(cHunger, 1);
	//Blut regenerieren (Vamp nicht)
	if (iMode != cRoleVampire && iMode != cRoleLich)
		DoVal(cBlood, 1);
	//Schmerz regenerieren
	if (iMode == cRoleVampire)
		DoVal(cPain, -2 + Random(8) / 7);
	if (iMode == cRoleHuman)
		if (CanRestorePain())
			DoVal(cPain, -1 - Random(2));
	//Fliegen
	if (iMode == cRoleVampire && GetAction() == "Fly")
	{
		if (GetVal(cBlood) > 1)
			DoVal(cBlood, -2);
		else
			SetAction("Jump");
		if (GetContact(this, -1) & CNAT_Left || GetContact(this, -1) & CNAT_Right)
			SetAction("Scale");
		if (GetContact(this, -1) & CNAT_Top)
			SetAction("Hangle");
	}
	//Magieverarbeitung
	if (GetRole())
		GetRole()->~Magic(this);
	//Unter bestimmten Umständen sterben
	if ((GetVal(cBlood) < 3 && iMode != cRoleVampire && iMode != cRoleLich) || GetVal(cHunger) >= GetMaxVal(cHunger) - 3)
		Death();
	//Schwäche
	if (GetVal(cSleep) > GetMaxVal(cSleep) * 2 / 3 || GetVal(cHunger) > GetMaxVal(cHunger) * 8 / 10)
		Weakness(iSlowDown, iViewRange);
	if (GetVal(cPain) > GetMaxVal(cPain) * 2 / 3)
		Weakness(iSlowDown, iViewRange);
	//Sicht setzen
	SetPlrViewRange(iViewRange, this, true);
	//Vampir bekommt generell einen Phys-Bonus
	if (iMode == cRoleVampire)
		iSlowDown += 10;
	//Physicals, Vampiraura gibt Vampir einen Bonus
	if (iMode == cRoleVampire && FindDarkAura())
	{
		//Log("!!");
		SetPhysicals(iSlowDown + 15, iSlowDown * 2);
	}
	else
		SetPhysicals(iSlowDown, iSlowDown);
}

private func CanRestorePain()
{
	if (GetEffect("Potion", this, 0, 5))
		if (GetEffect("Potion", this, 0, 5)->~IsUrticaOil())
			return false;
	if (GetEffect("AteNettle", this))
		return false;
	return true;
}

private func EffectSleepMod(bool fIsSleeping)
{
	if (GetEffect("Potion", this, 0, 5))
	{
		if (GetEffect("Potion", this, 0, 5)->~IsUrticaOil())
			if (fIsSleeping)
				return Max(1, Random(5) - 2);
			else
				return 0;
	}
	return 1;
}

private func Weakness(& iSlowDown, & iViewRange)
{
	iSlowDown = iSlowDown * 2 / 3;
	iViewRange = iViewRange * 2 / 3;
}

private func SetPhysicals(int iChange, int iFightChange)
{
	var szPhys = ["Walk", "Swim", "Jump", "Scale", "Hangle"];
	for (var sz in szPhys) 
		SetPhysical(sz, GetPhysical(sz, 0, 0, GetID(this)) * iChange / 100, PHYS_Temporary, this);
	SetPhysical("Fight", GetPhysical("Fight", 0, 0, GetID(this)) * iFightChange / 100, PHYS_Temporary, this);
}

// -------------------- Fliegen (na endlich) ---------------------------------

//Fliegen anfangen
protected func ControlUpDouble()
{
	if (GetVal(cForm) == cRoleVampire && GetVal(cBlood) && GetAction() == "Jump")
		SetAction("Fly");
	return _inherited();
}

//Fliegen aufhören
protected func ControlDownDouble()
{
	if (GetVal(cForm) == cRoleVampire && GetAction() == "Fly")
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


// --------------------------- Alle: Waffen imprägnieren ---------------------------

public func Poision(id idImpregnation, object pCarrier)
{
	pCarrier->~Impregnate(idImpregnation);
	RemoveObject(FindContents(idImpregnation));
	PlayerMessage(GetOwner(), "$VHDoped$", this, GetName(0, idImpregnation), GetName(0, GetID(pCarrier)));
}

// ------------  Alle: Alchemie ------------------

public func MakePotion(id idPotion)
{
	var IDs = idPotion->Basic();
	var Numbers = idPotion->Number();
	//prüfen ob alles benötigte da ist
	for (var i = 0; i < GetLength(IDs); i++)
	{
		if (ContentsCount(IDs[i]) >= Numbers[i])
		{
			continue;
		}
		PlayerMessage(GetOwner(), "$VHNotEnough$", this, GetName(0, IDs[i]));
		return 1;
	}
	//Rohstoffe entfernen
	for (var i = 0; i < GetLength(IDs); i++)
	{
		Sound("Alchem*");
		var v = Numbers[i];
		while (v--)
			RemoveObject(FindContents(IDs[i]));
	}
	//erstellen
	CreateContents(idPotion);
}

// -----------------------------Vampir/Diener: Diener erschaffen--------------------------------

protected func MakeSlave(id idnewRole) //Versklavung
{
	//Nochmal nachüberprüfen falls unser Opfer aufgestanden ist
	if (!CanCreateSlaves())
		return false;
	//Für je vier Bürger ein Sklave
	if ((GetPlayerCount() / FindAllSlaves(true) >= 4 || !FindAllSlaves(true)) && GetPlayerCount() >= 4)
	{
		var pClonk = SleepingClonk();
		DoVal(cBlood, -75);
		SetWealth(GetOwner(pClonk), 1);
		PlayerMessage(GetOwner(this), "$VHMadeSlave$", 0, GetPlayerName(GetOwner(pClonk)));
		if (idnewRole == cRoleGhul)
			pClonk->BecomeGhoul(this);
		if (idnewRole == cRoleLich)
			pClonk->BecomeLich(this);
	}
	else
	{
		PlayerMessage(GetOwner(this), "$VHToMuchSlaves$");
	}
} 


public func CanCreateSlaves()
{
	if (GetVal(cForm) == cRoleVampire && SleepingClonk() && (!FindLich() || !FindGhul()) && GetVal(cBlood) > 75)
		return 1;
}

public func BecomeGhoul(object pMaster)
{
	var idMode = cRoleGhul;
	//Alte Form speichern
	idoldRole = GetRole();
	SetRole(idMode);
	idMode->Support(pMaster, this);
	Check();
}

public func BecomeVampire()
{
	var idMode = cRoleVampire;
	SetRole(idMode);
	idMode->Support(this);
	Check();
}

protected func BecomeLich(object pMaster)
{
	var idMode = cRoleLich;
	SetRole(idMode);
	idMode->Support(pMaster, this);
	Check();
}



// -----------------------------Alle: Beten--------------------------------

public func Pray(id idPray)
{
	var pPrayer = CreateContents(idPray);
	pPrayer->~Apply(this);
	PrayCooldown(idPray);
}

public func PrayCooldown(id idPrayer)
{
	if (!idPrayer->~GetPrayCooldown())
		SetVal(cCooldown, 45);
	else
		SetVal(cCooldown, idPrayer->~GetPrayCooldown());
	return true;
}

//---------------------------------Suchläufe--------------------------

global func FindVampire()
{
	for (var pClonk in FindObjects(Find_ID(MEDC))) 
		if (pClonk->GetRole() == cRoleVampire)
			return pClonk;
	return false;
}

global func FindGhul()
{
	for (var pClonk in FindObjects(Find_ID(MEDC))) 
		if (pClonk->GetRole() == cRoleGhul)
			return pClonk;
}

global func FindLich()
{
	for (var pClonk in FindObjects(Find_ID(MEDC))) 
		if (pClonk->GetRole() == cRoleLich)
			return pClonk;
}

global func FindAllSlaves(bool fCount)
{
	var pClonks = [];
	for (pClonk in FindObjects(Find_ID(MEDC))) 
		if (pClonk->GetRole() == cRoleGhul || pClonk->GetRole() == cRoleLich)
			pClonks[GetLength(pClonks)] = pClonk;
	if (fCount)
		return GetLength(pClonks);
	return pClonks;
}

//Findet eine Schatenaura
protected func FindDarkAura()
{
	var aClonks;
	aClonks = FindObjects(Find_ID(MEDC), Find_Distance(150, 0, 0));
	for (var pClonk in aClonks) 
	{
		if (GetEffect("DarkAura", pClonk))
			return true;
	}
}

// schlafender Clonk in der Nähe
public func SleepingClonk()
{
	if (GetAction() != "Walk" && GetAction() != "Swim")
		return 0;
	var pClonk = FindObject2(Find_ID(GetID(this)), Find_Action("Dead"), Find_Exclude(this), Find_InRect(-7, -9, 14, 18));
	return pClonk;
}

// Mordwaffen
public func Weapon()
{
	if (GetRole() == cRoleLich)
		return MEDC;
	if (GetRole() == cRoleGhul)
		return MEDC;
	if (GetRole() == cRoleVampire)
		return M_BT;
	if (ContentsCount(STAK))
		return STAK;
	if (ContentsCount(AXE1))
		return AXE1;
	if (ContentsCount(SWOR))
		return SWOR;
}

//--------------------------------Schaden und Tod-------------------------------------

protected func Death(id idDef, id idCorpse)
{
	// den Beutel fallenlassen
	if (GetAlchemBag())
		GetAlchemBag()->~Loose();
	//falls keine Leiche angegeben, Standardleiche
	if (!idCorpse)
		idCorpse = CP_2;
	//InventarLeeren
	DropItems();
	//HUD entfernen
	RemoveObject(FindObject2(Find_ID(SHUD), Find_Owner(GetOwner(this))));
	//Vampir tot -> Alle Diener sterben auch
	if (GetVal(cForm) == cRoleVampire && FindAllSlaves())
		for (var pPartner in FindAllSlaves()) 
			pPartner->Death(idDef);
	Sound("Die");
	DeathAnnounce();
	var pCorpse = CreateObject(idCorpse, 0, 8, -1);
	LocalN("name", pCorpse) = GetName();
	if (idDef == AXE1 || idDef == MEDC)
		pCorpse->~Head();
	//nur Dorfbewohner relunchen!
	//Log("%v", GetOwner(this));
	if (GetVal(cForm) == cRoleHuman && GetVal(cJob) != cRoleGhul)
		GameCallEx("RelaunchPlayer", GetOwner(this), this);
	RemoveObject(this);
}

protected func DropItems()
{
	var aItems = FindObjects(Find_Container(this));
	for (var p in aItems) 
		Exit(p);
	return true;
}

//-------------------------------Neues Treffersystem-----------------------------

protected func QueryCatchBlow(object pObj)
{
	if (inherited(pObj))
		return 1;
	//Damit der Vampir von nichts getroffen wird, während dem Schattenlauf
	if (GetEffect("ShadowRun", this))
		return 1;
} 

protected func CatchBlow(int iLevel, object pObj)
{
	if (!pObj)
		return 1;
	var fDoDamage = pObj->~HitEffect(this);
	//Hiteffect könnte etwas mit dem Objekt gemacht haben...
	if (!pObj)
		return 1;
	pObj->~BounceOff();
	if (fDoDamage)
	{
		DoEnergy(iLevel, this);
	}
	return inherited(iLevel, pObj);
}

// Schlag mit Handwaffe
private func Striking()
{
	// Waffe bestimmen 
	var pWeapon;
	if (!(pWeapon = GetFightWeapon()))
		return 0;
	// Sound 
	pWeapon->~StrikeSound();
	// Schaden ausrechnen
	var pEnemy = GetActionTarget();
	var iDamage = pWeapon->~GetStrikeDamage();
	iDamage = iDamage * (100 - 3 * Sqrt(pEnemy->~IsShielded())) / 100;
	// Schlagen!
	DoEnergy(-iDamage, pEnemy);
	pWeapon->~MeleeEffect(pEnemy);
	return 1;
}
 
//-------------------------Wertveränderung von außen und Wertfuncs ------------------------

// Nahrungsaufnahme
public func Feed(int iAmount, bool fIsBlood)
{
	if (GetVal(cForm) == cRoleHuman || GetVal(cForm) == cRoleGhul)
	{
		DoEnergy(iAmount / 2);
		DoVal(cHunger, -iAmount * 10);
		Sound("KnightMunch");
	}
	else
	{
		if (fIsBlood)
		{
			DoVal(cBlood, iAmount);
			Sound("BloodSuck*");
		}
		else
		{
			PlayerMessage(GetOwner(this), "$VHHumanFood$", this);
			return 0;
		}
	}
	return 1;
}

//Schmerzänderung
public func DoPain(int iAmount)
{
	//Aufschrei
	if (iAmount > 0)
	{
		Sound("Hurt*");
		iAmount = iAmount * DamageMod(GetVal(cForm)) / 100;
	}
	DoVal(cPain, iAmount);
}

//Blutverlieren mit optischen Effekt
public func LoseBlood(int iAmount, int iY, bool fBack)
{
	var iDir = 2 * GetDir(this) - 1;
	if (fBack)
		iDir *= -1;
	DoVal(cBlood, -iAmount);
	if (Contained(this))
		return -1;
	for (var i = 0; i < iAmount * 3; i++)
	{
		CreateParticle("Blood", 3 * iDir, RandomX(-2, 2) + iY, RandomX(6, 12) * iDir, RandomX(-3, 1), 10, RGB(230, 10, 10));
	}
}

//-------------------------- Abfragen ------------------------------------

public func GetVal(int iMode)
{
	if (iMode == cHunger)
		return iHunger;
	if (iMode == cPain)
		return iPain;
	if (iMode == cSleep)
		return iSleep;
	if (iMode == cBlood)
		return iBlood;
	if (iMode == cCooldown)
		return iCooldown;
	if (iMode == cHealth)
		return GetEnergy(this);
	if (iMode == cMana)
		return GetMagicEnergy(this);
	if (iMode == cForm)
	{
		if (GetRole() == cRoleVampire)
			return cRoleVampire;
		if (GetRole() == cRoleLich)
			return cRoleLich;
		if (GetRole() == cRoleGhul && IsGhulForm())
			return cRoleGhul;
		else
			return cRoleHuman;
	}
	if (iMode == cSlowDown)
		return iSlower;
	if (iMode == cViewRange)
		return iPlrViewRange;
	return -1;
}

public func GetMaxVal(int iMode)
{
	if (iMode == cHunger)
		return 1000;
	if (iMode == cPain)
		return GetRole()->Pain();
	if (iMode == cSleep)
		return GetRole()->Sleep();
	if (iMode == cBlood)
		return GetRole()->Blood();
	if (iMode == cHealth)
		return GetRole()->Health();
	if (iMode == cMana)
		return GetRole()->Mana();
	if (iMode == cCooldown)
		return iMaxCooldownTime;
	return -1;
}

public func DoVal(int iMode, int iChange)
{
	SetVal(iMode, GetVal(iMode) + iChange);
}

public func SetVal(int iMode, int inewValue)
{
	//Keine negativen Werte
	if (inewValue < 0)
		inewValue = 0;
	//Schmerz und Müdigkeit kann etwas über das Maximum steigen
	if (iMode == cPain || iMode == cSleep)
	{
		if (inewValue > GetMaxVal(iMode) + 200)
			inewValue = GetMaxVal(iMode) + 200;
	}
	else
		if (inewValue > GetMaxVal(iMode) && GetMaxVal(iMode) != -1 && iMode != cCooldown)
			inewValue = GetMaxVal(iMode);
	//Werte setzen
	if (iMode == cHunger)
		iHunger = inewValue;
	else
		if (iMode == cPain)
			iPain = inewValue;
		else
			if (iMode == cSleep)
				iSleep = inewValue;
			else
				if (iMode == cBlood)
					iBlood = inewValue;
				else
					if (iMode == cCooldown)
					{
						iCooldown = inewValue;
						iMaxCooldownTime = inewValue;
					}
					else
						if (iMode == cSlowDown)
							iSlower = inewValue;
						else
							if (iMode == cViewRange)
								iPlrViewRange = inewValue;
							else
								return -1;
}

public func GetRole()
{
	return idRole;
}
public func GetOldRole()
{
	return idoldRole;
}
public func IsGhulForm()
{
	return fGhul;
}
public func SetGhulForm(bool fIsGhul)
{
	fGhul = fIsGhul;
}
public func SetRole(id idDef)
{
	idRole = idDef;
}
public func IsDownCooled()
{
	return iCooldown == 0;
}

//Kontinuirlich runterzählen
protected func FxIntCoolDownTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	if (iMaxCooldownTime == 0)
		return 1;
	if (iCooldown > 0)
		iCooldown--;
	else
		iMaxCooldownTime = 0;
}

//**TODO**// Ab hier alles noch verbessern
//############################# Armbrust - übernommen aus M&M Assa.

private func FireCrossbowAt(int iX, int iY, bool fForce)
{
	// Zielt gar nicht: erst mal laden 
	if (!IsAiming())
	{
		LoadCrossbow(FindContents(CRBW), 1);
		return 0;
	}
	FireBowAt(iX, iY, fForce);
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
	ShiftContents(0, 0, CRBW);
	// Schon beim Nachladen? 
	if (IsLoading())
		return 1;
	// War gerade beim Zielen: letzten Winkel sichern 
	if (IsAiming())
		iLastAimAngle = GetPhase();
	else
		iLastAimAngle = 4;
	if (Contents(0, this)->IsRdy())
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
	if (GetPhase() == 7)
	{
		if (Local()++ < 2)
			SetPhase(2);
		else
			Local() = 0;
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
	return GetAction() == "AimBow" || GetAction() == "AimCrossbow";
} 
 
//! Lädt der Clonk gerade den Bogen nach? 
private func IsLoading()
{
	return GetAction() == "LoadBow" || GetAction() == "LoadCrossbow";
}

/* Bolzen aus dem Inventar nehmen */ 
public func GetBolt(idID)
{
	// Einzelne Bolzen suchen 
	var pObj, pBolt;
	for (var i = 0; pObj = Contents(i); i++)
		if (pObj->~IsBolt() || pObj->~IsHook())
			// ID-Vergleich
			if (!idID || GetID(pObj) == idID)
				return pObj;
	// Bei Bolzen Pakete aufteilen 
	for (var i = 0; pObj = Contents(i); i++)
		if (pObj->~IsBoltPack())
			// ID-Vergleich 
			if (!idID || pObj->~UnpackTo() == idID)
				// Bolzen aus Paket verwenden
				if (pBolt = pObj->~GetItem())
					return pBolt;
	// Keine Bolzen gefunden 
	return 0;
}

/* Bolzen im Inventar zählen */ 
private func GetBoltCount()
{
	return GetSpecialCount("IsBolt");
}

/* Überprüft ob der übergebene Typ ein Pfeil ist  */ 
private func CheckBolt(idDef)
{
	return idDef->~IsBolt();
} 
 
/* Überprüft ob der übergebene Typ ein Pfeilpaket ist */ 
private func CheckBoltPack(idDef)
{
	return idDef->~IsBoltPack();
} 

public func BeginLoading()
{
	AddEffect("IntProgress", this, 1, 1, this, 0, 200);
}
protected func FxIntProgressStart(object pTarget, int iNumber, fTmp, iMaxTime)
{
	if (fTmp)
		return;
	// Aktion speichern
	EffectVar(0, pTarget, iNumber) = GetAction();
	// Zeit speichern
	EffectVar(2, pTarget, iNumber) = iMaxTime;
}
protected func FxIntProgressTimer(object pTarget, int iNumber, int iTime)
{
	if (GetAction() != EffectVar(0, pTarget, iNumber))
		return -1;
	if (iTime > EffectVar(2, pTarget, iNumber))
		return;
}
protected func FxIntProgressStop(pTarget, iNumber, iReason, fTmp)
{
	if (fTmp)
		return;
}
