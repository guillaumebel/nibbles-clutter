��    o      �  �         `	     a	     v	  �   {	  �  $
  '     I   /  I   y  I   �  B     B   P  I   �  I   �  I   '  G   q  S   �  H     H   V  F   �  }  �     d     ~     �     �  I  �     �  �     /  �     �     �  7   �  �       �     �  �   �     �     �     �     �     �  U   �  �  @  �   �  n  �     F!  �  ]!     #     #      %#  $   F#     k#  7  s#  N   �$     �$     %  �   %    �%  �  �&    [(     a)  �  r)  �   +  !  �+     �,  =   �,  %   ,-  �   R-  i  !.  @  �/     �0  
   �0     �0     �0  8   �0  �   .1  <  2    ?3     C4  �   [4    )5     +6  <   C6     �6  @   �6     �6  �   �6     �7  1   �7    �7    �9  �   ;  �   �;  Y  I<  ,   �=  �   �=  /  �>  j  �?  �   iA  �   "B  _  �B     0D  �  5D  �   �E  �  �F     I     �I     �I  �   �I  �  �J     NM     gM  �  zM     O     O  �   O  �  �O  .   �Q  I   �Q  I   +R  I   uR  B   �R  B   S  I   ES  I   �S  I   �S  G   #T  S   kT  H   �T  H   U  F   QU    �U     X     5X     =X     PX  k  jX     �[  �   �[  O  �\     �]     �]  7   ^  �   9^  \  �^     Tb  �   cb     c     8c     Dc     dc     xc  W   �c  �  �c  ,  �e  �  �f     jh  �  �h     j     j  7   !j  :   Yj  	   �j  b  �j  U   l     Wl  )   `l  �   �l    m    n  �   �o     �p  �  �p  �   qr  5  s     <t  ;   Dt  :   �t  �   �t  \  �u    w  
   �x     �x     �x     �x  B   �x  �   �x  F  �y  #  �z  (   |  �   H|  3  }      M~  G   n~     �~  9   �~       �        �  :   �  )  �    =�  �   K�  �   ݃  Z  u�  =   Ѕ    �  M  �  *  k�  �   ��  �   U�  j  �     _�  �  h�  �   �  @  �      &�     G�     X�    h�  X  ~�     ז  ?   �     .   X          H   W   >   O   8   (   l       n   $      _       a           "   %   A   K   V          -              3   5      =   1   \              o      i   b      ^   E   `   :          &   e   C   ]              M   
                2              6      L      P   f             S   h            /           <   B   m   k   @   Z   4      #       D   Y      G      Q       9   '              ,   [   J       T   U   )   0   g               !   d   R       N   ?       	           +      I   j                          c   7   F   ;           *           "I'm not sure" Flags 2006 <application><application>Mines</application></application> is a puzzle game where you locate mines floating in an ocean using only your brain and a little bit of luck. <application>GNOME Mines</application> was written by Pista (<email>szekeres@cyberspace.mht.bme.hu</email>). This manual was written by Callum McKenzie (<email>callum@spooky-possum.org</email>) with help from Vincent Povirk and based on an earlier manual by Tim Riehle (<email>tkriehle@citilink.com</email>). To report a bug or make a suggestion regarding this application or this manual, follow the directions in this <ulink url="ghelp:gnome-feedback" type="help">document</ulink>. <application>Mines</application> Manual @@image: 'figures/easy-hints-1.png'; md5=4ce524b08d7be427097a63c3cdfec6ee @@image: 'figures/easy-hints-2.png'; md5=e37f8d409337ff13d85f36208fd04008 @@image: 'figures/easy-hints-3.png'; md5=d444acbdd254318948b8f07c9b390ba6 @@image: 'figures/faces.png'; md5=c4ef2dc5a8fee43f2e00b49d2a3151eb @@image: 'figures/flags.png'; md5=21bb5789d11d2ea40515a4dfcfd597ca @@image: 'figures/hard-hints-1.png'; md5=38f10c8159ec5526f1bd58ff322b7142 @@image: 'figures/hard-hints-2.png'; md5=d5eecaed4e281d9d8db67f039627aa59 @@image: 'figures/hard-hints-3.png'; md5=12f7ceaa8d7187cb694927ac015c10a8 @@image: 'figures/highscores.png'; md5=a62a94c773d992dcf399ee09ec35da5a @@image: 'figures/imnotsureflagscheckbox.png'; md5=58be021ed1cf90add08931282ec19bad @@image: 'figures/main-window.png'; md5=0f49e2af3381111f4d13575fefd52d87 @@image: 'figures/preferences.png'; md5=b0a57c641f5d123f509e71c632294a12 @@image: 'figures/statusbar.png'; md5=8fd07589a7caac8c0c7c42007adb6835 A copy of the <citetitle><ulink type="help" url="gnome-help:gpl">GNU General Public License</ulink></citetitle> is included as an appendix to the <citetitle>GNOME Users Guide</citetitle>. You may also obtain a copy of the <citetitle><ulink type="help" url="gnome-help:gpl">GNU General Public License</ulink></citetitle> from the Free Software Foundation by visiting <ulink type="http" url="http://www.fsf.org">their Web site</ulink> or by writing to <address> Free Software Foundation, Inc. <street>59 Temple Place</street> - Suite 330 <city>Boston</city>, <state>MA</state><postcode>02111-1307</postcode><country>USA</country></address> A not-so-obvious pattern. Action Advanced Situations As tricky as it gets. Assume that, in the figure below, we start off knowing that the top left square is mined and so the two squares marked 2 on the left only require one more mine. Next observe that the left-most 2 implies that one of the covered squares beneath it is mined, but we don't yet know which one. Since at least one of these squares is mined and they are both neighbors of the second 2 then we know that that 2 is also satisfied by one of these two squares (we still don't know which one). This means that the square beneath the 4 is clear because it is also a neighbor of the second 2 and not one of the two squares we identified previously. This one blank square now leaves the 4 with only four neighbors - all of which must be mines. Not only does it tell us which mines surround the 4, it now tells us which of the squares beside the 2 is mined! Authors Auto-clear. Reveals the unmarked squares around the clicked square, but only if enough squares have been marked to match the number in the square. By choosing <menuchoice><guimenu>Settings</guimenu><guimenuitem>Preferences</guimenuitem></menuchoice> from the menu you can change the size of the games. Larger sizes obviously take longer and require more concentration. Smaller games are more frantic. We provide Small, Medium, Large and Custom sizes. Callum Callum McKenzie Callum McKenzie <email>callum@spooky-possum.org</email> DOCUMENT AND MODIFIED VERSIONS OF THE DOCUMENT ARE PROVIDED UNDER THE TERMS OF THE GNU FREE DOCUMENTATION LICENSE WITH THE FURTHER UNDERSTANDING THAT: <placeholder-1/> DOCUMENT IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE DOCUMENT OR MODIFIED VERSION OF THE DOCUMENT IS FREE OF DEFECTS MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY, ACCURACY, AND PERFORMANCE OF THE DOCUMENT OR MODIFIED VERSION OF THE DOCUMENT IS WITH YOU. SHOULD ANY DOCUMENT OR MODIFIED VERSION PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL WRITER, AUTHOR OR ANY CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. THIS DISCLAIMER OF WARRANTY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY DOCUMENT OR MODIFIED VERSION OF THE DOCUMENT IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER; AND Feedback Finally, when there are three uncovered squares in a row and the clear square beside the middle one is a 3 (once again this square should have no other neighbors) then all three are mines. GNOME Documentation Project GNOME Mines Game Size and Difficulty High Scores Hints If you are used to the more traditional mouse controls, don't worry, they still work. If you get stuck - it is reasonably common to end up with two options that you can't logically resolve - then you can either guess, or ask for a hint. When you choose <guimenuitem>Hint</guimenuitem> from the <guimenu>Game</guimenu> menu a single square is revealed (plus surrounding squares if it is blank). The choice of square is random, but should be helpful. There is 10 second penalty added to your time whenever you use a hint. If you left click on a mine, or auto-clear a square with a mine in it you lose. To help prevent accidents, you aren't allowed to auto-clear around a square unless the number of marked mines equals the number given in the square. If your score is in the top ten you will be shown the list of top scores and have an opportunity to change the name recorded in the high score table. You can also review the scores later by choosing <guimenuitem>Scores</guimenuitem> from the <guimenu>Game</guimenu> menu. The drop-down menu at the top of the dialog lets you look at the scores for other board sizes. Important information. In the example below the mines are to the left of the ones. We figure this out by a process of elimination. If the square to the left of the 2 was a mine then the 1s would both have their maximum number of mines leaving no free square for the second mine adjacent to the 2 (since they would also be adjacent to the 1s). Having eliminated the middle square we are left with two squares we require for the 2 so they are both mined. Introduction January 2006 Left click in a revealed square. Left click on an un-revealed square. License Many of the names used by companies to distinguish their products and services are claimed as trademarks. Where those names appear in any GNOME documentation, and the members of the GNOME Documentation Project are made aware of those trademarks, then the names are in capital letters or initial capital letters. Marks the square as a mine. If it is already marked, the mark will be removed. McKenzie Mines Manual V2.9 Mines is GNOME's take on the popular logic puzzle minesweeper, which includes avoiding mines while receiving clues for the location of the mines. Now we're going to try a complicated example. It illustrates two points. The first is that even if you aren't sure which squares are mined, restricting the possibilities is helpful. The second is that knowing which squares aren't mined is nearly as good as knowing which are mined. On the right hand side of the dialog there are three parameters for controlling the custom size. Horizontal and Vertical are the width and height of the desired grid in squares. The number of mines is - obviously - the number of mines you need to find. If this number is set too high or too low then the game becomes very easy. The number of mines is limited to slightly less than the total number of squares. One final hint: remember the counter on the bottom left of the window that tells you how many mines you have left? Often, near the end of the game, this number can help you figure out what possible places mines can be. How to do this is up to you to figure out. Pausing the Game Permission is granted to copy, distribute and/or modify this document under the terms of the GNU Free Documentation License (GFDL), Version 1.1 or any later version published by the Free Software Foundation with no Invariant Sections, no Front-Cover Texts, and no Back-Cover Texts. You can find a copy of the GFDL at this <ulink type="help" url="ghelp:fdl">link</ulink> or in the file COPYING-DOCS distributed with this manual. Remember, the edge of the board is just the same as a large cleared area (but without the numbers). Often working at the edge of the board is the easiest way forward. Resizing the window makes the squares bigger or smaller to suit. If you want to make maximum use of the screen then there is a <guimenuitem>Fullscreen</guimenuitem> option under the <guimenu>Settings</guimenu> menu. You can also use the <keycap>F11</keycap> key to toggle full-screen mode. Result Reveals the contents of that square. For better or for worse. Right click on an un-revealed square. Simple analysis will solve most of the puzzle, but not all. Before going on to the next sections, where we give you more advanced hints, you should play for a bit and see what you can work out for yourself. Sometimes you end up in a situation where there is no way to deduce what the configuration is. In these cases you either have to guess or use the hint function (<menuchoice><guimenu>Game</guimenu><guimenuitem>Hint</guimenuitem></menuchoice>). Sometimes it isn't a 50/50 situation and instead of taking a hint you might like to gamble on the most likely outcome. Starting from an ocean of squares with unknown contents you must mark all the squares with mines and reveal all the clear squares. You don't actually have to mark the squares with mines, but it is a lot easier if you do. Once you have cleared all the un-mined squares, any remaining mined squares will be marked for you. Strategy The Basics The Details The Goal The extra mine disguises a copy of the previous example. The first half of this section explains the very basics of deducing where mines are. If you are familiar with how to play the game skip down to the <link linkend="advanced">Advanced Situations</link> subsection. The game can be paused automatically when you start using another window, or paused by choosing <guimenuitem>Pause</guimenuitem> from the <guimenu>Game</guimenu> menu. The playing area is blanked so you can't think ahead while the clock is stopped. To start playing again press the button labelled "Press to Resume". The high score for the custom size is usually meaningless. Any other player can make the game size smaller and get a better time. If you only ever play one custom size then it will tell you how well you are doing. This is the only reason it is still recorded. The high scores dialog. The next simplest case is where you have two covered squares adjacent to each other and both squares on one side are marked with a 2. If they have no other uncleared neighbors, then both squares are mines. The other check-box in the properties dialog is entitled Use "Too many flags" warning. This feature will show you a warning when you have placed too many flags. This is disabled by default, as it provides so much hint that it could be considered "cheating". The preferences dialog. The sequence of flags when "I'm not sure" flags are enabled. The simplest case. The stages of the game: the start, playing, losing, and winning. The three mine case. The time you have taken is displayed on the bottom right of the window. Beside it, on the left, is a count of the number of mines you have cleared and the total number hidden on the board. The two mine case. The yellow face shows you how well you are going. There is a check-box in the properties dialog entitled Use "I'm not sure" flags. If you enable this then, when right-clicking on a square, it cycles between the normal flags used to mark the square as mined, a flag with a question mark on it and no flag. This new flag is to help you with your reasoning, to mark mines you think might be a mine but aren't sure about. These flags have no effect on the game; you still have to use the appropriate number of red marker flags to be able to clear around a numbered square. These are a few basic situations which should be incredibly obvious, but if you're struggling with how to play the game, they should be useful. The first case is where you have an isolated covered square and most of the squares around it read 1. That covered square is a mine. These are some more complicated situations. They are a bit of a step up from the last section, so we hope you took our advice and played for a bit first. These are the simplest cases. Other permutations on these simple situations are possible, for example the three squares in the previous example could be in an L shape. This document is dual-licensed. You may either distribute and/or modify it under the terms of the <ulink type="help" url="gnome-help:gpl">GNU General Public License</ulink>, which the <link linkend="license">program is licensed</link> under, or you can distribute it under the terms of the GNU Free Documentation License. A copy of this follows. This manual describes version 2.14 of Mines. This manual is part of a collection of GNOME manuals distributed under the GFDL. If you want to distribute this manual separately from the collection, you can do so by adding a copy of the license to the manual, as described in section 6 of the license. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the <citetitle><ulink type="help" url="gnome-help:gpl">GNU General Public License</ulink></citetitle> for more details. This program is free software; you can redistribute it and/or modify it under the terms of the <citetitle><ulink type="help" url="gnome-help:gpl">GNU General Public License</ulink></citetitle> as published by the <ulink type="http" url="http://www.fsf.org">Free Software Foundation</ulink>; either version 2 of the License, or (at your option) any later version. This section provides information on the controls and the important menu items. If you need help figuring where the mines are, see the <link linkend="strategy">strategy section</link>. This situation can be disguised, for example consider the figure below. It is the same situation, but the extra mined square on the right has increased all the numbers by 1. This sort of situation is tricky. In all the other examples you can learn to look for patterns and avoid thinking hard. Situations like the one in the figure don't crop up easily or in easily recognisable places. Often it is easier to just ignore them and keep playing elsewhere in the hope that you can start clearing the squares from the other side. Tips To mark a square as having a mine you right-click on it and a flag is placed there. If you click on a square with a number and you have placed precisely that number of flags on adjacent squares then the remaining squares are revealed. If you didn't mark the mines correctly then an explosion will occur and the game is over. Squares with no adjacent mines (i.e. blank squares) have their adjacent squares automatically cleared. To report a bug or make a suggestion regarding the <application>Mines</application> application or this manual, follow the directions in the <ulink url="http://bugzilla.gnome.org">GNOME bug reporting database.</ulink>. UNDER NO CIRCUMSTANCES AND UNDER NO LEGAL THEORY, WHETHER IN TORT (INCLUDING NEGLIGENCE), CONTRACT, OR OTHERWISE, SHALL THE AUTHOR, INITIAL WRITER, ANY CONTRIBUTOR, OR ANY DISTRIBUTOR OF THE DOCUMENT OR MODIFIED VERSION OF THE DOCUMENT, OR ANY SUPPLIER OF ANY OF SUCH PARTIES, BE LIABLE TO ANY PERSON FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES OF ANY CHARACTER INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF GOODWILL, WORK STOPPAGE, COMPUTER FAILURE OR MALFUNCTION, OR ANY AND ALL OTHER DAMAGES OR LOSSES ARISING OUT OF OR RELATING TO USE OF THE DOCUMENT AND MODIFIED VERSIONS OF THE DOCUMENT, EVEN IF SUCH PARTY SHALL HAVE BEEN INFORMED OF THE POSSIBILITY OF SUCH DAMAGES. Use "Too many flags" warning Using the Mouse Window Size You are being timed. To be good at this game you have to work both quickly and correctly. Once you have found all the mines and revealed all the un-mined squares the clock is stopped. Your score is the time you took; the shorter the better. You start the game with a grid of covered squares and no idea what is in them. Clicking on a square reveals what is in it: either empty ocean or a mine. If you find a mine bad things happen and the game is over. If you uncover an empty square, you survive and get to try again. We do give you a bit of help; if you find an empty piece of ocean a number is printed there telling you how many mines are in the adjacent squares (or no number if there are no nearby mines). Once you find a few clear squares you can start to deduce which squares have mines in them and which don't. You win the game once you have revealed all the un-mined squares and marked all the mined squares. callum@spooky-possum.org translator-credits Project-Id-Version: gnomine_help
