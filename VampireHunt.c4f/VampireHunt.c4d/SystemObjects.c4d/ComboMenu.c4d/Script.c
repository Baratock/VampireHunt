/*-- Das Kombomenü zur Auswahl von Zaubern --*/
//
// Callbacks in den Zauberklassen:
// IsSpellClass(pMage): (bool) Rückgabewert true, wenn die Klasse von pMage angewählt werden kann
// GetElementColor: (dword) DWORD-Wert (ohne Alpha) der Elementfarbe
//
// Callbacks in den Zaubern:
// GetSpellClass: (id) ID der Zauberklasse (FIRE, WATR, EART, AIR1, MISC)
// GetSpellCombo: (string) Tastenkombination (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
//
// Aufruf des Menüs:
// global func CreateSpellMenu(object pCommandObject, object pMenuObj, id idSpellClass)
//
// Callback aus dem Menü (an pCommandObject):
// public func OnComboMenuEnter(id idSpell)  - Spruch ausgewählt
// public func OnComboMenuAbort()            - Menü abgebrochen

#strict 2

local pCallbackObject;   // Befehlsziel für Callbacks
local pTargetObject;     // Besitzobjekt (an das die Sicht geklebt wird, und dessen Besitzer die Auswahl trifft)
local iCastControlCount; // Anzahl der zugewiesenen szCastKey*
local szCastKeys;        // Gedrückte Tasten im Zaubermodus
local fFixedDir;         // Wenn gesetzt, wird Forward/Backward als Rechts/Links-Kombo genommen. Sonst nach Blickrichtung des Clonks.

public func OwnerObject()
{
    return pTargetObject;
}

/* Initialisierung */

public func Create(object pCommandObject, object pMenuObj, bool faFixedDir)
{
    // Felder kopieren
    pCallbackObject = pCommandObject;
    pTargetObject = pMenuObj;
    fFixedDir = faFixedDir;
    // Aktivität setzen
    SetAction("Open");
    // Eine volle Energieleiste sieht doch viel schöner aus :)
    DoEnergy(100);
    // Als Cursorobjekt auswählen, damit Tastaturkommandos gesendet werden und die Sicht stimmt
    SetCursor(GetOwner(), this, 1, 1);
    SelectCrew(GetOwner(), pMenuObj, 0, 1);
    SetViewCursor(GetOwner(), pMenuObj); // Magier als ViewCursor
    // Letztes Kommando löschen, um ControlSpecial auch beim schnellen ControlSpecialDouble abzufangen
    ClearLastPlrCom(GetController(pMenuObj));
    // Überwachungseffekt fürs Schließen an den Clonk
    AddEffect("IntCBMUCheck", pMenuObj, 1, 0, this);
    // Symbole erstellen
    return CheckSpells();
}

protected func Destruction()
{
    // Objekt wird entfernt (irregulär): Abbrechen
    return Abort(true);
}

global func CreateSpellMenu(object pCommandObject, object pMenuObj, bool faFixedDir)
{
    // Menüposition beenden
    var x = pMenuObj->GetX();
    var y = pMenuObj->GetY();
    // Erzeugen
    var pMenu = CreateObject(CBMU,AbsX(x),AbsY(y),GetOwner(pMenuObj));
    if (pMenu)
        pMenu->Create(pCommandObject, pMenuObj, faFixedDir);
    return pMenu;
}


/* Menü schließen */

public func Close(bool fDontRemove)
{
    // Felder zurücksetzen
    iCastControlCount = 0;
    szCastKeys = 0;
    // Alle Symbole löschen
    ClearSpellSymbols();
    // Sicht zurücksetzen - mit Pfeil, aber ohne Rahmen
    if (GetCursor(GetOwner()) == this || !GetCursor(GetOwner()))
    {
        if (pTargetObject)
            SetCursor(GetOwner(), pTargetObject, 1);
        // ViewCursor zurücksetzen
        SetViewCursor(GetOwner());
    }
    // Löschen
    if (!fDontRemove)
        RemoveObject(this);
    // Fertig
    return 1;
}

