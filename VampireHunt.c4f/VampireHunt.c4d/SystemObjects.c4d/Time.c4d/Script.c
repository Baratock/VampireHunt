/*-- Tageszeiten --*/

#strict 2

/* Locals */

//   0: Geschwindigkeit 1-10
//   1: Tageszeit 0-TimeResolution (Mittag-Mittag)
//   2: Helligkeit 0-100
// 4,5: Hilfsobjekte (Abwärtskompatiblität)
// 6-25: Himmelsfarben (Abwärtskompatiblität)

static daycount,end;  //Globale Werte auch für andere Objekte
static SkyAdjustOrig;
static SkyRed;

private func TimeResolution() { return 3600; }
private func DayTimeLength()  { return TimeResolution() / 2; }

public func ActualDayTime()   { return Local(1,this); }

/* Verzögerte Initialisierung */

protected func Initialize()
{
	SetAction("Initialize");
	// Starthelligkeit
	Local(2) = 100;
	return 1;
}

private func Initialized()
{
	// Geschwindigkeit ermitteln
	Local(0) = ObjectCount(GetID(this)) + 1;
	// Andere Objekte des gleichen Typs entfernen
	for(var pOther in FindObjects(Find_ID(GetID(this)),Find_Exclude(this)))
		RemoveObject(pOther);
	// Himmelsmodulation speichern
	SkyAdjustOrig = GetSkyAdjust(false);
	// Steuerungsaktion
	SetAction("Time");
	// Fertig
	return 1;  
}
  
/* Helligkeitsberechnung */
public func GetLightIntensity(int iTime)
{
	return(BoundBy(Cos((iTime * 360) / TimeResolution(), 250) - Cos((DayTimeLength() * 180) / TimeResolution(), 250), 0, 100));
}
  
/* Konfiguration */

public func SetTime(iTime)
{
	// Zeit setzen
	 Local(1) = iTime * TimeResolution() / 100;
	// Helligkeit anpassen
	RestoreSkyColors(Local(2) = GetLightIntensity(Local(1)));
	return 1;
}
  
/* Zeitverlauf */  

private func Timing()
{
    //Alles vorbei? Zeitstop.
    if(end)
    { 
        return 1;
    }
    //Spieler-status und Anzahl
    var villager,ghoul,vampire;
    
    var iPlrCnt = GetPlayerCount(C4PT_User);
    while(iPlrCnt--)
    {
        if(GetCursor(iPlrCnt) && GetID(Contained(GetCursor(iPlrCnt))) != SELE)
        {
            if(GetCursor(iPlrCnt)->~Who() == Role_Human) villager++;
            if(GetCursor(iPlrCnt)->~Who() == Role_Vampire) vampire++;
            if(GetCursor(iPlrCnt)->~Who() == Role_Slave) ghoul++;
        }
    }

    //Statusanzeigen bei Dämmerung und Abend
    if(Local(1) == TimeResolution()/4)
    {
        Message ("$twilight$",0,villager+ghoul,vampire);
        Sound("Nightfall",true);
    }

    if(Local(1) == (TimeResolution()/4)*3)
    {
        daycount++;
        Sound("Nightfall",1);
        Message ("$dawn$",0,daycount,villager+ghoul,vampire);
    }
	// Zeitfortschritt
	Local(1) = (Local(1) + Local(0)) % TimeResolution();
	// Helligkeit nach Tageszeit berechnen
	var iIntensity = GetLightIntensity(Local(1));
	// Helligkeit und Himmelsfarbe anpassen
	if (iIntensity > Local(2)) RestoreSkyColors(++Local(2));
	if (iIntensity < Local(2)) RestoreSkyColors(--Local(2));
	// Fertig
	return 1;
}
  
protected func UpdateTransferZone()
{
	// Kleiner Trick, damit der Himmel nach dem Laden aktualisiert wird
	if (GetAction(this) == "Time") 
		RestoreSkyColors(Local(2));
	return 0;  
}
  
/* Status */

global func IsDay()
{
	var pTime; 
	if (!(pTime = FindObject2(Find_ID(TIME))) || Local(2, pTime))
		return true;
	return false;
}
  
global func IsNight(){ return !IsDay();}
    
/* Himmelsfarbe */

private func RestoreSkyColors(iPercent)
{
  // Alte Speicherung? Übertragen
  if (Local (4)) GetOldSkyColors();
  if (Local (6)) 
  {
    var i;
    // ehemaliges OldGfx: Normales SetSkyColor
    while(i < 20) RestoreSkyColor(i++, 100);
  }

  // Minimale Gammakontrolle (Rampe 3)
  var lt = iPercent / 2 + 78;
  SetGamma(0, RGB(lt, lt, 128), 16777215, 3);

  // NewGfx: Einfach SetSkyAdjust
  SetSkyAdjust(RGBa(
    iPercent * GetRGBValue(SkyAdjustOrig,1) / 100,
    iPercent * (GetRGBValue(SkyAdjustOrig,2)-SkyRed/2) / 100, //Rot von den anderen abziehen
    iPercent * (GetRGBValue(SkyAdjustOrig,3)-SkyRed) / 100,
    iPercent * GetRGBValue(SkyAdjustOrig,0) / 100),
  GetSkyAdjust(0));

  return 1;
}
  
private func RestoreSkyColor(int iColor, int iPercent)
{
  SetSkyColor(iColor,
              ((Local(iColor+6)>>16 & 255) * iPercent)/100,
              ((Local(iColor+6)>> 8 & 255) * iPercent)/100,
              ((Local(iColor+6)     & 255) * iPercent)/100);
  Local(iColor + 6) = 0;
  return 0;
}

private func GetOldSkyColors()
{
  var i;
  i=-1; while (++i<11) Local(i+ 6)=Local(i,Local(4));
  i=-1; while (++i<11) Local(i+16)=Local(i,Local(5));
  // Alte Hilfsobjekte entfernen
  RemoveObject(Local(4));
  RemoveObject(Local(5));
  return 1;
}
