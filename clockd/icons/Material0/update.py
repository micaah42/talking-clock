import requests


qml_keywords = ['date', 'component', 'enum', 'class', 'delete', 'function', 'package', 'public', 'switch', 'try', 'print']
font = 'MaterialSymbolsOutlined'
codepoints = requests.get(f'https://github.com/google/material-design-icons/raw/master/variablefont/{font}%5BFILL%2CGRAD%2Copsz%2Cwght%5D.codepoints')
ttf = requests.get(f'https://github.com/google/material-design-icons/raw/master/variablefont/{font}%5BFILL%2CGRAD%2Copsz%2Cwght%5D.ttf')

with open(f'./{font}[FILL,GRAD,opsz,wght].ttf', 'wb') as f:
    f.write(ttf.content)


with open(f'./{font}.properties.txt', 'w') as f:
    points = codepoints.content.decode().splitlines()
    for point in points:
        name, value = point.strip().split(' ')
        name = f'_{name.upper()}' if not name[0].isalpha() or name in qml_keywords else name
        value = int(value, 16)
        line = f'readonly property string {name}: \'\\u{value:04x}\''
        f.write(line + '\n')
        print(line)