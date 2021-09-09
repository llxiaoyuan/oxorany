> 我们拥有如下代码，对其中的一段代码开启了`VMProtect`中的`全部`功能

```C++
#include <stdio.h>
#include <Windows.h>
#include "VMProtectSDK.h"
int main() {
	int a = 0, b = 0, c = 0;
	while (true) {
		VMProtectBegin("VMProtectBegin");
		a += 10;
		b += 100;
		c = a + b;
		VMProtectEnd();
		printf("%d %d %d\n", a, b, c);
		Sleep(1000);
	}
	return 0;
}
```

> 关闭随机基址

![image](https://user-images.githubusercontent.com/36320938/132643253-6ea66c4b-398c-4a7c-a883-5c7e5999eb27.png)

> 编译后的代码如下

![image](https://user-images.githubusercontent.com/36320938/132649596-96600ecc-fb86-4310-99e8-6863e88ebd8f.png)

> 使用`VMProtect`的默认选项对`exe`进行`编译`

![image](https://user-images.githubusercontent.com/36320938/132635129-19e57e60-98b9-4125-b807-12eec174fdcf.png)



