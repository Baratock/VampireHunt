#strict 2

#include WMPF

protected func Eat(object pClonk)
{
	if(pClonk -> Who())
	{
		pClonk-> RestorePain(10);
		pClonk-> RestoreSleep(-20);
	}
	RemoveObject(this);
	return 1;
}