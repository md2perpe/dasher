// DasherInterfaceBase.cpp
//
// Copyright (c) 2007 The Dasher Team
//
// This file is part of Dasher.
//
// Dasher is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// Dasher is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Dasher; if not, write to the Free Software 
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include "../Common/Common.h"

#include "OneButtonDynamicFilter.h"
#include "DasherInterfaceBase.h"
#include "Event.h"

using namespace Dasher;

static SModuleSettings sSettings[] = {
  /* TRANSLATORS: The time for which a button must be held before it counts as a 'long' (rather than short) press. */
  {LP_HOLD_TIME, T_LONG, 100, 10000, 1000, 100, _("Long press time")},
  /* TRANSLATORS: Double-clicks are special in some situations (they cause us to start reversing). This is the time in which the button must be pressed twice to count.*/
  {LP_MULTIPRESS_TIME, T_LONG, 100, 10000, 1000, 100, _("Double-press time")},
  /* TRANSLATORS: Backoff = reversing in Dasher to correct mistakes. This allows a single button to be dedicated to activating backoff, rather than using multiple presses of other buttons.*/
  {BP_BACKOFF_BUTTON,T_BOOL, -1, -1, -1, -1, _("Enable backoff button")},
  {BP_SLOW_START,T_BOOL, -1, -1, -1, -1, _("Slow startup")},
  {LP_SLOW_START_TIME, T_LONG, 0, 10000, 1000, 100, _("Startup time")},
  {LP_DYNAMIC_SPEED_INC, T_LONG, 1, 100, 1, 1, _("Percentage by which to automatically increase speed")},
  {LP_DYNAMIC_SPEED_FREQ, T_LONG, 1, 1000, 1, 1, _("Time after which to automatically increase speed (secs)")},
  {LP_DYNAMIC_SPEED_DEC, T_LONG, 1, 99, 1, 1, _("Percentage by which to decrease speed upon reverse")}
};

COneButtonDynamicFilter::COneButtonDynamicFilter(Dasher::CEventHandler * pEventHandler, CSettingsStore *pSettingsStore, CDasherInterfaceBase *pInterface)
  : CButtonMultiPress(pEventHandler, pSettingsStore, pInterface, 6, 1, _("One Button Dynamic Mode")) {
  m_iTarget = 0;

  m_iTargetX = new int[2];
  m_iTargetY = new int[2];

  m_iTargetX[0] = 100;
  m_iTargetY[0] = 100;

  m_iTargetX[1] = 100;
  m_iTargetY[1] = 3996; 

  m_bDecorationChanged = true;
}

COneButtonDynamicFilter::~COneButtonDynamicFilter() {
  delete[] m_iTargetX;
  delete[] m_iTargetY;  
}

bool COneButtonDynamicFilter::DecorateView(CDasherView *pView) {

  CDasherScreen *pScreen(pView->Screen());

  screenint x1;
  screenint y1;
  screenint x2;
  screenint y2;

  if(m_iTarget == 1) {
    pView->Dasher2Screen(-100, 0, x1, y1);
    pView->Dasher2Screen(-200, 1000, x2, y2);
  }
  else {
    pView->Dasher2Screen(-100, 3096, x1, y1);
    pView->Dasher2Screen(-200, 4096, x2, y2);
  }

  pScreen->DrawRectangle(x1, y1, x2, y2, 1, 1, Opts::ColorSchemes(Opts::Objects), true, false, 2);
    
  if(m_iTarget == 1) {
    pView->Dasher2Screen(-100, 3096, x1, y1);
    pView->Dasher2Screen(-200, 4096, x2, y2);
  }
  else {
    pView->Dasher2Screen(-100, 0, x1, y1);
    pView->Dasher2Screen(-200, 1000, x2, y2);
  }

  pScreen->DrawRectangle(x1, y1, x2, y2, 2, 2, Opts::ColorSchemes(Opts::Objects), true, false, 1);

  bool bRV(m_bDecorationChanged);
  m_bDecorationChanged = false;
  return bRV;
}

void COneButtonDynamicFilter::KeyDown(int Time, int iId, CDasherView *pDasherView, CDasherModel *pModel, CUserLogBase *pUserLog, bool bPos, int iX, int iY) {
  if (iId == 100 && !GetBoolParameter(BP_BACKOFF_BUTTON))
    //mouse click - will be ignored by superclass method.
    //simulate press of button 2...
    CButtonMultiPress::KeyDown(Time, 2, pDasherView, pModel, pUserLog);
  else
    CInputFilter::KeyDown(Time, iId, pDasherView, pModel, pUserLog, bPos, iX, iY);
}

void COneButtonDynamicFilter::KeyUp(int Time, int iId, CDasherView *pDasherView, CDasherModel *pModel, bool bPos, int iX, int iY) {
  if (iId == 100 && !GetBoolParameter(BP_BACKOFF_BUTTON))
    //mouse click - will be ignored by superclass method.
    //simulate press of button 2...
    CButtonMultiPress::KeyUp(Time, 2, pDasherView, pModel);
  else
    CInputFilter::KeyUp(Time, iId, pDasherView, pModel, bPos, iX, iY);
}

bool COneButtonDynamicFilter::TimerImpl(int Time, CDasherView *m_pDasherView, CDasherModel *m_pDasherModel, Dasher::VECTOR_SYMBOL_PROB *pAdded, int *pNumDeleted, CExpansionPolicy **pol) {
  m_pDasherModel->OneStepTowards(m_iTargetX[m_iTarget], m_iTargetY[m_iTarget], Time, pAdded, pNumDeleted);
  return true;
}

void COneButtonDynamicFilter::ActionButton(int iTime, int iButton, int iType, CDasherModel *pModel, CUserLogBase *pUserLog) {
  if (iType != 0) {
    //double/long push
    reverse();
    return;
  }
    
  if((iButton == 2) || (iButton == 3) || (iButton == 4)) {
    if(pUserLog)
      pUserLog->KeyDown(iButton, iType, 5);
    m_iTarget = 1 - m_iTarget;
    m_bDecorationChanged = true;
  }
  else {
    if(pUserLog)
      pUserLog->KeyDown(iButton, iType, 0);
  }
}


bool COneButtonDynamicFilter::GetSettings(SModuleSettings **pSettings, int *iCount) {
  *pSettings = sSettings;
  *iCount = sizeof(sSettings) / sizeof(SModuleSettings);

  return true;
};
