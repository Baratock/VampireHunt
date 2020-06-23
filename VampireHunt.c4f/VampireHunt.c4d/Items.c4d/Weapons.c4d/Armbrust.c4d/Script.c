/*-- Armbrust geklaut aus M&M --*/

#strict 2

local iPower;
local iAngle;

local fLoad;

local pCrosshair;

/* Aktivieren (zum Zielen per Maussteuerung) */
public func Activate(pClonk)
{
  [$CtrlActivateDesc$]
  // Der Clonk kann dieses Objekt nicht verwenden
  if (!pClonk->~CanUse(GetID())) return 0;
  // Träger nicht bereit zum neuen Zielen
  if (!pClonk->~ReadyToFire()) return;
  // Träger: neue Zielaktion setzen
  if (!pClonk->~LoadCrossbow(1))
      return 0;
  // Fadenkreuz zum besseren Zielen erzeugen
  pCrosshair = CreateObject(WCHR, 0, 0, GetOwner(pClonk));
  pCrosshair->StartCheck("*Crossbow*", pClonk);
  iAngle = 84;
  ActualizePhase(pClonk);
  iPower = 100;
  // Erfolg
  return 1;
}

/* Spezialsteuerung durch Träger */
public func ControlThrow(pClonk)
{
  // Träger hat gezielt
  if (IsAiming())
    { Fire(pClonk); return(1); }
  // Bogen einsetzen
  return Activate(pClonk);
}

public func ControlUp (pClonk)               // hoch zielen
{
  AimUp(pClonk, 1, "ControlConf", CRBW);
}

public func ControlDown(pClonk)             // runter zielen
{
  AimDown(pClonk, 1, "ControlConf", CRBW);
}

public func ControlUpdate(object clonk, int comdir, bool dig, bool throw)
{
  AimUpdate(clonk, comdir, 1, "ControlConf", CRBW);
}

public func ControlConf(int conf, object clonk)
{
  if(clonk->GetAction() != "AimCrossbow" && clonk->GetAction() != "LoadCrossbow")
  {
    AimCancel(clonk);
  }
  else
  {
    // Feinere Abstufung mit JnR möglich
    var angle_change;
    if(GetPlrCoreJumpAndRunControl(clonk->GetController()))
      angle_change = conf * 1;
    else
      angle_change = conf * 6;

    iAngle = BoundBy(iAngle + angle_change, 0, 140);
    ActualizePhase(clonk);
  }
}

public func ActualizePhase(pClonk)
{
  // Nur wenn noch gezielt/geladen wird
  if(!IsAiming() && !IsLoading()) return;

  // Phase des Clonks an Winkel anpassen
  if(GetAction(pClonk)=="AimCrossbow")
  {
    if(iAngle<6)       SetPhase(0, pClonk);
    else if(iAngle<24)  SetPhase(1, pClonk);
    else if(iAngle<36)  SetPhase(2, pClonk);
    else if(iAngle<48)  SetPhase(3, pClonk);
    else if(iAngle<72)  SetPhase(4, pClonk);
    else if(iAngle<90)  SetPhase(5, pClonk);
    else if(iAngle<108)  SetPhase(6, pClonk);
    else SetPhase(7, pClonk);
  }

  // Fadenkreuz anpassen
  var iDir = GetDir(pClonk); if(!iDir) iDir = -1;
  SetVertexXY(0, -Sin(iAngle, 40)*iDir, Cos(iAngle, 40), pCrosshair);

  // Schussparameter sammeln
  var iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir;
  GetCrossbowOut(this, iAngle, 0, iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir);
  iOutXDir *= iDir; iOutX *= iDir;
  // Farbe Grün beim Zielen oder Rot beim Laden
  var iColor = RGB(0, 255);
  if(pClonk->GetAction() == "LoadCrossbow") iColor = RGB(255);
}

public func ControlLeft(pClonk)
{
  // Träger Zielaktion
  if (IsAiming() || IsLoading())
  {
    SetDir(DIR_Left, pClonk);
    ActualizePhase(pClonk);
    return 1;
  }
  // Keine Spezialsteuerung
  return 0;
}

