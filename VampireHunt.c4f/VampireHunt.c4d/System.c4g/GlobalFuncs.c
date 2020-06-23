#strict 2

global func DrawMatRect(string szMat, int x, int y, int w, int h)
{
	return DrawMaterialQuad(szMat,x,y,x+w,y,x+w,y+h,x,y+h);
}

global func UsePoision(object pPoision, &poision, object pTarget)
{
	if(poision == 0)
		return 0;
	poision -> Affect(pTarget,pPoision);
	poision = 0;
	//clear clr modulation
	SetClrModulation(0,pPoision);
	//reset name
	SetName(GetName(0,GetID(pPoision)));
	return 1;	
}

global func FxFadeOutStart (object pTarget, int iEffectNumber, int iTemp)
{
    SetClrModulation(RGBa(255,255,255,0),pTarget);
    if(pTarget->IsArrow())
    {
        SetCategory(C4D_Object,pTarget);
        SetMass(0,pTarget);
        SetAction("Idle",pTarget);
    }
}

global func FxFadeOutTimer (object pTarget, int iEffectNumber, int iEffectTime)
{
    var iaClr;
    iaClr = GetRGBaValue(GetClrModulation(pTarget),0);
    if(iaClr == 255)
    {
        return -1;
    }
    //erhöht alpha  
    SetObjAlpha(iaClr + 3,pTarget);
}

global func FxFadeOutStop (object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
    return RemoveObject(pTarget);
}

global func SetObjAlpha(int byAlpha, object pObj)
{
    var dwClrMod = GetClrModulation(pObj);
    if (!dwClrMod)
    { 
        dwClrMod = byAlpha << 24; 
    }
    else
    { 
        dwClrMod = dwClrMod & 16777215 | byAlpha << 24;
    }
    return SetClrModulation(dwClrMod, pObj);
}

global func FxDummyTimer()
{
    return -1;
}

global func FxTimerMenuStart(object pTarget, int iEffectNumber, int iTemp, var1)
{
	EffectVar(0,pTarget,iEffectNumber) = var1;
}

global func FxTimerMenuTimer (object pTarget, int iEffectNumber, int iEffectTime)
{
	PlayerMessage(GetOwner(pTarget),Format("Noch %d Sekunden.",(37*10-iEffectTime)/37));
	if(iEffectTime >= 37*10 || !GetMenu(pTarget))
		return -1;
}

global func FxTimerMenuStop (object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
	if(GetMenu(pTarget))
		eval(EffectVar(0,pTarget,iEffectNumber));
}

