SUMMARY = "Applications & Libraries used to make a Talking Clock"
HOMEPAGE = "https://github.com/micaah42"

LICENSE = "CLOSED"

inherit cmake_qt5

IMAGE_FEATURES += " allow-root-login"

DEPENDS += "\
    qtmultimedia \
    qtwebsockets \
    qtquickcontrols2 \
"

SRC_URI = "git://git@github.com/micaah42/talking-clock.git;protocol=https;branch=main"
SRCREV = "${AUTOREV}"

S = "${WORKDIR}/git"

# angular application

do_compile:append() {
    # compile and install node modules in source directory
    cd ${WORKDIR}/git/web
    npm install
    npm run build
}

do_install:append() {
    install -d ${D}/${datadir}/clockd/www
    install -m 0644 "${WORKDIR}/git/web/dist/*" "${D}/${datadir}/clockd/www"
}

