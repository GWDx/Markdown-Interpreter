import tkinter
from tkinter.messagebox import showerror
from tkinter.filedialog import askopenfilename
from tkinter import Label, Button, Entry, Checkbutton, StringVar, IntVar

import re
import json
import subprocess

from uploadFTP import uploadFTP


def main():
    inputFileName = ''
    outputFileName = ''

    def selectInput():  # 选择文件
        nonlocal inputFileName, outputFileName
        inputFileName = askopenfilename()
        outputFileName = re.sub(r'(.+)\.(?!html).*', r'\1', inputFileName) + '.html'
        inputLabel.config(text=inputFileName)

    def convert():      # 转换
        nonlocal inputFileName, outputFileName
        command = 'mdToHTML.exe -o "' + outputFileName + '" "' + inputFileName + '"'
        # ret = subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        # if ret.returncode != 0:
        #     showerror(title='错误', message='错误')
        subprocess.Popen(command, shell=True)   # 创建新进程，执行转换程序
        inputFileName = ''
        inputLabel.config(text=inputFileName)

    def upload():       # 上传
        nonlocal save, userName, password

        def changeSave():   # 保存
            nonlocal save
            save = 1 - save

        def submit():       # 提交
            nonlocal userName, password
            userName = userNameEntry.get()
            password = passwordEntry.get()
            if save == 1:       # 保存用户名及密码
                writeSaveFile = open('config.json', 'w')
                writeContent = json.dumps({'userName': userName, 'password': password})
                writeSaveFile.write(writeContent)
                writeSaveFile.close()

            try:                # 尝试上传
                uploadFTP(userName, password, outputFileName)
            except:
                showerror(title='错误', message='错误')
            second.destroy()

        second = tkinter.Toplevel()
        second.geometry('400x250')
        second.title('登录 FTP')

        userNameVar = StringVar(value=userName)
        passwordVar = StringVar(value=password)
        saveVar = IntVar(value=save)

        userNameLabel = Label(second, text='用户名')
        passwordLabel = Label(second, text='密码')
        userNameEntry = Entry(second, textvariable=userNameVar, width=30)
        passwordEntry = Entry(second, textvariable=passwordVar, width=30, show='*')
        saveCheckbutton = Checkbutton(second, text='保存', variable=saveVar, command=changeSave)
        submitButton = Button(second, text='提交', command=submit)

        userNameLabel.place(x=40, y=60)
        passwordLabel.place(x=40, y=100)
        userNameEntry.place(x=130, y=60)
        passwordEntry.place(x=130, y=100)
        saveCheckbutton.place(x=100, y=140)
        submitButton.place(x=200, y=140)

    try:    # 读取用户名及密码
        readSaveFile = open('config.json', 'r')
        readContent = readSaveFile.read()
        readSaveFile.close()
        readData = json.loads(readContent)
        save = 1    # ?
        userName = readData['userName']
        password = readData['password']
    except:
        save = 0
        userName = ''
        password = ''

    first = tkinter.Tk()
    first.geometry('480x320')
    first.title('mdToHTMLGUI')

    panel = tkinter.Frame(first)
    selectInputButton = Button(panel, text='选择文件', command=selectInput, font='宋体 -20')
    convertButton = Button(panel, text='转换', command=convert, font='宋体 -20')
    uploadButton = Button(panel, text='上传', command=upload, font='宋体 -20')
    inputLabel = Label(first, text=inputFileName, width=30, height=3, wraplength=320, font='Arial -20', fg='red')

    selectInputButton.pack(side='left', padx=20)
    convertButton.pack(side='left', padx=20)
    uploadButton.pack(side='left', padx=20)
    panel.pack(side='top', pady=40)
    inputLabel.pack(side='top', pady=40)

    tkinter.mainloop()


if __name__ == '__main__':
    main()
