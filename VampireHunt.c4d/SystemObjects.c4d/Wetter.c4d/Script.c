/*-- Wetter **TODO** --*/

#strict 2

local iTimer;

protected func Initialize()
{
	Check();
	SoundLevel("rain", 100);
	SetPosition(0, 0);
}

public func Check()
{
	iTimer = RandomX(100, 500);
	if (GetTemperature() > 0)
	{
		SetAction("Rain");
	}
	else
	{
		SetAction("Hail");
	}
}

public func CrRain()
{
	var x = RandomX(0, LandscapeWidth());
	if (iTimer > 0)
	{
		CreateParticle("Drop", x, 0, RandomX(GetWind(0, 0, 1) / 4, GetWind(0, 0, 1) / 2), RandomX(200, 300), 5 * 64 + Random(32));
		iTimer--;
		if (Random(2))
			Sound("thundercrack*");
	}
	else
	{
		Check();
	}
}

public func CrHail()
{
	var x = RandomX(0, LandscapeWidth());
	if (iTimer > 0)
	{
		CreateParticle("Hail", x, 0, RandomX(GetWind(0, 0, 1) / 4, GetWind(0, 0, 1) / 2), RandomX(200, 300), 5 * 3 + Random(1));
		iTimer--;
		if (Random(2))
			Sound("thundercrack*");
	}
	else
	{
		Check();
	}
}
