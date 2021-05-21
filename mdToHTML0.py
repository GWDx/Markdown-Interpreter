import re


def headReplace(line):
    heads = re.search(r'^#{1,6}', line)
    if not heads:
        return None
    number = len(heads.group())
    content = line[number+1:]
    return '<h'+str(number)+' id="' + content + '">' + content + '</h'+str(number)+'>'


def bold(line):
    content = re.sub(r'\*\*(.+?)\*\*', r'<b>\1</b>', line)
    return content


def bodyAppend(ans):
    return '<body>\n' + ans + '</body>\n'


def htmlAppend(ans):
    return '<!doctype html>\n<html>\n' + ans + '</html>\n'


def pAppend(line):
    return '<p>' + line + '</p>'


def wordFilter(line):
    return bold(line)


def lineFilter(line):
    if line.strip(' ') == '':
        return ''
    head = headReplace(line)
    if head:
        return head
    return pAppend(line)


def main():
    inputFile = open('test.md', 'r', encoding='utf-8')
    raw = inputFile.read()

    lines = raw.split('\n')
    ans = []
    for line in lines:
        ans.append(lineFilter(wordFilter(line)))

    out = htmlAppend(bodyAppend('\n'.join(ans)))
    outputFile = open('test.html', 'w', encoding='utf-8')
    outputFile.write(out)


main()
# print(replace('12**34**56'))
