/*-- Bolzenpaket --*/

#strict 2

#include ARWP

/* Paketeigenschaften */

public func UnpackTo() { return BOLT; }
public func IsBoltPack() { return 1; }
public func MaxPackCount() { return 10; }
public func IsAnvilProduct() { return 1; }

public func IsArrowPack() { return 0; }

/* Keine Manuelle Zerlegung */

protected func Activate(pClonk) { return 0; }