> 我们拥有如下代码，对其中的一段代码开启了`VMProtect`中的`虚拟化`功能

```C++
#include <stdio.h>
#include <Windows.h>
#include "VMProtectSDK.h"
int main() {
	int a = 0, b = 0, c = 0;
	while (true) {
		VMProtectBeginVirtualization("VMProtectBeginVirtualization");
		a += 10;
		b += 100;
		c = a + b;
		printf("%d %d %d\n", a, b, c);
		VMProtectEnd();
		Sleep(1000);
	}
	return 0;
}
```

> 编译后的代码如下

![image](https://user-images.githubusercontent.com/36320938/132635534-533e2510-e01b-4fbc-97b4-e5f753996378.png)

> 使用`VMProtect`的默认选项对`exe`进行`编译`

![image](https://user-images.githubusercontent.com/36320938/132635129-19e57e60-98b9-4125-b807-12eec174fdcf.png)