POT-Creation-Date: 2008-02-12 10:54+0000
PO-Revision-Date: 2008-12-19 16:40+0100
Last-Translator: Iñaki Larrañaga Murgoitio <dooteo@euskalgnu.org>
Language-Team: Basque <itzulpena@euskalgnu.org>
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
X-Generator: KBabel 1.11.4
Plural-Forms:  nplurals=2; plural=(n != 1);
 "Ez nago ziur" banderak 2006 <application><application>Minak</application></application> jokoa buru-hausgarri bat da. Garuna erabiliz eta suerte apur batekin, ozeanoan igeri dauden minak aurkitu behar dituzu. <application>GNOME Minak</application> Pista-k (<email>szekeres@cyberspace.mht.bme.hu</email>) idatzi zuen. Eskuliburu hau Callum McKenzie-k (<email>callum@spooky-possum.org</email>) idatzi zuen Vincent Povirk-en laguntzarekin eta Tim Riehle-ren (<email>tkriehle@citilink.com</email>) eskuliburu batean oinarrituz. Aplikazio honi buruzko akatsen berri emateko edo oharrak bidaltzeko, <ulink url="ghelp:gnome-feedback" type="help">dokumentua</ulink>n aurkituko dituzu argibideak. <application>Minak</application>en eskuliburua @@image: 'figures/easy-hints-1.png'; md5=4ce524b08d7be427097a63c3cdfec6ee @@image: 'figures/easy-hints-2.png'; md5=e37f8d409337ff13d85f36208fd04008 @@image: 'figures/easy-hints-3.png'; md5=d444acbdd254318948b8f07c9b390ba6 @@image: 'figures/faces.png'; md5=c4ef2dc5a8fee43f2e00b49d2a3151eb @@image: 'figures/flags.png'; md5=21bb5789d11d2ea40515a4dfcfd597ca @@image: 'figures/hard-hints-1.png'; md5=38f10c8159ec5526f1bd58ff322b7142 @@image: 'figures/hard-hints-2.png'; md5=d5eecaed4e281d9d8db67f039627aa59 @@image: 'figures/hard-hints-3.png'; md5=12f7ceaa8d7187cb694927ac015c10a8 @@image: 'figures/highscores.png'; md5=a62a94c773d992dcf399ee09ec35da5a @@image: 'figures/imnotsureflagscheckbox.png'; md5=58be021ed1cf90add08931282ec19bad @@image: 'figures/main-window.png'; md5=0f49e2af3381111f4d13575fefd52d87 @@image: 'figures/preferences.png'; md5=b0a57c641f5d123f509e71c632294a12 @@image: 'figures/statusbar.png'; md5=8fd07589a7caac8c0c7c42007adb6835 <citetitle><ulink type="help" url="gnome-help:gpl">GNU Lizentzia Publiko Orokorra</ulink></citetitle>ren kopia <citetitle>GNOMEren erabiltzaile</citetitle>aren gidaren eranskinean daukazu. <citetitle><ulink type="help" url="gnome-help:gpl">GNU GNU Lizentzia Publiko Orokorra</ulink></citetitle>ren kopia bat lor dezakezu Free Software Foundation-en bidez. Bisitatu <ulink type="http" url="http://www.fsf.org">haien webgunea</ulink>, edo idatzi hona: <address> Free Software Foundation, Inc. <street>59 Temple Place</street> - 330. Suitea <city>Boston</city>, <state>MA</state><postcode>02111-1307</postcode><country>AEB</country></address> Hain argia ez den eredu bat. Ekintza Egoera aurreratuak Ematen duen bezain zaila. Eman dezagun azpiko irudian badakigula goiko ezkerreko laukiak mina bat duela. Beraz, 2 zenbakiaz markatuta ezkerretara dituen bi laukiek mina bakarra behar dute. Jarraian, kontuan har ezazu ezker-ezkerrean dagoen 2 horrek adierazten duela azpian estalita dauden laukietako batek mina bat duela, nahiz eta oraindik ez dakigun zeinek. Lauki horietako batek mina bat duenez, eta guztiak 2 horren ondoan daudenez, badakigu 2 hori bi lauki horietako batek asetzen duela (oraindik ez dakigu zein laukik). Horrek esan nahi du 4aren azpian dagoen laukia hutsik dagoela, bigarren 2 horren alboan baitago eta ez da aurrez identifikatu dugun bi laukietako bat. Lauki huts horrek, beraz, lau lauki uzten ditu 4 horren inguruan –guztiek minak izan behar dute. Ez digu soilik esaten zein diren 4a inguratzen duten minak, baizik minak 2aren inguruko zein laukitan dauden ere esaten digu. Egileak Automatikoki garbituko du. Klik egin duzun laukiaren inguruan markatu gabe dauden laukiak desestaliko ditu, baina soilik laukiko zenbakia eta markatutako lauki kopurua bat baldin badatoz. Menuan <menuchoice><guimenu>Ezarpenak</guimenu><guimenuitem>Hobespenak</guimenuitem></menuchoice> aukeratzen baduzu, jokoen tamaina aldatu ahal izango duzu. Zenbat eta tamaina handiagoa, orduan eta denbora luzeago eta kontzentrazio handiagoa. Joko txikiak frenetikoagoak dira. Tamaina txikia, ertaina, handia eta pertsonalizatua daude. Callum Callum McKenzie Callum McKenzie <email>callum@spooky-possum.org</email> DOKUMENTUA ETA DOKUMENTUAREN BERTSIO ALDATUAK GNU DOKUMENTAZIO LIBREAREN LIZENTZIAREN BALDINTZEN ARABERA EMATEN DIRA, ETA ONDOREN ZEHAZTEN DIRENAK ONARTZERA BEHARTZEN DUTE: <placeholder-1/> DOKUMENTUA "DAGOEN-DAGOENEAN" EMATEN DA, INOLAKO BERMERIK GABE, EZ ESPRESUKI ADIERAZITAKORIK ETA EZ INPLIZITURIK ERE; EZ DA BERMATZEN DOKUMENTUA EDO BERTSIO ALDATUA AKATSIK GABEA DENIK, MERKATURATZEKO EDO XEDE JAKIN BATERAKO EGOKIA DENIK EDO ARAURIK HAUSTEN EZ DUENIK, HORI GUZTIA MUGARIK GABE. DOKUMENTUAREN EDO DOKUMENTUAREN BERTSIO ALDATUAREN KALITATEARI, ZEHAZTASUNARI ETA PERFORMANTZIARI BURUZKO ERANTZUKIZUN OSOA ZUREA DA. DOKUMENTUREN BATEK EDO BERTSIO ALDATUREN BATEK EDOZEIN MOTATAKO AKATSIK IZANEZ GERO, ZUK (EZ HASIERAKO IDAZLEAK, EZ EGILEAK ETA EZ INONGO KOLABORATZAILEK) ZEURE GAIN HARTU BEHARKO DUZU BERRIKUSTEKO, KONPONTZEKO EDO ZUZENTZEKO BEHAR DIREN ZERBITZU GUZTIEN KOSTUA. BERME-UKATZE HAU LIZENTZIA HONEN FUNTSEZKO ZATIA DA. EZ DA BAIMENIK EMATEN EZEIN DOKUMENTU EDO BERTSIO ALDATU ERABILTZEKO, BALDIN ETA EZ BADA BERME-UKATZE HAU ONARTZEN. Ohar-bidaltzea Azkenik, ilara berean estalitako hiru lauki badaude eta erdiko laukiaren ondoko laukiak 3 bat badu (oraingoan ere estalitako laukirik ez dagoela inguruan), hiru laukiak minak dira. GNOMEren dokumentazio-proiektua GNOME Minak Jokoaren tamaina eta zailtasuna Puntuaziorik onenak Iradokizunak Saguaren kontrol ohikoagoetara ohituta baldin bazaude, lasai egon, oraindik balio dute. Trabatuta gelditzen bazara –oso ohikoa da logikoki ebatzi ezin dituzun bi aukera izatea–, bi aukera hauek dituzu: asmatzea edo iradokizuna eskatzea. <guimenu>Jokoa</guimenu> menutik <guimenuitem>Iradokizuna</guimenuitem> aukeratzen duzunean, lauki bakar bat desestaltzen da (eta, lauki hutsa bada, inguruko laukiak ere). Laukia ausaz aukeratzen da, baina lagungarria izaten da. 10 segundoko penalizazioa ezartzen zaizu iradokizun bat eskatzen duzun bakoitzean. Mina batean ezkerreko botoiaz klik egiten baduzu, edo mina bat duen lauki bat automatikoki desestaltzen baduzu, galdu egingo duzu. Istripuak gerta ez daitezen, ezin duzu lauki baten ingurua automatikoki desestali, markatutako mina kopurua eta laukian ematen den zenbakia berdinak ez badira behintzat. Zure puntuazioa hamar altuenen artean baldin badago, puntuazio altuenen zerrenda agertuko zaizu eta puntuazio-taulan gordetako izena aldatzeko aukera izango duzu. Aurrerago, puntuazioak ikusi nahi badituzu, aukeratu <guimenu>Jokoa</guimenu> menutik <guimenuitem>Puntuazioak</guimenuitem>. Elkarrizketa-koadroaren gainaldean dagoen goitibeherako menuak beste puntuazioak ikusteko aukera ematen dizu. Informazio garrantzitsua Azpiko adibidean, minak ‘bat’en ezkerretara daude. Ondorio horretara eliminazio-prozesu baten ondoren iritsi gara. 2aren ezkerreko laukia mina bat balitz, 1ek mina kopuru gorena izango lukete eta ez lukete lauki librerik utziko 2aren ondoko bigarren minarentzat (1en alboan egongo bailitzake). Erdiko laukia ezabatu dugunean, bi lauki gelditzen zaizkigu 2rako; beraz, biak minak dira. Sarrera 2006ko urtarrila Egin klik ezkerreko botoiaz desestalitako lauki batean. Egin klik ezkerreko botoiaz desestali gabeko lauki batean. Lizentzia Enpresek beren produktu eta zerbitzuak bereizteko erabiltzen dituzten izen asko marka erregistratu gisa hartu behar dira. Izen horiek GNOMEren edozein agiritan agertzen direnean, eta GNOMEren Dokumentazio Proiektuko kideak marka komertzialak direla konturatzen direnean, izen horiek maiuskulaz idatzita agertuko dira, osorik edo hasierako letra bakarrik. Laukia mina batez markatuko du. Jada markatua baldin badago, marka ezabatu egingo da. McKenzie Mina-jokoaren eskuliburuaren 2.9 bertsioa Mina bilatzailea joko ospetsuaren GNOMErako bertsioa da Minak, non minak saihesten diren bitartean aztarnak daude minen kokalekuei buruz. Orain adibide korapilatsu bat ikusiko dugu. Bi puntu argitzen ditu. Lehenengoa: minak zein laukitan dauden ziur ez badakizu ere, aukerak murriztea lagungarria da. Bigarrena: minarik zein laukitan ez dagoen jakitea, zein laukitan dauden jakitea bezain ona da. Elkarrizketa-koadroaren eskuineko aldean, tamaina pertsonalizatua erregulatzeko hiru parametro daude. Horizontala eta Bertikala nahi ditugun laukien saretaren zabalera eta altuera dira. Mina kopurua –jakina– aurkitu behar duzun mina kopurua da. Kopuru hori oso altua edo oso baxua baldin bada, jokoa oso erraza da. Mina kopurua mugatua da: lauki kopurua baino pixka bat gutxiago. Azken aholkua: gogoratu leihoaren behealdean ezkerretan dagoen kontadoreak zenbat mina falta zaizkizun esaten dizula. Askotan, jokoaren amaieratik gertu, zenbaki horrek lagundu egiten dizu minak non egon daitezkeen asmatzen. Nola asmatu, zure esku dago. Jokoa gelditzea Baimena ematen da dokumentu hau kopiatu, banatu eta/edo aldatzeko, Free Software Foundation-ek argitaratutako GNU Dokumentazio Librearen Lizentziaren 1.1. bertsioan edo berriago batean ezarritako baldintzetan, atal aldaezinik gabe, aurreko azaleko testurik gabe eta atzeko azaleko testurik gabe. GFDL lizentziaren kopia <ulink type="help" url="ghelp:fdl">esteka honetan</ulink> edo eskuliburu honekin batera ematen den COPYING-DOCS fitxategian aurkituko duzu. Gogoratu, taularen ertza garbitutako eremu bat bezalakoa dela (baina zenbakirik gabea). Askotan taularen ertzetik hastea izaten da modurik errazena. Leihoaren tamaina aldatzeak laukiak handiagotu edo txikiagotzen ditu. Pantaila ahalik eta handiena egin nahi baduzu, <guimenuitem>Pantaila betea</guimenuitem> aukera daukazu <guimenu>Ezarpenak</guimenu> menuan. <keycap>F11</keycap> tekla ere erabil dezakezu pantaila betearen modua aktibatzeko/desaktibatzeko. Emaitza Lauki horretako edukia agertzen da. Onerako edo txarrerako. Egin klik eskuineko botoiaz desestali gabeko lauki batean. Azterketa xinple batek buru-hausgarriaren zati handiena ebatziko luke, baina ez osoa. Hurrengo ataletara joan baino lehen (iradokizun aurreratuagoak emango dizkizuegu han), jolastu apur bat eta ikusi zenbat ebatz dezakezuen zuek bakarrik. Batzuetan, konfigurazioa zein den jakiteko ez da modu logikorik egoten. Horrelakoetan, asmatzen saia zaitezke edo iradokizunak erabili (<menuchoice><guimenu>Jokoa</guimenu><guimenuitem>Iradokizuna</guimenuitem></menuchoice>. Batzuetan ez duzu % 50eko aukerarik, eta iradokizun bat eskatu beharrean hobe izaten da emaitza probableenarekin ausartzea. Zer duten ez dakigun laukiez beteriko ozeano batetik hasita, minak dituzten lauki guztiak markatzera eta lauki hutsak desestaltzera iritsi behar duzu. Egia esan, ez dituzu zertan minak dituzten laukiak markatu, baina askoz errazagoa da hala egiten baduzu. Minarik gabeko lauki guztiak desestali dituzunean, minadun laukiren bat markatu gabe gelditzen bada, automatikoki markatuko da. Estrategia Oinarrizkoak Zehaztasunak Helburua Mina gehigarri horrek aurreko adibidearen kopia bat ezkutatzen du. Atal honen lehenengo erdian, minak non dauden ondorioztatzeko oinarriak azaltzen dira. Jokoa ezaguna bazaizu, joan zuzenean <link linkend="advanced">Egoera aurreratuak</link> azpiatalera. Jokoa automatikoki geldi daiteke beste leiho bat erabiltzen hasten zarenean, edo <guimenu>Jokoa</guimenu> menuan <guimenuitem>Gelditu</guimenuitem> aukeratzen duzunean. Joko-eremua garbitu egiten da; beraz, ordularia geldituta egon arren ezin duzu pentsatu. Berriro ere jolasten hasteko, sakatu "Sakatu berriz hasteko" botoia. Pertsonalizatutako tamainaren puntuazio altuenak garrantzi gutxikoak izan ohi dira. Beste edozein jokalarik tamaina txikitu eta denbora hobea egin dezake. Beti tamaina berean jolasten baduzu, zer moduzkoa zaren jakiteko balioko dizu. Arrazoi horregatik soilik jasotzen dira puntuazio horiek. Puntuazio altuenen elkarrizketa-koadroa. Hurrengo kasu errazena: estalitako bi lauki baldin badituzu elkarren ondoan, eta inguruko laukiak 2 zenbakiaz markatuta baldin badaude, eta inguruan ez badago estalitako beste laukirik, bi laukiak minak dira. Propietateen elkarrizketa-koadroan dagoen beste kontrol-laukiak ‘Erabili "Bandera gehiegi" abisua’ du izena. Elementu honek abisu bat bistaratuko du bandera gehiegi erabiltzen dituzunean. Elementu hau desaktibatuta dago lehenespenez, hainbeste iradokizun ematen dituenez "tranpa egitea" bezalakoa baita. Hobespenen elkarrizketa-koadroa. "Ez nago ziur" banderak aktibatuta daudenean dagoen bandera-sekuentzia. Kasurik errazena. Jokoaren faseak: hastea, jolastea, galtzea eta irabaztea. Hiru minen kasua. Behar izan duzun denbora leihoaren beheko eskuinaldean bistaratuko da. Horren ondoan, ezkerretara, aurkitu duzun mina kopurua eta taulan ezkutatzen zen mina kopurua agertuko dira. Bi minen kasua. Aurpegi horiak zer moduz jolasten ari zaren adierazten du. Propietateen elkarrizketa-koadroan, badago ‘Erabili "Ez nago ziur" banderak’ izena duen kontrol-lauki bat. Lauki hori aktibatzen baduzu, lauki batean eskuineko botoiaz klik egitean minak markatzeko erabiltzen den bandera normala eta galdera-ikur bat duen bandera txandakatuko dira. Bandera berri honek arrazoiketa jarraitzen lagunduko dizu, zure ustez minak diren baina guztiz ziur ez zauden laukiak markatzeko. Bandera hauek ez dute inongo eraginik jokoan; marka gorriko banderak jarri behar dituzu laukiaren zenbakiaren arabera ingurua garbitzeko. Oso agerikoak diren bi egoera dira, baina, jokoan nola jolastu jakiteko ahaleginetan bazabiltza, erabilgarriak izan daitezke. Lehenengo kasua: estalitako lauki bakartu bat baduzu eta inguruko lauki gehienetan 1 zenbakia baldin badago, estalitako lauki hori mina bat da. Egoera hauek konplikatuagoak dira. Azken ataletik aurreraxeago daude; beraz, espero dugu gure aholkuak jarraitu izana eta apur bat jolastu izana. Horiek dira kasurik errazenak. Egoera xinple hauetan beste permutazio batzuk ere posible dira; esaterako, azken adibideko hiru laukiak L forman egotea. Dokumentu honek bi lizentzia ditu. Dokumentu hau <link linkend="license">programa-lizentzia</link>ren pean dagoen <ulink type="help" url="gnome-help:gpl">GNU Lizentzia Publiko Orokorra</ulink>ren arabera banatu eta/edo aldatu dezakezu, edo GNU Dokumentazio Librearen lizentziaren baldintzak betez banatu dezakezu. Jarraian, horren kopia bat dago. Eskuliburu honetan, Minak jokoaren 2.14 bertsioa azaltzen da. Eskuliburu hau GFDL lizentziarekin banatzen diren GNOME eskuliburuen bildumakoa da. Eskuliburu hau bildumatik bereizita banatu nahi baduzu, banatu dezakezu, baina eskuliburuari lizentziaren kopia bat gehitu beharko diozu lizentzian bertan 6. atalean azaltzen den bezala. Programa hau erabilgarria izango delakoan banatzen da, baina INOLAKO BERMERIK GABE; era berean, ez da bermatzen beraren EGOKITASUNA MERKATURATZEKO edo HELBURU PARTIKULARRETARAKO ERABILTZEKO. Argibide gehiago nahi izanez gero, ikus <citetitle><ulink type="help" url="gnome-help:gpl">GNU Lizentzia Publiko Orokorra</ulink></citetitle>. Programa hau software librea da; birbana eta/edo alda dezakezu Free Software Foundation-ek argitaratutako <ulink type="help" url="gnome-help:gpl"><citetitle>Lizentzia Publiko Orokorra</citetitle></ulink>ren 2. bertsioan, edo (nahiago baduzu) bertsio berriago batean, jasotako baldintzak betez gero. Kontrolei eta menu-elementu garrantzitsuei buruzko informazioa ematen ditu atal honek. Minak non dauden jakiteko laguntza behar baduzu, ikus <link linkend="strategy">estrategia atala</link>. Egoera mozorrotu egin daiteke. Aztertu hurrengo irudia. Egoera berdina da, baina eskuineko laukiak ere mina bat du, eta, beraz, zenbaki guztiei 1 gehitu zaie. Egoera multzo hori korapilatsua da. Gainerako adibide guztietan, ereduak jarraitzen ikasten duzu eta ez duzu gehiegi pentsatu behar. Irudikoa bezalako egoerak ez dira erraz agertzen eta ez dira erraz identifikatzen. Askotan errazagoa izaten da bertan behera uztea eta beste tokiren batean jolasten jarraitzea, ea hala beste aldeko laukiak garbitzen hasten garen. Aholkuak Mina duen lauki bat markatzeko, egin klik eskuineko botoiaz laukian eta bandera bat agertuko da bertan. Zenbaki bat duen lauki batean klik egiten baduzu, eta alboko laukietan justu kopuru hori aurkitu baduzu, gainerako laukiak desestali egingo dira. Minak ondo markatu ez bazenituen, leherketa bat gertatuko da eta jokoa amaitu egingo da. Alboan minarik ez duten laukien (lauki hutsen) alboko laukiak automatikoki desestaltzen dira. <application>Minak</application>i buruzko akatsen berri emateko edo oharrak bidaltzeko, <ulink url="http://bugzilla.gnome.org">GNOMEra erroreak aurkezteko datu-basea</ulink>n aurkituko dituzu argibideak. EZINGO DA INONGO ZIRKUNSTANTZIA EDO LEGE-TEORIARIK OINARRI HARTU --EZ ERANTZUKIZUN ZIBILARI BURUZKORIK (ZABARKERIA BARNE HARTUTA) EZ KONTRATUARI BURUZKORIK, EZ BESTERIK-- DOKUMENTU HONEN EDO BERTSIO ALDATU BATEN EGILEA, HASIERAKO IDAZLEA, EDOZEIN KOLABORATZAILE EDO BANATZAILE, EDO ALDERDI HORIEN EDOZEIN HORNITZAILE BESTE PERTSONA BATEN AURREAN ERANTZULE EGITEKO, PERTSONA HORREK EDOZEIN MOTATAKO KALTE ZUZENEKO, ZEHARKAKO, BEREZI, INTZIDENTAL EDO ONDORIOZKOAK JASAN DITUELAKO, BESTEAK BESTE (MUGARIK GABE), BEZEROAK GALTZEAREN, LANA ETEN BEHARRAREN, ORDENAGAILUAK EZ IBILTZEAREN EDO GAIZKI IBILTZEAREN ONDORIOZKO KALTEAK, EDO DOKUMENTUA NAHIZ HAREN BERTSIO ALDATUAK ERABILTZETIK ONDORIOZTATZEN DIREN EDO ERABILERA HORREKIN ZERIKUSIA DUEN EDOZEIN KALTE EDO GALERA, ALDERDIARI KALTE HORIEK GERTA ZITEZKEELA ADITZERA EMAN BAZAIO ERE. Erabili "Bandera gehiegi" abisua Sagua erabiltzea Leihoen tamaina Kronometratu egingo zaitugu. Joko honetan ona izateko, azkar eta zuzen ibili behar duzu. Mina guztiak aurkitzen dituzunean eta minarik gabeko lauki guztiak desestaltzen dituzunean, ordularia gelditu egingo da. Puntuazioa behar izan duzun denbora da; geroz eta gutxiago, hobeto. Jokoa hasten duzunean, estalitako laukiz betetako sareta duzu, eta ez dakizu zer dagoen lauki horietako bakoitzean. Lauki batean klik egiten baduzu, azpian zer dagoen jakingo duzu: ozeano hutsa edo mina bat. Mina aurkitzen baduzu, gauza txarrak gertatuko dira eta jokoa amaitu egingo da. Hutsik dagoen lauki bat desestaltzen baduzu, bizirik jarraituko duzu eta berriro saiatzeko aukera izango duzu. Laguntza pixka bat ematen dizugu: ozeano hutsa aurkitzen baduzu, zenbaki bat agertuko da laukian, alboko laukietan zenbat mina dauden esango dizuna (zenbakirik ez badago esan nahi du ez dagoela minarik alboko laukietan). Hutsik dauden zenbait lauki aurkitzen dituzunean, minak zein laukitan dauden eta zeinetan ez ondorioztatzen has zaitezke. Jokoa irabazteko, minarik gabeko lauki guztiak desestali behar dituzu, eta mina dutenak markatu egin behar dituzu. callum@spooky-possum.org Hizkuntza Politikarako Sailburuordetza <hizpol@ej.gv.es>, 2008. 