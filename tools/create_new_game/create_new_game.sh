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
cp ${TEMPLATE_LOC}/template_module.mk "${GAMESDIR}/${GAMENAME}/${GAMENAME}.mk"