public func Abort(bool fDontRemove)
{
    // Backup des Zielobjektes, weil dieses Objekt ungültig wird
    // Zugriff auf die Locals könnte gefährlich werden
    var pTarget = pCallbackObject;
    // Selber schließen - löscht das Objekt
    Close(fDontRemove);
    // Callback ans Zielobjekt
    if (pTarget)
        pTarget->~OnComboMenuAbort();
    else 
        return 0;
}

// Blindaufrufe
public func OnComboMenuEnter(id idSpell) {}
public func OnComboMenuAbort()           {}
public func OnComboMenuSpecial()         {}


/* Zaubersymbole */

// Zaubersymbol-Overlay erzeugen
private func CreateSpellSymbol(idSpell, szAction, iPosX, iPosY,fSmall,&riMinX, &riMaxX, &riMinY, &riMaxY)
{
    // Symbol erzeugen
    var pSymbol = CreateObject(MGSM, 0,0, GetOwner());
    pSymbol->SetSymbolGfx(szAction, idSpell, this(), pCallbackObject);
    if(fSmall)
        pSymbol->SetCon(pSymbol->GetCon() / 2);
    SetVertex(0, 0, -iPosX, pSymbol);
    SetVertex(0, 1, -iPosY, pSymbol);
    // Grenzwerte anpassen
    if (iPosX < riMinX)
        riMinX = iPosX;
    else if (iPosX > riMaxX)
        riMaxX = iPosX;
    if (iPosY < riMinY)
        riMinY = iPosY;
    else if (iPosY > riMaxY)
        riMaxY = iPosY;
    // Sichtbarkeit nur für den Besitzer oder neutrale Viewports
    pSymbol->SetVisibility(VIS_Owner | VIS_God);
    // Fertig; Symbol zurückgeben
    return pSymbol;
}

private func GetSpellCombo(id idSpell)
{
    // Standard-Spellcombo
    return idSpell->GetSpellCombo();
}

//liefert alle möglichen Fähigkeiten zurück
public func GetAbilitys(object pTarget)
{
    var i = 0;
    var aAbi = [];
    //ziel finden
    var pCallBack = pTarget;
    if(!pCallBack)
    {
        pCallBack = pCallbackObject;
    }
    //Abis hard coden -> bessere Performance
    var A=[M_PI,M_AS,M_BY,M_BT,M_BB,M_CH,M_RU,M_DE,M_EA,M_GH,M_IV,M_MR,M_PW,M_PT,M_PY,M_RE,M_BS,M_SA,M_SL,M_WL,M_GU,M_SS];
    for(var idPlan in A)
    {
        if(idPlan->~IsAbility() && idPlan->~CanUse(pCallBack))
        {
            aAbi[i] = idPlan;
            i++;
        }
    }
    return aAbi;
}
  
