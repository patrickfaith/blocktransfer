#include <btdefs.h>
#include <btlist.h>
#include <btblocks.h>
 
BTblock * BTblock::TriggerBlock(short triggerType)
{
  if ( triggerType == BT_TRIG_KEY_RETURN )
  {
    return NULL ; // BTex.theBook->trigKeyReturn;
  }
  else if (triggerType == BT_TRIG_COM_RETURN )
  {
    return NULL ; // BTex.theBook->trigComReturn;
  }
  else if( triggerType == BT_TRIG_BUT_FOCUS )
  {
    return BTex.theBook->trigButFocus;
  }
  else if( triggerType == BT_TRIG_BUT_EXECUTE )
  {
    return BTex.theBook->trigButExecute;
  }
  else
  {
    return NULL ;
  }
}

BTblock * BTviewBlock::TriggerBlock(short triggerType)
{
  if ( triggerType == BT_TRIG_KEY_RETURN )
  {
    if (trigKeyReturn == NULL)
      return NULL ; // BTex.theBook->trigKeyReturn;
    else
      return trigKeyReturn;
  }
  else if (triggerType == BT_TRIG_COM_RETURN )
  {
    if (trigComReturn == NULL )
      return NULL ; // BTex.theBook->trigComReturn;
    else
      return trigComReturn;
  }
  else if( triggerType == BT_TRIG_BUT_FOCUS )
  {
    if ( trigButFocus == NULL )
      return BTex.theBook->trigButFocus;
    else
      return trigButFocus ;
  }
  else if( triggerType == BT_TRIG_BUT_EXECUTE )
  {
    if ( trigButExecute == NULL )
      return BTex.theBook->trigButExecute;
    else
      return trigButExecute;
  }
  else
    return NULL ;
}