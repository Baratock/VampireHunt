/*-- Status Hud --*/

#strict 2

local pOwner;
static const SHUD_Blood = 1;
static const SHUD_Hunger = 2;
static const SHUD_Sleep= 3;
static const SHUD_Pain = 4;
static const SHUD_Blocker = 5;
static const SHUD_Exp = 6;
static const SHUD_Bottle = 20;

protected func Initialize()
{	
	SetVisibility(VIS_None,this);
	SetPosition(90,110,this);
	return true;
}

protected func Check()
{
	SetPosition(90,110,this);
	if(!pOwner || !GetAlive(pOwner))
		RemoveObject(this);
	if(GetCursor(GetOwner(this)) == pOwner)
	{
		SetVisibility(VIS_Owner,this);

		//Blut
		SetGraphics("Bar",this,GetID(),SHUD_Blood,2,"Bottle");
		SetClrModulation(RGB(160,30,30),this,SHUD_Blood);
		SetGraphics("Blood",this,GetID(),SHUD_Blood+SHUD_Bottle,2,"Bottle");
		SetObjDrawTransform(1000,0,1000*(-48),0,1000,-1000*4,this,SHUD_Blood+SHUD_Bottle);
		SetObjDrawTransform(600,0,1000*(-48),0,1000*LocalN("blood",pOwner)/pOwner->~MaxBlood(),-1000*6+1000*33-1000*33*LocalN("blood",pOwner)/pOwner->~MaxBlood(),this,SHUD_Blood);
		
		//Hunger
		SetGraphics("Bar",this,GetID(),SHUD_Hunger,2,"Bottle");
		SetClrModulation(RGB(150,90,10),this,SHUD_Hunger);
		SetGraphics("Blood",this,GetID(),SHUD_Hunger+SHUD_Bottle,2,"Bottle");
		SetObjDrawTransform(1000,0,1000*(-27),0,1000,-1000*4,this,SHUD_Hunger+SHUD_Bottle);
		SetObjDrawTransform(600,0,1000*(-27),0,1000*LocalN("hunger",pOwner)/1000,-1000*6+1000*33-1000*33*LocalN("hunger",pOwner)/1000,this,SHUD_Hunger);
		
		//Müdigkeit
		var sleep = LocalN("sleep",pOwner);
		if(sleep > pOwner->~MaxSleepnes())
			sleep = pOwner->~MaxSleepnes();
		
		SetGraphics("Bar",this,GetID(),SHUD_Sleep,2,"Bottle");
		SetClrModulation(RGB(30,30,240),this,SHUD_Sleep);
		SetGraphics("Blood",this,GetID(),SHUD_Sleep+SHUD_Bottle,2,"Bottle");
		SetObjDrawTransform(1000,0,1000*(-6),0,1000,-1000*4,this,SHUD_Sleep+SHUD_Bottle);
		SetObjDrawTransform(600,0,1000*(-6),0,1000*sleep/pOwner->~MaxSleepnes(),-1000*6+1000*33-1000*33*sleep/pOwner->~MaxSleepnes(),this,SHUD_Sleep);
		
		//Schmerz
		var pain = LocalN("pain",pOwner);
		if(pain > pOwner->~MaxPain())
			pain = pOwner->~MaxPain();
		
		SetGraphics("Bar",this,GetID(),SHUD_Pain,2,"Bottle");
		SetClrModulation(RGB(30,30,30),this,SHUD_Pain);
		SetGraphics("Blood",this,GetID(),SHUD_Pain+SHUD_Bottle,2,"Bottle");
		SetObjDrawTransform(1000,0,1000*16,0,1000,-1000*4,this,SHUD_Pain+SHUD_Bottle);
		SetObjDrawTransform(600,0,1000*16,0,1000*pain/pOwner->~MaxPain(),-1000*6+1000*33-1000*33*pain/pOwner->~MaxPain(),this,SHUD_Pain);
		
		//Cooldown
		SetGraphics("Bar",this,GetID(),SHUD_Blocker,2,"Bottle");
		SetClrModulation(RGB(190,150,30),this,SHUD_Blocker);
		SetGraphics("Blood",this,GetID(),SHUD_Blocker+SHUD_Bottle,2,"Bottle");
		SetObjDrawTransform(1000,0,1000*36,0,1000,-1000*4,this,SHUD_Blocker+SHUD_Bottle);
		SetObjDrawTransform(600,0,1000*36,0,1000*pOwner->~GetCoolDownTimer()/pOwner->~GetCoolDown(),-1000*6+1000*33-1000*33*pOwner->~GetCoolDownTimer()/pOwner->~GetCoolDown(),this,SHUD_Blocker);
		
		//Wenn noch Level über der Aktuellen Stufe dann Exp Balken anzeigen
		if(!pOwner->~IsMaxLevel())
		{
			//ExpBalken
			var xoff = 26, yoff = 34;
			
			SetGraphics("Bar",this,GetID(),SHUD_Exp,2,"Bottle");
			SetClrModulation(RGB(200,120,30),this,SHUD_Exp);
			SetObjDrawTransform(
			0			, pOwner->~LevelExpPer2K(), 1000*(xoff-91)+pOwner->~LevelExpPer2K()*30,
			900		, 0												, 1000*yoff	,
			this,SHUD_Exp);
			
			SetGraphics("Blood",this,GetID(),SHUD_Exp+SHUD_Bottle,2,"Bottle");
			SetObjDrawTransform(
			0			, -1000	, 1000*(xoff-62),
			1000	, 0			, 1000*yoff			,
			this, SHUD_Exp+SHUD_Bottle);
			
			SetGraphics("Blood",this,GetID(),SHUD_Exp+1+SHUD_Bottle,2,"Bottle");
			SetObjDrawTransform(
			0			, 1000, 1000*xoff,
			-1000	, 0		, 1000*yoff,
			this, SHUD_Exp+1+SHUD_Bottle);
			
		}
		else
		{
			SetGraphics(0,this,0,SHUD_Exp);
			SetGraphics(0,this,0,SHUD_Exp+SHUD_Bottle);
			SetGraphics(0,this,0,SHUD_Exp+SHUD_Bottle+1);
		}
	}
	else
		SetVisibility(VIS_None,this);
}

protected func SetClonk(object pClnk)
{
	pOwner = pClnk;
	SetOwner(GetOwner(pOwner),this);
	SetVisibility(VIS_Owner,this);
	Check();
	return true;
}