private func CheckSpells()
{
    var i,idSpell,iCount,szKey,szCombo,idClass, iKey, idLastSpell;
    // Alle bekannten Sprüche prüfen
    for(;idSpell = GetAbilitys()[i]; i++)
    {
        // Tastenkombination für den Spruch abfragen
        szCombo = GetSpellCombo(idSpell);
        // Spruch gefunden? (Keine weiteren Tasten erforderlich) -> Ausführen!
        if (szCombo == szCastKeys)
            return DoSpell(idSpell);
        // Bisherige Tastenkombination nicht OK?
        var j = iCastControlCount, fWrong=0;
        while (j--)
            if (GetChar(szCombo, j) != GetChar(szCastKeys, j))
                fWrong=1;
        if (fWrong)
            continue;
        // Spruch sichern
        idLastSpell = Var(12+iCount*2) = idSpell;
        // Zugehörige Taste ermitteln, zählen und sichern
        ++Var(Var(13+iCount++*2) = GetComCharIndex(GetChar(szCombo, iCastControlCount)));
    }
    // Nur ein Spruch gefunden? Aber noch keine Taste gedrückt?
    if (iCount == 1 && szCastKeys != 0) 
     // Dann weitere Kombo überspringen und diesen zaubern  
     return DoSpell(idLastSpell);   
// Kein Spruch passt? Menü abbrechen    
if (!iCount)
         return Abort();    
 // Bild aufbauen   
var iXMin, iXMax, iYMin, iYMax, iSymCount, iMaxSymCount, x,y;   
ClearSpellSymbols();    
i = iCount; 
while (i--)
    {   
    // Zauber und Taste ermitteln   
    idSpell=Var(12+i*2); iKey=Var(13+i*2); iSymCount=Var(iKey); 
    // Winkel ermitteln 
    var iAngle = 0; 
    if(iKey == 0) if (GetDir(pTargetObject) || fFixedDir) iAngle = 150; else iAngle = 30;   
    if(iKey == 1) iAngle = 90;  
    if(iKey == 2) if (GetDir(pTargetObject) || fFixedDir) iAngle = 30; else iAngle = 150;   
    if(iKey == 3) iAngle = -150;    
    if(iKey == 4) iAngle = -90; 
    if(iKey == 5) iAngle = -30; 
    // Und daraus Basisposition 
    var iBaseX = Cos(iAngle, 35), iBaseY = Sin(iAngle, 35);
        // Erster Zauber auf dieser Taste?
        iSymCount = Var(iKey+6)++; iMaxSymCount = Var(iKey);    
    if (!iSymCount) 
        // Tastensymbol erzeugen    
        CreateSpellSymbol(0, GetComName(iKey), iBaseX, iBaseY, true, iXMin, iXMax, iYMin, iYMax);
        // Position für das Zaubersymbol ermitteln  
    // Reihe und Spalte ausrechnen  
    var iRow = 0, iCol = iSymCount, iRemain = iMaxSymCount; 
    while(iCol > iRow)
        {
            iRow++;
            iCol -= iRow;
            iRemain -= iRow;
        }   
    // Radius, Winkelabstand zwischen Symbolen  
    var iRadius = 70 + iRow * 40,   
    iSymAngleDiff = iRadius / 5;    
    // Anzahl Symbole in dieser Reihe   
    var iCols = Min(iRow + 1, iRemain); 
    // Winkel berechnen 
    iAngle = iAngle - (iSymAngleDiff * (iCols - 1)) / 2 + iSymAngleDiff * iCol; 
    x = Cos(iAngle, iRadius);   
    y = Sin(iAngle, iRadius);
        // Zaubersymbol erzeugen
        var pSymbol = CreateSpellSymbol(idSpell, 0, x,y, false, iXMin, iXMax, iYMin, iYMax);
    }
    // Prüfen, ob das Menü aus der Landschaft verschwindet
    var iMoveX, iMoveY;
    if (iXMin+GetX() < 17)
        iMoveX = 17-iXMin-GetX();
    else if (iXMax+GetX()>LandscapeWidth ()-17)
        iMoveX = -17-iXMax-GetX()+LandscapeWidth();
    if (iYMin+GetY() < 17)
        iMoveY = 17-iYMin-GetY();
    else if (iYMax+GetY()>LandscapeHeight()-17)
        iMoveY = -17-iYMax-GetY()+LandscapeHeight();
    if (iMoveX || iMoveY)
        MoveSpellSymbols(iMoveX, iMoveY);
    // Fertig
    return 1;
}

private func ClearSpellSymbols()
{
    var pObj;
    while (pObj = FindObjectOwner(MGSM, GetOwner()))
        RemoveObject(pObj);
}

private func MoveSpellSymbol(object pSymbol, int iByX, int iByY)
{
    SetVertex(0, 0, GetVertex(0, 0, pSymbol)-iByX, pSymbol);
    SetVertex(0, 1, GetVertex(0, 1, pSymbol)-iByY, pSymbol);
    // Fertig
    return 1;
}

private func MoveSpellSymbols(iByX, iByY)
{
    // Alle gefundenen Symbole verschieben
    var pObj;
    while (pObj = FindObjectOwner(MGSM, GetOwner(), 0,0,0,0, 0, 0,0, pObj))
    {
        SetVertex(0, 0, GetVertex(0, 0, pObj)-iByX, pObj);
        SetVertex(0, 1, GetVertex(0, 1, pObj)-iByY, pObj);
    }
    // Fertig
    return 1;
}


/* Steuerung */

protected func CrewSelection(bool fDeselect, bool fCursor)
{
    // Bei Crewabwahl schließen (mit Verzögerung)
    if (fDeselect)
    {
        SetViewCursor(GetOwner()); // ViewCursor zurücksetzen
        SetAction("Aborting");
    }
    return 1;
}

