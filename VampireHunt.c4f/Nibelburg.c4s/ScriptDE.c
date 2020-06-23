/*-- Vampirjagd, allgemeiner Szenscript (übertragbar) --*/

#strict

static choser; //Verteilobjekt
static villagers; //Anzahl der Ersatz-Dörfler
static guard,forest,priest,smith;

func Initialize()
{
 //Geklauter Nebel...
 for(var i=0;i<350;++i) 
   CreateParticle("Fog",Random(LandscapeWidth()),Random(1200),0,0,RandomX(900,1700));

 choser = CreateObject(VHRG,50,50,-1);
 CreateObject(VHMB,50,50,-1);
 CreateObject(_ETG,50,50,-1);
 villagers=8;
 ScriptGo(1);
 return(1);
}


func Script0()
{

 var i,j;
 for (j=GetPlayerCount(); j; ++i)
 {
  if (GetPlayerName(i))
  {
   SetFoW(true,i); //ist der FOW aus?
   var pCursor = GetCursor(i);
   if (pCursor) Enter(choser,pCursor);
   --j;
  }
 }
 choser -> Vampire();

 //Wichtig: Zeit nach dem Wähler setzen
 CreateObject(TIME,100,100,-1);

 ScriptGo(0);

 return(1);
}


//------------------------------Relaunchsystem---------------------------------------

func RelaunchPlayer(plr)
{
 var clonk = CreateObject(MEDC,LandscapeWidth()/2,50,plr);
 LocalN("blood",clonk) = 100; //Provisorisch blut nachfüllen

 MakeCrewMember(clonk,plr);
 SetCursor(plr,GetCrew(plr));

 var soul = CreateObject(SELE,LandscapeWidth()/2,50,plr);
 soul -> SetVisibility(VIS_Owner());
 Enter(soul,clonk);

 if(GetWealth(plr) == 1)
 {
  soul->SetClrModulation(RGBa(55,55,55,127));
  soul->BlockRelaunch(1);
  PlayerMessage(plr,"Ihr konntet im Leben eurem Meister nicht helfen! Helft ihm lieber wenigstens im Tod bevor er euch aus Rache in der Hölle schmoren lässt!");
  return(1);
 }
 if(GetWealth(plr) == 2)
 {
  soul->SetClrModulation(RGBa(255,55,55,127));
  soul->BlockRelaunch(2);
  PlayerMessage(plr,"Nun müsst ihr als Blutgeist ein elendes Dasein fristen...|ihr habt versagt!");
  return(1);
 } 
 if(GetWealth(plr) == 3)
 {
  soul->SetClrModulation(RGBa(255,55,55,127));
  soul->BlockRelaunch(3);
  PlayerMessage(plr,"Möge euer Diener genauso elendig sterben dunkle Gestalt!");
  return(1);
 }
 if(GetWealth(plr) == 4)
 {
  soul->SetClrModulation(RGBa(255,55,55,127));
  soul->BlockRelaunch(4);
  PlayerMessage(plr,"Ihr und euer Trupp soll in der Hölle schmoren!");
  return(1);
 }
 if(GetWealth(plr) == 5)
 {
  soul->SetClrModulation(RGBa(255,55,55,127));
  soul->BlockRelaunch(5);
  PlayerMessage(plr,"Ihr und eure Horde sollt in der Hölle schmoren!");
  return(1);
 }
 if(GetWealth(plr) == 6)
 {
  soul->SetClrModulation(RGBa(255,55,55,127));
  soul->BlockRelaunch(6);
  PlayerMessage(plr,"Ihr und eure Horde sollt in der Hölle schmoren!");
  return(1);
 }      
 soul->SetClrModulation(RGBa(255,255,255,127));
 PlayerMessage(plr,"Eure Sorge lässt euch nicht ruhen.|Vielleicht könnt ihr sogar ins Leben zurückkehren...");
}