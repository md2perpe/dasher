#ifndef __mouse_input_h__
#define __mouse_input_h__

#include "../Common/Common.h"
#include "../DasherCore/DasherInput.h"
#include "../DasherCore/DasherTypes.h"

#include <iostream>

using namespace Dasher;

class CDasherMouseInput : public CDasherInput {
public:
  CDasherMouseInput(CEventHandler * pEventHandler, CSettingsStore * pSettingsStore) 
    : CDasherInput(pEventHandler, pSettingsStore, 0, 0, _("Mouse Input")) {

    m_iX = 0;
    m_iY = 0;
  };

  // Fill pCoordinates with iN coordinate values, return 0 if the
  // values were in screen coordinates or 1 if the values were in
  // Dasher coordinates.

  virtual int GetCoordinates(int iN, myint * pCoordinates) {

    pCoordinates[0] = m_iX;
    pCoordinates[1] = m_iY;

    return 0;
  };

  // Get the number of co-ordinates that this device supplies

  virtual int GetCoordinateCount() {
    return 2;
  };

  void SetCoordinates(myint _iX, myint _iY) {
    m_iX = _iX;
    m_iY = _iY;
  };

private:
  myint m_iX;
  myint m_iY;

};

static SModuleSettings sSettings[] = {
  {LP_YSCALE, T_LONG, 10, 2000, 1, 1, _("Pixels covering Y range")}
};

class CDasher1DMouseInput:public CDasherInput {
public:
  CDasher1DMouseInput(CEventHandler * pEventHandler, CSettingsStore * pSettingsStore) 
    /* TRANSLATORS: Only use the vertical mouse coordinate - this is prefered for some disabled users. */
    : CDasherInput(pEventHandler, pSettingsStore, 2, 0, _("One Dimensional Mouse Input")) {

    m_iOffset = 0;

    m_iY = 0;
    m_iRealY = 0;
  };

  // Fill pCoordinates with iN coordinate values, return 0 if the
  // values were in screen coordinates or 1 if the values were in
  // Dasher coordinates.

  virtual int GetCoordinates(int iN, myint * pCoordinates) {
    pCoordinates[0] = m_iY; 
    return 1;
  };

  // Get the number of co-ordinates that this device supplies

  virtual int GetCoordinateCount() {
    return 1;
  };

  virtual void SetMaxCoordinates(int iN, myint * iDasherMax) {
    // FIXME - need to cope with the more general case here

    m_iDasherMaxX = iDasherMax[0];
    m_iDasherMaxY = iDasherMax[1];
  };

  void SetCoordinates(myint iY, myint iScale) {
    m_iRealY = iY;
    m_iY = (iY - m_iOffset) * 4096 / iScale + 2048;
  };

  void KeyDown(int iTime, int iId) {
    if(iId == 10) {
      m_iOffset = m_iRealY;
    }
  };

  bool GetSettings(SModuleSettings **pSettings, int *iCount) {
    *pSettings = sSettings;
    *iCount = sizeof(sSettings) / sizeof(SModuleSettings);
    
    return true;
  };

private:

  myint m_iDasherMaxX;
  myint m_iDasherMaxY;

  myint m_iY;

  myint m_iRealY;

  myint m_iOffset;
};

#endif