public func ControlRight(pClonk)
{
  // Träger Zielaktion
  if (IsAiming() || IsLoading())
  {
    SetDir(DIR_Right, pClonk);
    ActualizePhase(pClonk);
    return 1;
  }
  // Keine Spezialsteuerung
  return 0;               
}

public func ControlSpecial(pClonk)
{
  // Träger Zielaktion
  if (IsAiming() || IsLoading())
  {
    iPower += 10;
    var iMaxPower = 150;
    if(iPower > iMaxPower) iPower = 50;
    ActualizePhase(pClonk);
    PlayerMessage(GetOwner(pClonk), "Power %d", pClonk, iPower);
    return 1;
  }
  // Keine Spezialsteuerung
  return 0;
}

public func ControlRightDouble(pClonk)
{
  // Zielaktion abbrechen
  if (IsAiming() || IsLoading())
  {
    StopAiming(pClonk);
    SetComDir(COMD_Right, pClonk);
  }
  // Keine Spezialsteuerung
  return 0;
}

public func ControlLeftDouble(pClonk)
{
  // Zielaktion abbrechen
  if (IsAiming() || IsLoading())
  {
    StopAiming(pClonk);
    SetComDir(COMD_Left, pClonk);
  }
  // Keine Spezialsteuerung
  return 0;
}

public func StopAiming(object pClonk)
{
  // Stop 
  SetComDir(COMD_Stop,pClonk); 
  // Basisaktion setzen 
  pClonk->SetAction("Walk",); 
  // Sound 
  Sound("Grab"); 
}

/* Wird gerade vom Träger gezielt? */
public func IsAiming() 
{
  if(!Contained()) return false;
    return (Contained()->~IsAiming()); 
}

public func IsLoading()
{ 
  if(!Contained()) return false;
    return (Contained()->~IsLoading()); 
}

public func ControlDig(pClonk)
{
	if (IsAiming() || IsLoading())
	{
		// Nächste Munition
		Local(10)++;
		Local(9) = GetMunition(Local(10));
		if(Local(9)) 
			PlayerMessage(GetOwner(pClonk), "%s", this, GetName(Local(9)));
		return 1;
	}
	return 0;
}

/* Munitionssuche */
private func GetMunition(iIndex)
{
	if(!Contained()) 
		return 0;
	var aAmmo = [], idTest, idFound, fFound;
	// Liste mit Munitionsid's machen
	for(var pObj in FindObjects(Find_Container(Contained()), Find_Func("IsBoltPack")))
	{
		var idObj = GetID(pObj);
		// Haben wir die ID schon?
	    fFound = 0;
		for(pTest in aAmmo)
			if((GetID(pTest) == idObj) && LocalN("poision",pTest) == LocalN("poision",pObj))
				fFound = 1;
		if(!fFound)
			aAmmo[GetLength(aAmmo)] = pObj;
	}
	// den Eintrag mit passendem Index ausgeben
	return aAmmo[ iIndex % GetLength(aAmmo) ];
} 

// Haben wir noch was von der Munitionssorte?
private func CheckMunitionID(object pMuni)
{
	// Liste mit Munitionsid's machen
	var idFound;
	for(var pObj in FindObjects(Find_Container(Contained()), Find_Func("IsBoltPack")))
	{
		// ID suchen
		idFound = GetID(pObj);
		// Ist die ID die gesuchte?
		if(idFound == GetID(pMuni) && LocalN("poision",pMuni) == LocalN("poision",pObj)) return 1;
	}
}

private func GetMunition2()
{
	var pObj = GetMunition(Local(10));
	if(pObj->~IsBolt())
		return pObj;
	if(pObj->~IsBoltPack())
		return pObj->GetItem();
}

