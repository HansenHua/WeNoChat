#include "ui_windows.h"
#include "login.h"
#include "netset.h"

Login *mUi::login = NULL;
NetSet *mUi::netset = NULL;

void load_windows()
{
    mUi::login = new Login();
    mUi::netset = new NetSet();
}
