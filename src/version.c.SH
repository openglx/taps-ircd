#!/bin/sh

# $Id: version.c.SH 34 2010-07-31 18:35:02Z openglx $ 

spitshell=cat
package=IRC

echo "Extracting $package/src/version.c..."

if test -r version.c.last
then
   generation=`sed -n 's/^char \*generation = \"\(.*\)\";/\1/p' < version.c.last`
   if test ! "$generation" ; then generation=0; fi
else
   generation=0
fi

generation=`expr $generation + 1`

uname=`uname -a`
creation=`date | \
awk '{if (NF == 6) \
         { print $1 " "  $2 " " $3 " "  $6 " at " $4 " " $5 } \
else \
         { print $1 " "  $2 " " $3 " " $7 " at " $4 " " $5 " " $6 }}'`

$spitshell >version.c <<!SUB!THIS!
/*
 *   IRC - Internet Relay Chat, src/version.c
 *   Copyright (C) 1990 Chelsea Ashley Dyerman
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 1, or (at your option)
 *   any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * This file is generated by version.c.SH. Any changes made will go away.
 */

#include "struct.h"
#include "patchlevel.h"
#include "serno.h"

char *generation = "$generation";
char *creation = "$creation";
char *ircdversion = PATCHLEVEL;
char *serno = SERIALNUM;
char *platform = "$uname";

char *infotext[] =
{
  "$package --",
  "Based on the original code written by Jarkko Oikarinen",
  "Copyright 1988, 1989, 1990, 1991 University of Oulu, Computing Center",
  "",
  "This program is free software; you can redistribute it and/or",
  "modify it under the terms of the GNU General Public License as",
  "published by the Free Software Foundation; either version 1, or",
  "(at your option) any later version.",
  "",
  "TAPS Version:",  
  "^Stinger^           Jacques Weewer      stinger@ircworld.nl",
  "openglx             Felipe              openglx@StarByte.net",
  "",
  "TAPS - This Ain't PTlink Software - is based on PTlink Software.",
  "Former Project Manager for PTlink Software, our beloved:",
  "Lamego              Joao Pinto          Lamego@PTlink.net",
  "",
  "The following people have contributed to this release of PTlink",
  "in nick alphabetical order:",  
  "fabulous@brasnet",
  "HuMPie",  
  "Mauritz@brasnet",
  "",  
  "The core Hybrid Team as of this major release:",
  "",
  "cosine              Patrick Alken       wnder@uwns.underworld.net",
  "Dianora             Diane Bruce         db@db.net",
  "is-                 Edward Brocklesby   ejb@leguin.org.uk",
  "Neph|l|m            Adel Mezibra        adel@cybercable.fr",
  "pro                 Bill Jonus          proggy@earthling.net",
  "Rodder              Jon Lusky           lusky@blown.net",
  "Wohali              Joan Touzet         jolo@cadence.com",
  "",
  "The following people have contributed to this release of hybrid",
  "in nick alphabetical order:",
  "",
  "Bleep               Thomas Helvey       tomh@inxpress.net",
  "CBongo              Chris Bongaarts     cab@tc.umn.edu",
  "comstud             Chris Behrens       cbehrens@concentric.net",
  "echo                Adam J. Durana      adam@caca.com",
  "FlashMan            Craig A. Huegen     chuegen@quadrunner.com",
  "jailbird            Dustin Marquess     jailbird@alcatraz.fdf.net",
  "johan               Johannes Erdfelt    jerdfelt@eng.mindspring.net",
  "LarZ                Matthew Ramsey      mjr@blackened.com",
  "orabidoo            Roger Espel Llima   espel@unix.bigots.org",
  "Seaner              Sean McVeigh        sdlmcvei@uwaterloo.ca",
  "Shadowfax           Michael Pearce      mpearce@varner.com",
  "Slowaris            Ryan Parker         rjp@wpmedia.com",
  "ThemBones           Brian Kraemer       kraemer@u.washington.edu",
  "White_Dragon        Chip Norkus         wd@dal.net",
  "WintrHawk           Jake Khuon          khuon@GCtr.Net",
  "",
  "",
  "Developers and Contributors for previous versions:",
  "",
  "Chris Portman       Keith Fralick       Jon Mischo",
  "Sam Noble           Taner Halicioglu    Darren Reed",
  "Markku Savela       Greg Lindahl        Jarkko Oikarinen",
  "Armin Gruner        Matthew Green       Chuck Kane",
  "Matt Lyle           Vesa Ruokonen       Nicolas PIOCH",
  "Stellan Klebom      Dan Goodwin         Mike Bolotski",
  "Ian Frechette       Markku Jarvinen     Kimmo Suominen",
  "Jeff Trim           Vijay Subramaniam   Karl Kleinpaste",
  "Bill Wisner         Tom Davis           Hugo Calendar",
  "Tom Hopkins         Stephen van den Berg",
  "Bo Adler            Michael Sandrof     Jon Solomon",
  "Jan Peterson        Helen Rose          Paul Graham",  
  "",
  "Thanks also goes to those persons not mentioned here who have added",
  "their advice, opinions, and code to IRC.",
  "Thanks also to those who provide the kind sys admins who let me and",
  "others continue to develop IRC.",
  "",
  0,
};
!SUB!THIS!
