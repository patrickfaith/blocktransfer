class BTmessageBlock : public BTblock   // a output stuff into the modem
{
  public:
  BTbuff     *buffer;
  WPARAM      wParam;
  LPARAM      lParam;
  UINT      iMessage;
  BTmessageBlock():BTblock()
    {buffer=NULL;wParam=0;lParam=0;iMessage;};
  virtual long Type() { return BT_TYPE_MESSAGE; } ; 
} ;
