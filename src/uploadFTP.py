import ftplib
import re


def uploadFTP(userName, password, localFileName):
    host = 'home.ustc.edu.cn'
    ftp = ftplib.FTP(host, encoding='gbk')
    ftp.login(userName, password)

    # ans = ftp.retrlines('LIST')
    # print(ans)    # ? encoding
    ftp.cwd('public_html')
    remoteFileName = re.sub(r'.+[\\\/](.+)', r'\1', localFileName)
    localFile = open(localFileName, 'rb')
    ftp.storbinary('STOR ' + remoteFileName, localFile)
    localFile.close()
    ftp.quit()