/* Pfeil abfeuern */
public func Fire()
{
  //ist nun nichtmehr geladen
  HasLoaded(false);
  var pClonk = Contained();
  // Kein Träger?
  if(!pClonk) return 0;
  // Im Gebäude?
  if (pClonk->Contained()) { pClonk->~StopAiming(); return(0); }
  // Unvollständig?
  if (~GetOCF() & OCF_Fullcon) return 0;
  // Noch beim Nachladen
  if (GetAction() == "Reload") return 0;
  // Pfeil vom Träger holen
  var pArrow = GetMunition2();
  if (!pArrow) 
  {
    // andere Munition?
    ControlDig(Contained());
    pArrow = GetMunition2();
    // wohl doch nicht :-(
    if (!pArrow) 
      return 0;
  }
  // Beim Träger ggf. Bogen wieder auswählen
  ShiftContents(pClonk, 0, CRBW);
  // Austrittsgeschwindigkeit ermitteln
  var iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir;
  var iDir = GetDir(pClonk); if(!iDir) iDir = -1;
  GetCrossbowOut(this, iAngle, 0, iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir);
  iOutXDir *= iDir; iOutX *= iDir; iOutR *= iDir;
  // Besitzer des Projektils setzen
  pArrow->SetOwner(Contained()->GetOwner());
  // Pfeil abfeuern
  Exit(pArrow, iOutX,iOutY,iOutR, 0,0,iOutRDir );
  pArrow->SetXDir(iOutXDir); pArrow->SetYDir(iOutYDir);
  pArrow->~Launch(pClonk);
  // Sicht verfolgen
  if (GetPlrView(GetController(pClonk)) != 2)
    SetPlrView(GetController(pClonk), pArrow);
  // Sound
  Sound("Crossbow*");
  pClonk->~LoadCrossbow();
  ActualizePhase(pClonk);
  // Fertig!
  return 1;
}

/* Ausgangsgeschwindigkeit und -position, wenn der Ritter mit dem Bogen in angegebene Richtung schießt */
public func GetCrossbowOut(pBow, iAngle, fDeviate, &iOutX, &iOutY, &iOutR, &iOutXDir, &iOutYDir, &iOutRDir) 
{
  // Austrittsgeschwindigkeit ermitteln 
  iOutXDir = Sin(iAngle, iPower);
  iOutYDir = -Cos(iAngle, iPower);
  // Austrittsposition... 
  iOutX = Sin(iAngle, 25 * GetCon() / 100);
  iOutY = -Cos(iAngle, 25 * GetCon() / 100); 
  // Pfeildrehung 
  iOutR = iAngle; 
  if(iAngle > 0) iOutRDir = 3; else iOutRDir = -3;
  // Fertig 
  return 1;
} 


/* Pfeil abfeuern */
public func FireAngle(iAngle)
{
  var pClonk = Contained();
  // Kein Träger?
  if(!pClonk) return 0;
  // Im Gebäude?
  if (pClonk->Contained()) { pClonk->~StopAiming(); return(0); }
  // Unvollständig?
  if (~GetOCF() & OCF_Fullcon) return 0;
  // Noch beim Nachladen
  if (GetAction() == "Reload") return 0;
  // Pfeil vom Träger holen
  var pArrow = GetMunition2();
  if (!pArrow) 
  {
    // andere Munition?
    ControlDig(Contained());
    pArrow = GetMunition2();
    // wohl doch nicht :-(
    if (!pArrow) 
      return 0;
  }
  // Beim Träger ggf. Bogen wieder auswählen
  ShiftContents(Contained(), 0, CRBW);
  // Austrittsgeschwindigkeit ermitteln
  var iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir;
  pClonk->~GetCrossbowOut(this, iAngle, true, iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir);
  // Besitzer des Projektils setzen
  pArrow->SetOwner(Contained()->GetOwner());
  // Pfeil abfeuern
  Exit(pArrow, iOutX,iOutY,iOutR, 0,0,iOutRDir );
  pArrow->SetXDir(iOutXDir); pArrow->SetYDir(iOutYDir);
  pArrow->~Launch(pClonk);
  // Sound
  Sound("Crossbow*");  
  // Nachladen
  SetAction("Reload");
  Contained()->~LoadCrossbow();
  // Munitionscheck
  if(!CheckMunitionID(Local(9)))
  {
    // Leer? Dann andere Munition
    ControlDig(Contained());
  }
  return 1;
}

public func IsRdy()
{
	return fLoad;
}

public func HasLoaded(bool fAimed)
{
	fLoad = fAimed;
}

protected func Incineration()
{
	//Einfärben für Menüs
	SetClrModulation(RGB(50,50,50),this());
	SetAction("Burned");
	return(1);
}

/* Kann geschmiedet werden */
public func IsAnvilProduct() { return 1; }
