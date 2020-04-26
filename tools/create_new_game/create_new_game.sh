#!/bin/bash

TOPDIR="../.."

if [ $2 ]
then
    TOPDIR=$2
fi

GAMESDIR="${TOPDIR}/games"
GAMENAME="$1"
TEMPLATE_LOC="${TOPDIR}/tools/create_new_game"

mkdir -p "${GAMESDIR}/${GAMENAME}/gamesprites"
cp ${TEMPLATE_LOC}/template_module.mk "${GAMESDIR}/${GAMENAME}/${GAMENAME}_module.mk"
cp ${TEMPLATE_LOC}/template_main.cpp "${GAMESDIR}/${GAMENAME}/${GAMENAME}_main.cpp"

echo "CPPDIRS += \$(GAMESDIR)/${GAMENAME}" >> "${GAMESDIR}/${GAMENAME}/${GAMENAME}_module.mk"
echo "USERINCDIR += \$(GAMESDIR)/${GAMENAME}" >> "${GAMESDIR}/${GAMENAME}/${GAMENAME}_module.mk"

