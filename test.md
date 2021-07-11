# MarkDown测试

[TOC]


### 1、标题

```
标题
# Title1
## Title2
### Title3
#### Title4
##### Title5
###### Title6
```

# Title1
## Title2
### Title3
#### Title4
##### Title5
###### Title6

### 2、分割线
```
******
------
```

******
------

### 3、列表

有序列表


1. list1
2. list2
   1. list3
   2. list4
   1. list5
3. list6


无序列表


* list1
+ list2
  - list3
  * list4
  + list5
- list6

### 4、修饰

> 使用*倾斜

这是*倾斜*。
这是 _倾斜_。

> 使用**加粗

这是**加粗**。
这是 __加粗__。

> 使用～～来删除

这是~~删除~~。
这是<u>下划</u>
这是***粗体加斜体***
这是 ~~<u>删除加下划</u>~~

> 使用 `来添加行内代码

这是```printf```。

### 5、图片和链接
#### 导入图片


![baidu](baidu.gif)


#### 导入链接

[百度一下](http://www.baidu.com)


#### 导入角标

这是程序设计2大作业[1]


### 6、引用、代码

使用">"进行引用
```
>  一级引用
>> 二级引用
>> 二级引用
>>> 三级引用
> 一级引用
```
>  一级引用
>> 二级引用
>> 二级引用
>>> 三级引用
> 一级引用


#### 行间代码
``` C
#include<iostream>
using namespace std;
int main(){
    cout<<"Hello,World"<<endl;
}
```

### 7、表格

#### 例如如下表格
```
|姓名|学号|班级|绩点|
|:-—|:--:|:-:|--:|
|张舒恒|000000|一班|？？？|
|甘文迪|000001|一班|？？？|
```

|姓名|学号|班级|GPA|
|:---|:--:|:--:|--:|
|张舒恒|000000|一班|？？？|
|甘文迪|000001|一班|？？？|





### 8、TOC

目录（见上面）

### 10、流程图

以[flow]开始流程图，以[!flow] 结束

> [flow]
> st=>start: 开始
> e=>end: 结束
> op=>operation: GPA
> cond=>condition: ==4.3?
> st->op->cond
> cond(yes)->op
> cond(no)->e
> [!flow]


[flow]

st=>start: 开始
e=>end: 结束
op=>operation: GPA
cond=>condition: ==4.3?
st->op->cond
cond(yes)->op
cond(no)->e

[!flow]


end of file