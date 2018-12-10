# IDAngr-PinTool

PinTool skeleton for state synchronization with IDAngr

> tested with pin-3.7-97619-g0d0c92f4f-msvc-windows on Windows 10 x86_64 with SDK 10.0.17763.0

Change the path of your Windows.h in this line inside WinHeaderStub.h:

```cpp
// LOOK AT ME : change 10.0.17763.0 with your SDK version
#include <C:/Program Files (x86)/Windows Kits/10/Include/10.0.17763.0/um/Windows.h>
```