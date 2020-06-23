/*--- Reiﬂklinge ---*/

#strict 2
#include SWOR

public func StrikeSound()        { return Sound("SwordHitp*"); }
public func GetStrikeDamage(){ return 7; }
public func GetCausedPain()  { return 17-Random(4); }
public func GetStrikeTime()  { return 7-Random(4); }
public func GetBloodLoss()   { return 4+Random(5); }
