/*-- Körper --*/

#strict 2

local name; //Wer isn das?

protected func Hit()
{
	return Sound("Splat*");
}

public func Head()
{
	SetAction("Headless");
	Sound("Splatter");
	LocalN("name", CreateObject(CP_1, -6, 0, -1)) = name;
}