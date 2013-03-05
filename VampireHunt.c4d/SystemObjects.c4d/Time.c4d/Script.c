/*-- Tageszeiten, keine Abwärtskompatiblität --*/

#strict 2

local iSpeed; //Geschwindigkeit 1-10
local iTime;  //Tageszeit 0-TimeResolution (Mittag-Mittag)
local iHue;   //Helligkeit 0-100

static daycount;  //Globale Werte auch für andere Objekte
static SkyAdjustOrig;
static SkyRed;

private func TimeResolution()
{
	return 3600;
}
private func DayTimeLength()
{
	return TimeResolution() / 2;
}

public func ActualDayTime()
{
	return iTime;
}

// Verzögerte Initialisierung
protected func Initialize()
{
	SetAction("Initialize");
	// Starthelligkeit
	iHue = 100;
	return 1;
}

private func Initialized()
{
	// Geschwindigkeit ermitteln
	iSpeed = ObjectCount(GetID(this)) + 1;
	// Andere Objekte des gleichen Typs entfernen
	for (var pOther in FindObjects(Find_ID(GetID(this)), Find_Exclude(this))) 
		RemoveObject(pOther);
	// Himmelsmodulation speichern
	SkyAdjustOrig = GetSkyAdjust(false);
	// Steuerungsaktion
	SetAction("Time");
	// Fertig
	return 1;
}

// Helligkeitsberechnung
public func GetLightIntensity(int iTime)
{
	var iVal = Cos((iTime * 360) / TimeResolution(), 250) - Cos((DayTimeLength() * 180) / TimeResolution(), 250);
	return BoundBy(iVal, 0, 100);
}
  
// Konfiguration
public func SetTime(int inewTime)
{
	// Zeit setzen
	iTime = inewTime * TimeResolution() / 100;
	// Helligkeit anpassen
	iHue = GetLightIntensity(iTime);
	RestoreSkyColors(iHue);
}
  
// Zeitverlauf
private func Timing()
{
	//Spieler-status und Anzahl
	var villager, vampire;
	
	var iPlrCnt = GetPlayerCount(C4PT_User);
	while (iPlrCnt--)
	{
		if (GetCursor(iPlrCnt) && GetID(Contained(GetCursor(iPlrCnt))) != SELE)
		{
			if (GetCrew(iPlrCnt)->GetVal(cForm) == cRoleHuman || GetCrew(iPlrCnt)->GetVal(cForm) == cRoleGhul)
				villager++;
			if (GetCrew(iPlrCnt)->GetVal(cForm) == cRoleVampire)
				vampire++;
		}
	}
	
	//Statusanzeigen bei Dämmerung und Abend
	if (iTime == TimeResolution() / 4)
	{
		Message("$twilight$", 0, villager, vampire);
		Sound("Nightfall", true);
	}
	
	if (iTime == TimeResolution() * 3 / 4)
	{
		daycount++;
		Sound("Nightfall", true);
		Message("$dawn$", 0, daycount, villager, vampire);
	}
	// Zeitfortschritt
	iTime = (iTime + iSpeed) % TimeResolution();
	// Helligkeit nach Tageszeit berechnen
	var iIntensity = GetLightIntensity(iTime);
	// Helligkeit und Himmelsfarbe anpassen
	if (iIntensity > iHue)
		RestoreSkyColors(iHue++);
	if (iIntensity < iHue)
		RestoreSkyColors(iHue--);
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
  
// Status

global func IsDay()
{
	var pTime;
	if (!(pTime = FindObject2(Find_ID(TIME))) || LocalN("iHue", pTime))
		return true;
	return false;
}
  
global func IsNight()
{
	return !IsDay();
}
    
// Himmelsfarbe

private func RestoreSkyColors(int iPercent)
{
	// Minimale Gammakontrolle (Rampe 3)
	var lt = iPercent / 2 + 78;
	SetGamma(0, RGB(lt, lt, 128), 16777215, 3);
	// NewGfx: Einfach SetSkyAdjust
	SetSkyAdjust(RGBa(iPercent * GetRGBValue(SkyAdjustOrig, 1) / 100, iPercent * (GetRGBValue(SkyAdjustOrig, 2) - SkyRed / 2) / 100, iPercent * (GetRGBValue(SkyAdjustOrig, 3) - SkyRed) / 100, iPercent * GetRGBValue(SkyAdjustOrig, 0) / 100), GetSkyAdjust(false));
}