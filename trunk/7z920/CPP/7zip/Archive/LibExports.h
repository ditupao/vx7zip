#ifndef LIB_EXPORTS_H
#define LIB_EXPORTS_H



STDAPI CreateObject(const GUID *clsid, const GUID *iid, void **outObject);

STDAPI SetLargePageMode();



#endif