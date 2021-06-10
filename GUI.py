import tkinter
import tkinter.messagebox
from tkinter.filedialog import askopenfilename
import subprocess
import re


def main():
    inputFileName = outputFileName = ''

    def convert():
        nonlocal inputFileName, outputFileName
        if outputFileName == '':
            outputFileName = re.sub(
                r'(.+)\..*', r'\1', inputFileName) + '.html'
        # command = 'mdToHTML -o "' + outputFileName + '" "' + inputFileName + '"'
        command = 'dir'

        # ret = subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        subprocess.Popen(command, shell=True)

        inputFileName = outputFileName = ''

        label.config(text=inputFileName)

    def selectInputFile():
        nonlocal inputFileName
        inputFileName = askopenfilename()

        label.config(text=inputFileName)

    # 创建顶层窗口
    top = tkinter.Tk()
    # 设置窗口大小
    top.geometry('480x320')
    # 设置窗口标题
    top.title('mdToHTML')
    # 创建标签对象并添加到顶层窗口
    label = tkinter.Label(top, text=inputFileName, width=30, height=3, wraplength=320,
                          font='Arial -32', fg='red')
    label.pack(expand=1)
    # 创建一个装按钮的容器
    panel = tkinter.Frame(top)
    # 创建按钮对象 指定添加到哪个容器中 通过command参数绑定事件回调函数
    button1 = tkinter.Button(panel, text='选择', command=selectInputFile)
    button1.pack(side='left')
    button2 = tkinter.Button(panel, text='转换', command=convert)
    button2.pack(side='right')
    panel.pack(side='bottom', pady=20)
    # 开启主事件循环
    tkinter.mainloop()


if __name__ == '__main__':
    main()
