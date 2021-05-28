import re


def headReplace(line):
    heads = re.match(r'^(#{1,6}) (.*)',line)
    if not heads:
        return None
    length = str(len(heads.group(1)))
    content = heads.group(2)
    return '<h'+length+' id="' + content + '">' + content + '</h'+length+'>'


def bodyAppend(ans):
    return '<body>\n' + ans + '</body>\n'


def htmlAppend(ans):
    return '<!doctype html>\n<html>\n' + ans + '</html>\n'


def pAppend(line):
    return '<p>' + line + '</p>'


def codeSplit(all):
    ans = []
    for segment in all:
        parts = segment.split('```')
        for index, elem in enumerate(parts):
            if index % 2 == 1:
                ans.append(re.sub(
                    r'(.*?)\n(.*)', r'<pre><code lang="\1">\2</code></pre>', elem, flags=re.DOTALL))
            else:
                ans.append(elem)
    return ans


def segmentFliter(all):
    return codeSplit([all])


def wordFilter(line):
    def boldReplace(line):
        return re.sub(r'\*\*(.+?)\*\*', r'<b>\1</b>', line)

    def urlReplace(line):
        return re.sub(r'\[(.*?)\]\((.+?)\)', r'<a href="\2">\1</a>', line)

    return boldReplace(urlReplace(line))


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

    ans = []
    for segment in segmentFliter(raw):
        if segment.startswith('<pre'):
            ans.append(segment)
        else:
            for line in segment.split('\n'):
                if line != '':
                    ans.append(lineFilter(wordFilter(line)))
                else:
                    ans.append('')

    out = htmlAppend(bodyAppend('\n'.join(ans)))
    outputFile = open('test.html', 'w', encoding='utf-8')
    outputFile.write(out)


main()
# print(segmentFliter(
#     '''
# A
# ```
# 1+2
# ```
# B
# '''
# ))
