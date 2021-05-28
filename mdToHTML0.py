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

def quoteReplace(all):
    def replaceResult(segment):
        ans = re.sub(r'(^|\n)\s*?> (.*)', r'\n<p>\2</p>', segment.group())
        return '\n<blockquote>'+ans+'</blockquote>'
    ans = re.sub(r'((^|\n)\s*?> .*)+', replaceResult, all)
    return ans


def listReplace(all):
    def replaceResult(segment):
        ans = re.sub(r'(^|\n)\s*?(\+|-) (.*)', r'\n<li>\3</li>', segment.group())
        return '\n<ul>'+ans+'</ul>'
    ans = re.sub(r'((^|\n)\s*?(\+|-) .*)+', replaceResult, all)
    return ans


def codeReplace(all):
    ans = re.sub(r'\s+```(.*?)\n(.*?)```\n',
                 r'\n<pre><code lang="\1">\2</code></pre>\n', all, flags=re.DOTALL)
    return ans


def segmentFliter(all):
    replaced = quoteReplace(listReplace(codeReplace(all)))
    # ans = re.split(r'\n+(?!([^<]*((?!<pre>|<ul>|<blockquote>)<\w+>|(?!<\/pre>|<\/ul>|<\/blockquote>)<\/\w+>))*[^<]*(<\/pre>|<\/ul>|<\/blockquote>))', replaced)
    ans = re.findall(r'<(?:pre|ul|blockquote)>[\s\S]*?<\/(?:pre|ul|blockquote)>|\n|.+', replaced)
    result = ['' if i == '\n' else i for i in ans]
    return result

def wordFilter(line):
    def boldReplace(line):
        return re.sub(r'\*\*(.+?)\*\*', r'<b>\1</b>', line)

    def urlReplace(line):
        return re.sub(r'\[(.*?)\]\((.+?)\)', r'<a href="\2">\1</a>', line)

    return boldReplace(urlReplace(line))


def lineFilter(line):
    head = headReplace(line)
    if head:
        return head
    return pAppend(line)


def main():
    inputFile = open('test.md', 'r', encoding='utf-8')
    raw = inputFile.read()

    ans = []
    for segment in segmentFliter(raw):
        if segment.startswith('<pre>') or segment=='':
            ans.append(segment)
        elif segment.startswith('<ul>') or segment.startswith('<blockquote>'):
            ans.append(wordFilter(segment))
        else:
            ans.append(lineFilter(wordFilter(segment)))

    out = htmlAppend(bodyAppend('\n'.join(ans)))
    outputFile = open('test.html', 'w', encoding='utf-8')
    outputFile.write(out)


main()