// Mausauswahl - Callback aus den Symbolen heraus
public func MouseSelection(object pSymbol)
{
    // Klassensymbol?
    var idSpell = pSymbol->GetSpellID();
    if (idSpell)
    {
        return DoSpell(idSpell);
    }
    // Pfeilsymbol?
    var szControl = pSymbol->GetControlChar();
    if (szControl)
        return DoCastControl(szControl);
    // Unbekanntes Symbol
    return 0;
}

protected func ControlLeft()          { return(DoCastControl("L")); }
protected func ControlRight()         { return(DoCastControl("R")); }
protected func ControlUp()            { return(DoCastControl("5")); }
protected func ControlDown()          { return(DoCastControl("2")); }
protected func ControlThrow()         { return(DoCastControl("4")); }
protected func ControlDig()           { return(DoCastControl("6")); }

// Spezial/Spezial2 bricht normalerweise ab, wenn nicht gesondert überladen
protected func ControlSpecial()
{
    if (pCallbackObject)
        if (pCallbackObject->~OnComboMenuSpecial(this))
            return 0;
    Abort();
}
  
protected func ControlSpecial2()
{
    if (pCallbackObject)
        pCallbackObject->~OnComboMenuSpecial2(this);
    Abort();
}

protected func DoCastControl(szControl)
{
    // Abbruch, wenn das Zielobjekt weg ist
    if (!pTargetObject)
        return Abort();
    // Links/Rechts umwandeln
    if (szControl == "L")
        if (GetDir(pTargetObject) || fFixedDir)
            szControl="1";
        else
            szControl="3";
    if (szControl == "R")
        if (GetDir(pTargetObject) || fFixedDir)
            szControl="3";
        else
            szControl="1";
  
    // ...spielen wir einen Combosound ab
    Sound(Format("Combo%s", szControl), 0, pCallbackObject, 100, GetOwner(pCallbackObject));
    // Taste speichern
    if (!szCastKeys)
        szCastKeys = szControl;
    else
        szCastKeys = Format("%s%s", szCastKeys, szControl);
    ++iCastControlCount;
    // Letzten Befehl ausleeren, um verspätete Control*Single-Aufrufe abzufangen
    ClearLastPlrCom(GetController());
    // Sprüche prüfen - kann das Ojbekt löschen!
    CheckSpells();
    // Fertig
    return 1;
}

// ComString2Index
private func GetComIndex(szCom)
{
    return GetChar(szCom) - 49; // GetChar("1")==49
}

// ComChar2Index
private func GetComCharIndex(cCom)
{
    return cCom - 49; // GetChar("1")==49
}

// ComString 2 ComName
private func GetComName(iCom)
{
    if (!iCom)
        if (GetDir(pTargetObject) || fFixedDir)
            return("Left");
        else
            return("Right");
    if (iCom == 2)
        if (GetDir(pTargetObject) || fFixedDir)
            return("Right");
        else 
            return("Left"); 
    if (iCom == 1)
        return("Down");
    if (iCom == 3)
        return("Throw");
    if (iCom == 4)
    return("Up");
    if (iCom == 5)
        return("Dig");
    return("<i>Unknown</i>");
}


/* Auswahl */

public func DoSpell(id idSpell)
{
    // Backup des Zielobjektes, weil dieses Objekt ungültig wird und die Local ausgeleert
    var pTarget = pCallbackObject;
    // Selber schließen - löscht das Objekt
    // Beim Schließen aber keinen Abort-Callback ausführen!
    pCallbackObject = 0;
    Close();
    // Callback ans Zielobjekt
    if (pTarget)
        idSpell->Activate(pTarget);
    else
        return 0;
}

/* Hilfseffekt: Menü schließen wenn der Clonk stirbt */

protected func FxIntCBMUCheckStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
    if (fTemp)
        return 0;
    Close(true);
    // Objekt verzögert entfernen, um rekursive Entfernungsaufrufe zu verhindern
    Schedule(Format("RemoveObject(Object(%d))", ObjectNumber(this)), 1);
}
  
