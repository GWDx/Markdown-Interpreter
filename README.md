## Markdown-Interpreter

将 Markdown 转换为 HTML。



### 使用方式

#### 命令行界面

命令行参数：`mdToHTML.exe [options] file`

| 选项        | 含义           |
| ----------- | -------------- |
| `-h`        | 查看帮助信息   |
| `-o [file]` | 指定输出文件名 |
| `-p`        | 在终端输出     |



### 编译

> + g++, make
> + python 3, pyinstaller

#### 方案一

```bash
git clone https://github.com/GWDx/Markdown-Interpreter.git
cd Markdown-Interpreter
mkdir bin
make
```

#### 方案二

```bash
git clone https://github.com/GWDx/Markdown-Interpreter.git
cd Markdown-Interpreter
mkdir bin
cd src
make
```

