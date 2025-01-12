import glob
from clang.cindex import Index, Cursor, CursorKind
import clang
import sys

NETWORKMANAGER_QT_PATH = "/home/michael/contribute/networkmanager-qt"


def find_classes(node: Cursor) -> list[Cursor]:
    """return all classes contained in (or possibly including) node"""

    classes = []
    kind: CursorKind = node.kind

    if (
        kind.is_reference()
        and node.spelling.startswith("class")
        and not node.spelling.endswith("Private")
        and node.spelling != "class NetworkManager::NETWORKMANAGERQT_EXPORT"
    ):
        # if node.spelling.startswith("class NetworkManager::AccessPoint"):

        print(f"{node.spelling} [line={ node.location.line}]")
        print(vars(node))
        classes.append(node)

    for child in node.get_children():
        classes.extend(find_classes(child))

    return classes


header_files = glob.glob(f"{NETWORKMANAGER_QT_PATH}/src/**/*.cpp", recursive=True)

# header_files = ['/home/michael/talking-clock/qnetworkmanager/src/wireless.cpp']
for file in header_files:
    index = clang.cindex.Index.create()
    tu = index.parse(file, args=[f"-I {NETWORKMANAGER_QT_PATH}/src"])
    print("Translation unit:", tu.spelling)
    classes = find_classes(tu.cursor)

# print(header_files)
