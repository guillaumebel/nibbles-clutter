��    [      �     �      �     �     �     �     �     �     �     �     �     �     �     �  	   �     �     �     �                          	            .     �   ?  �  $	  C  �
  L   �  M   D  H   �    �  _   �  |   Y  �   �  �   �     g     o     {  �   �    8    N     \     b  +   p     �     �     �     �  
   �     �     �  x     �  �     (     5     =     C  7  K     �     �     �     �  �  �  ]  i     �     �     �     �                 ,      1   �   7     �   &   "  3   2"  �   f"  �   d#    d$  w  k%  �   �'  �   �(  �   �)  �  �*     [-     n-  �   t-  �   S.  �   /     �/     �/  �  �/     51     81     <1     ?1     B1     E1     I1     L1     O1     S1     V1  	   Y1     c1     g1     j1     l1     o1     q1     s1     u1     w1     z1  E   |1  	  �1  �  �2  J  u4  L   �5  M   6  H   [6  !  �6  l   �8  �   39  �   �9  �   �:     �;     �;     �;  �   �;  \  �<  .  �?     B     "B  +   0B     \B     kB  <   |B     �B     �B     �B     �B  �   	C  �  �C     \E     dE     lE  	   sE  b  }E     �F     �F     �F     G  �  "G  �  �H     yK     �K  
   �K     �K  '   �K     �K     �K     �K  �   L  A  �L  8   "N  M   [N    �N    �O  �   �P  �  �Q    ~T  �   �U  �   ZV  @  IW     �Z     �Z    �Z  �   �[  �   R\     �\  ?   �\         ,   ;                  )           /   
           7   F   :   U   S           <               T           C   1   2   X             3   @   D   R   ?      -             A   6   Z   Q         B                          P      0   M   E   *   >       5                      	              "   L         N   I          4           [   %       G      V       +       =      .       W   #   9       K           J           O   $      &      8   '          (   H       !   Y    10 108 11 12 13 138 14 15 172 18 20 2000-2002 210 28 3 42 5 6 7 8 82 9 <application>Five or More</application> Manual <application>Five or More</application> is included in the <filename>GNOME-games</filename> package, which is part of the GNOME desktop environment. This document describes version 2.8 of <application>Five or More</application>. <application>Five or More</application> was written by Róbert Szókovács (<email>szo@appaloosacorp.hu</email>) and Szabolcs Bán (<email>shooby@gnome.hu</email>). This manual was written by Emese Kovács (<email>emese@gnome.hu</email>). For information on how to make comments, suggestions and to report bugs please see the <ulink url="ghelp:gnome-feedback" type="help">feedback document</ulink>. <application>Five or More</application>, is the Gnome port of the once popular Windows game called <application>Color Lines</application> The game's objective is to align as often as possible five or more objects of the same color and shape causing them to disappear. Play as long as possible, and be #1 in the High Scores. @@image: 'figures/glines-gameover.png'; md5=949d09a01ee02944bf3f278ce1ebc2ba @@image: 'figures/glines-startgame.png'; md5=e6e58f72dc4802d89a45a7726f4c7fd7 @@image: 'figures/preferences.png'; md5=745336aefd6ade740f6064bdf4121379 A copy of the <citetitle>GNU General Public License</citetitle> is included as an appendix to the <citetitle>GNOME Users Guide</citetitle>. You may also obtain a copy of the <citetitle>GNU General Public License</citetitle> from the Free Software Foundation by visiting <ulink url="http://www.fsf.org" type="http">their Web site</ulink> or by writing to <address> Free Software Foundation, Inc. <street>59 Temple Place</street> - Suite 330 <city>Boston</city>, <state>MA</state><postcode>02111-1307</postcode><country>USA</country></address> A game of <application>Five or More</application> begins with some random objects on the board. Any changes you make are instantly applied and remembered for the next time you run <application>Five or More</application>. As objects disappear, you get points. The points you score depends on the number of objects you managed to align (see <xref linkend="glines-table1"/>). Your score is shown in the upper right corner of the window. As the game progresses, the board gets more and more crowded (unless you are really good...) and making objects disappear gets harder and harder. The game is over, when the board is full. Authors Basic usage Customizing the Game DOCUMENT AND MODIFIED VERSIONS OF THE DOCUMENT ARE PROVIDED UNDER THE TERMS OF THE GNU FREE DOCUMENTATION LICENSE WITH THE FURTHER UNDERSTANDING THAT: <placeholder-1/> DOCUMENT IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE DOCUMENT OR MODIFIED VERSION OF THE DOCUMENT IS FREE OF DEFECTS MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY, ACCURACY, AND PERFORMANCE OF THE DOCUMENT OR MODIFIED VERSION OF THE DOCUMENT IS WITH YOU. SHOULD ANY DOCUMENT OR MODIFIED VERSION PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL WRITER, AUTHOR OR ANY CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. THIS DISCLAIMER OF WARRANTY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY DOCUMENT OR MODIFIED VERSION OF THE DOCUMENT IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER; AND Each theme consists of a single PNG image file. This image consists of an array of seven rows, corresponding to the different object colors/shapes, and four columns, each of which is a frame in the animation of the object. The actual size of the image is irrelevant, it will be rescaled, but the sub-images must form a four by seven grid to get the correct effect. When using SVG remember that the image size is the page size, if your grid does not fill the page try shrinking the page size. Use an existing theme as a guide. Emese Emese Kovács Emese Kovács <email>emese@gnome.hu</email> Feedback Five or More Five or More Manual V2.8 GNOME Documentation Project Game over! Height How to create new themes If you start using the mouse again, the cursor will disappear. The cursor will reappear if you use the arrow keys again. Instead of using the mouse you can use the keyboard to play <application>Five or More</application>. To use the keyboard, hit an arrow key and a cursor will appear near the center of the board. The arrow keys will move the cursor. When the cursor is over the object you want to move, use the spacebar to select it. Then shift the cursor to the object's destination and hit the space bar again to move the object. Introduction Kovács Large License Many of the names used by companies to distinguish their products and services are claimed as trademarks. Where those names appear in any GNOME documentation, and the members of the GNOME Documentation Project are made aware of those trademarks, then the names are in capital letters or initial capital letters. Medium Number of Types Number of objects Objects per Turn Permission is granted to copy, distribute and/or modify this document under the terms of the GNU Free Documentation License (GFDL), Version 1.1 or any later version published by the Free Software Foundation with no Invariant Sections, no Front-Cover Texts, and no Back-Cover Texts. You can find a copy of the GFDL at this <ulink type="help" url="ghelp:fdl">link</ulink> or in the file COPYING-DOCS distributed with this manual. Playing <application>Five or More</application> is easy. At startup, you find yourself with five objects randomly positioned on the board. Each turn, you are allowed to move one object. You can move it anywhere on the board as long as there is a clear path. Afterward, the computer drops more objects at random positions (the exact number depends on the size of the game). You can preview the next objects to be dropped in the upper right corner of the game's window. If you manage to align five objects of the same shape and color, they disappear and you are given an extra move before more objects drop. Playing Five or More Playing the Game Preferences Score given Scoring in Five or More September 2004 Size Small Starting <application>Five or More</application> presents you with the <interface>game board</interface> and a new game starts. What it looks like is shown in <xref linkend="mainwindow-fig"/>. The <guimenu>Board Size</guimenu> section lets you select the size of the board. Each size has a slightly different balance: larger sizes give you more space to move, but more objects arrive each turn. The parameters for each size are given in <xref linkend="table-sizes"/>. The main window when you start a game. This manual describes version 2.12 of Five or More. This manual is part of a collection of GNOME manuals distributed under the GFDL. If you want to distribute this manual separately from the collection, you can do so by adding a copy of the license to the manual, as described in section 6 of the license. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the <citetitle>GNU General Public License</citetitle> for more details. This program is free software; you can redistribute it and/or modify it under the terms of the <citetitle>GNU General Public License</citetitle> as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version. To make a move, click on one of the objects. When an object is activated it starts rotating (or doing something else depending on the theme, but let us not anticipate&hellip;). You can then select its destination by clicking an empty square. If the path to the destination is clear the activated item travels there. If the destination square is occupied by another object nothing will be moved and the selection will be transferred to the new object. If the path is not clear (you cannot jump objects over obstacles!) the application prints a warning in the status bar (bottom of the window) and the original object remains active. To play Five or More one must align colored pieces as the board gets filled with randomly placed pieces. When five or more pieces of the same color get lined up, they disappear. The game ends when the board gets filled up all the way. To report a bug or make a suggestion regarding the <application>Five or More</application> application or this manual, follow the directions in the <ulink url="ghelp:gnome-feedback" type="help">GNOME Feedback Page</ulink>. To run <application>Five or More</application>, select <guimenuitem>Five or More</guimenuitem> from the <guisubmenu>Games</guisubmenu> submenu of the <guimenu>Main Menu</guimenu>, or type <command>glines</command> on the command line. UNDER NO CIRCUMSTANCES AND UNDER NO LEGAL THEORY, WHETHER IN TORT (INCLUDING NEGLIGENCE), CONTRACT, OR OTHERWISE, SHALL THE AUTHOR, INITIAL WRITER, ANY CONTRIBUTOR, OR ANY DISTRIBUTOR OF THE DOCUMENT OR MODIFIED VERSION OF THE DOCUMENT, OR ANY SUPPLIER OF ANY OF SUCH PARTIES, BE LIABLE TO ANY PERSON FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES OF ANY CHARACTER INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF GOODWILL, WORK STOPPAGE, COMPUTER FAILURE OR MALFUNCTION, OR ANY AND ALL OTHER DAMAGES OR LOSSES ARISING OUT OF OR RELATING TO USE OF THE DOCUMENT AND MODIFIED VERSIONS OF THE DOCUMENT, EVEN IF SUCH PARTY SHALL HAVE BEEN INFORMED OF THE POSSIBILITY OF SUCH DAMAGES. Using the Keyboard Width You can configure <application>Five or More</application> by clicking on <guimenuitem>Preferences</guimenuitem> in the applications <guimenu>Settings</guimenu> menu. The following <interface>dialog</interface> will appear: You can select different object styles and background colors in the <guimenu>Themes</guimenu> section. If you find it difficult distinguishing the colours, try the shapes theme. You can speed up the movement of the objects by checking the Use fast moves box in the <guimenu>General</guimenu> preferences section. emese@gnome.hu translator-credits Project-Id-Version: glines_help
POT-Creation-Date: 2008-02-12 10:50+0000
PO-Revision-Date: 2008-12-19 15:00+0100
Last-Translator: Iñaki Larrañaga Murgoitio <dooteo@euskalgnu.org>
Language-Team: Basque <itzulpena@euskalgnu.org>
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
X-Generator: KBabel 1.11.4
Plural-Forms:  nplurals=2; plural=(n != 1);
 10 108 11 12 13 138 14 15 172 18 20 2000-2002 210 28 3 42 5 6 7 8 82 9 <application>Bost edo gehiago</application> aplikazioaren eskuliburua <application>Bost edo gehiago</application> <filename>GNOME-games</filename> paketearen barruan sartzen da, eta hori GNOMEren mahaigaineko ingurunearen parte da. Dokumentu honek <application>Bost edo gehiago</application> aplikazioaren 2,8 bertsioa deskribatzen du. <application>Bost edo gehiago</application> Robert Szókovács-ek (<email>szo@appaloosacorp.hu</email>) eta Szabolcs Bán-ek (<email>shooby@gnome.hu</email>) idatzi zuten. Eskuliburu hau Emese Kovács-ek idatzi zuen (<email>emese@gnome.hu</email>). Komentarioak egiteko informazioa eta proposamenak lortzeko eta akatsen berri emateko, ikus <ulink url="ghelp:gnome-feedback" type="help">oharrak bidaltzeko dokumentua</ulink>. <application>Bost edo gehiago</application> aplikazioa Windows-en <application>Color Lines</application> joko ezagunaren Gnome-ren moldaketa da. Jokoaren helburua da kolore eta forma bereko bost bola edo gehiago ahalik eta gehienetan lerrokatzea eta desagerraraztea. Jolastu ahalik eta gehien eta izan lehenengoa onenen zerrendan. @@image: 'figures/glines-gameover.png'; md5=949d09a01ee02944bf3f278ce1ebc2ba @@image: 'figures/glines-startgame.png'; md5=e6e58f72dc4802d89a45a7726f4c7fd7 @@image: 'figures/preferences.png'; md5=745336aefd6ade740f6064bdf4121379 <citetitle>GNU Lizentzia Publiko Orokorra</citetitle>ren kopia <citetitle>GNOMEren erabiltzaile</citetitle>aren gidaren eranskinean daukazu. <citetitle>GNU Dokumentazio Librearen Lizentzia</citetitle>ren kopia bat lor dezakezu Free Software Foundation-en bidez. Bisitatu <ulink url="http://www.fsf.org" type="http">haien webgunea</ulink>, edo idatzi hona: <address> Free Software Foundation, Inc. <street>59 Temple Place</street> - 330. Suitea <city>Boston</city>, <state>MA</state><postcode>02111-1307</postcode><country>AEB</country></address> <application>Bost edo gehiago</application> jokoa taulan ausaz kokatutako zenbait objektu daudela hasten da. Egiten duzun edozein aldaketa mementoan aplikatuko da, eta, <application>Bost edo gehiago</application> aplikazioa abiarazten duzun hurrengo aldian, kontuan hartuko da. Objektu bat desagertzen denean, puntuak irabazten dituzu. Lortzen dituzun puntuak lerrokatzen dituzun objektuen araberakoak dira (ikus: <xref linkend="glines-table1"/>). Zure puntuazioa leihoaren goiko eskuineko izkinan agertzen da. Jokoak aurrera egiten duen heinean, taula geroz eta gehiago beteko da (baldin eta ikaragarri ona ez bazara...) eta objektuak desagerraraztea gero eta zailagoa izango da. Taula guztiz betetzen denean, amaitu da jokoa. Egileak Oinarrizko erabilera Jokoa pertsonalizatzea DOKUMENTUA ETA DOKUMENTUAREN BERTSIO ALDATUAK GNU DOKUMENTAZIO LIBREAREN LIZENTZIAREN BALDINTZEN ARABERA EMATEN DIRA, ETA ONDOREN ZEHAZTEN DIRENAK ONARTZERA BEHARTZEN DUTE: <placeholder-1/> DOKUMENTUA "DAGOEN-DAGOENEAN" EMATEN DA, INOLAKO BERMERIK GABE, EZ ESPRESUKI ADIERAZITAKORIK ETA EZ INPLIZITURIK ERE; EZ DA BERMATZEN DOKUMENTUA EDO BERTSIO ALDATUA AKATSIK GABEA DENIK, MERKATURATZEKO EDO XEDE JAKIN BATERAKO EGOKIA DENIK EDO ARAURIK HAUSTEN EZ DUENIK, HORI GUZTIA MUGARIK GABE. DOKUMENTUAREN EDO DOKUMENTUAREN BERTSIO ALDATUAREN KALITATEARI, ZEHAZTASUNARI ETA PERFORMANTZIARI BURUZKO ERANTZUKIZUN OSOA ZUREA DA. DOKUMENTUREN BATEK EDO BERTSIO ALDATUREN BATEK EDOZEIN MOTATAKO AKATSIK IZANEZ GERO, ZUK (EZ HASIERAKO IDAZLEAK, EZ EGILEAK ETA EZ INONGO KOLABORATZAILEK) ZEURE GAIN HARTU BEHARKO DUZU BERRIKUSTEKO, KONPONTZEKO EDO ZUZENTZEKO BEHAR DIREN ZERBITZU GUZTIEN KOSTUA. BERME-UKATZE HAU LIZENTZIA HONEN FUNTSEZKO ZATIA DA. EZ DA BAIMENIK EMATEN EZEIN DOKUMENTU EDO BERTSIO ALDATU ERABILTZEKO, BALDIN ETA EZ BADA BERME-UKATZE HAU ONARTZEN. Gai bakoitza PNG irudi-fitxategi batez osaturik dago. Irudi hori zazpi errenkada eta lau zutabeko matrizea da. Errenkadak objektuen koloreei eta formei dagozkie eta zutabe bakoitza objektuaren animaziozko markoa da. Irudiaren benetako tamainak garrantzia gutxi du, eskala aldatu egingo baitzaio, baina azpiirudiek lau bider zazpiko sareta sortu behar dute efektu egokia izan dezaten. SVG erabiltzen duzunean, gogoan izan irudiaren tamaina orri batekoa dela. Saretak ez badu orria betetzen, gutxitu orriaren tamaina. Lehendik dagoen gai bat erabili gida gisa. Emese Emese Kovács Emese Kovacs <email>emese@gnome.hu </email> Ohar-bidaltzea Bost edo gehiago 'Bost edo gehiago' aplikazioaren eskuliburuaren 2.8 bertsioa GNOMEren dokumentazio-proiektua Jokoa amaitu da! Altuera Gai berriak nola sortu Berriro ere sagua erabiltzen hasten baldin bazara, kurtsorea desagertu egingo da. Berriro ere geziak erabiltzen baldin badituzu, kurtsorea agertu egingo da. Sagua erabili ordez, teklatua erabil dezakezu <application>Bost edo gehiago</application>n jolasteko. Teklatua erabiltzeko, sakatu gezi-tekla bat, eta kurtsorea taularen erdialdean agertuko da. Gezi-teklek kurtsorea mugituko dute. Kurtsorea mugitu nahi duzun objektuaren gainean dagoenean, erabili zuriune-barra objektua hautatzeko. Ondoren, mugitu kurtsorea objektua eraman nahi duzun tokira eta sakatu zuriune-barra objektua mugitzeko. Sarrera Kovács Handia Lizentzia Enpresek beren produktu eta zerbitzuak bereizteko erabiltzen dituzten izen asko marka erregistratu gisa hartu behar dira. Izen horiek GNOMEren edozein agiritan agertzen direnean, eta GNOMEren Dokumentazio Proiektuko kideak marka komertzialak direla konturatzen direnean, izen horiek maiuskulaz idatzita agertuko dira, osorik edo hasierako letra bakarrik. Tartekoa Mota kopurua Objektu kopurua Txanda bakoitzeko objektuak Baimena ematen da dokumentu hau kopiatu, banatu eta/edo aldatzeko, Free Software Foundation-ek argitaratutako GNU Dokumentazio Librearen Lizentziaren 1.1. bertsioan edo berriago batean ezarritako baldintzetan, atal aldaezinik gabe, aurreko azaleko testurik gabe eta atzeko azaleko testurik gabe. GFDL lizentziaren kopia <ulink type="help" url="ghelp:fdl">esteka honetan</ulink> edo eskuliburu honekin batera ematen den COPYING-DOCS fitxategian aurkituko duzu. <application>Bost edo gehiago</application>n jolastea erraza da. Jokoaren hasieran bost puxtarri izango dituzu taulan han eta hemen ausaz kokatuta. Txanda bakoitzean bat mugi dezakezu. Taulako edozein tokitara mugi dezakezu, bidea garbi dagoen artean. Jarraian, ordenagailuak objektu gehiago utziko ditu erortzen ausazko lekuetara (objektu kopuru zehatza jokoaren tamainaren araberakoa da). Joko-leihoaren goiko eskuineko izkinan ikus dezakezu eroriko den hurrengo objektua. Forma eta kolore bereko bost objektu lerrokatzea lortzen baduzu, desagertu egiten dira eta objektu gehiago erori baino lehen beste mugimendu bat egiteko aukera irabaziko duzu. 'Bost edo gehiago'n jolastea Jokatzea Hobespenak Lortutako puntuazioa ‘Bost edo gehiago’n puntuak lortzea 2004ko iraila Tamaina Txikia <application>Bost edo gehiago</application> abiarazten duzunean, <interface>jokoaren taula</interface> agertuko da eta joko berri bat hasiko da. Hemen ikus dezakezu da taula nolakoa den: <xref linkend="mainwindow-fig"/> <guimenu>Taularen tamaina</guimenu> atalak taularen tamaina hautatzeko aukera ematen dizu. Tamaina bakoitzak nahiko ezaugarri desberdinak ditu: tamaina handienek mugitzeko leku gehiago dute, baina txanda bakoitzean objektu gehiago erortzen dira. Tamaina bakoitzaren parametroak hemen daude: <xref linkend="table-sizes"/>. Joko bat abiarazten duzunean agertzen den leiho nagusia. Eskuliburu honetan 'Bost edo gehiago' aplikazioaren 2.12 bertsioa azaltzen da Eskuliburu hau GFDL lizentziarekin banatzen diren GNOME eskuliburuen bildumakoa da. Eskuliburu hau bildumatik bereizita banatu nahi baduzu, banatu dezakezu, baina eskuliburuari lizentziaren kopia bat gehitu beharko diozu lizentzian bertan 6. atalean azaltzen den bezala. Programa hau erabilgarria izango delakoan banatzen da, baina INOLAKO BERMERIK GABE; era berean, ez da bermatzen beraren EGOKITASUNA MERKATURATZEKO edo HELBURU PARTIKULARRETARAKO ERABILTZEKO. Argibide gehiago nahi izanez gero, ikus <citetitle>GNU Lizentzia Publiko Orokorra</citetitle>. Programa hau software librea da; birbana eta/edo alda dezakezu Free Software Foundation-ek argitaratutako <citetitle>GNU Lizentzia Publiko Orokorra</citetitle>ren 2. bertsioan, edo (nahiago baduzu) bertsio berriago batean, jasotako baldintzak betez gero. Mugimendu bat egiteko, egin klik objektu batean. Objektu bat aktibatzen denean, biratzen hasten da (edo gaiaren araberako zerbait egiten, baina ez ditzagun gauzak aurreratu&hellip;). Objektua nora eraman nahi duzun aukeratzeko, egin klik hutsik dagoen lauki batean. Aukeratu duzun laukia hutsik baldin badago, hautatu duzun objektua bertara joango da. Aukeratu duzun laukian beste objektu bat baldin badago, ezer ere ez da mugituko eta lehengo objektuaren ordez bigarrena aktibatuko da. Bidea garbi ez badago (objektuek ezin dute traben gainetik salto egin!) aplikazioak abisu bat agertzen du egoera-barran (leihoaren behealdean) eta jatorrizko objektuak aktibatuta jarraitzen du. 'Bost edo gehiago'ra jokatzeko kolore bedineko piezak lerrokatu behar dituzu taula ausazko koloreak dituen piezas betetzen doan heinean. Kolore berdineko bost edo pieza gehiago lerrokatutakoan desagertu egingo dira. Taula erabat betetakoan amaitzen da jokoa. <application>Bost edo gehiago</application>ri buruzko akatsen berri emateko edo oharrak bidaltzeko, <ulink url="ghelp:gnome-feedback" type="help">GNOMEra oharrak bidaltzeko orria</ulink>n aurkituko dituzu argibideak. <application>Bost edo gehiago</application> abiarazteko, hautatu <guimenu>Menu nagusia</guimenu>n <guisubmenu>Jokoak</guisubmenu> azpimenuko <guimenuitem>Bost edo gehiago</guimenuitem> edo idatzi <command>glines</command> komando-lerroan. EZINGO DA INONGO ZIRKUNSTANTZIA EDO LEGE-TEORIARIK OINARRI HARTU --EZ ERANTZUKIZUN ZIBILARI BURUZKORIK (ZABARKERIA BARNE HARTUTA) EZ KONTRATUARI BURUZKORIK, EZ BESTERIK-- DOKUMENTU HONEN EDO BERTSIO ALDATU BATEN EGILEA, HASIERAKO IDAZLEA, EDOZEIN KOLABORATZAILE EDO BANATZAILE, EDO ALDERDI HORIEN EDOZEIN HORNITZAILE BESTE PERTSONA BATEN AURREAN ERANTZULE EGITEKO, PERTSONA HORREK EDOZEIN MOTATAKO KALTE ZUZENEKO, ZEHARKAKO, BEREZI, INTZIDENTAL EDO ONDORIOZKOAK JASAN DITUELAKO, BESTEAK BESTE (MUGARIK GABE), BEZEROAK GALTZEAREN, LANA ETEN BEHARRAREN, ORDENAGAILUAK EZ IBILTZEAREN EDO GAIZKI IBILTZEAREN ONDORIOZKO KALTEAK, EDO DOKUMENTUA NAHIZ HAREN BERTSIO ALDATUAK ERABILTZETIK ONDORIOZTATZEN DIREN EDO ERABILERA HORREKIN ZERIKUSIA DUEN EDOZEIN KALTE EDO GALERA, ALDERDIARI KALTE HORIEK GERTA ZITEZKEELA ADITZERA EMAN BAZAIO ERE. Teklatua erabiltzea Zabalera <application>Bost edo gehiago</application> aplikazioa konfiguratu egin dezakezu. Horretarako, egin klik <guimenu>Ezarpenak</guimenu> aplikazioen menuko <guimenuitem>Hobespenak</guimenuitem> botoian. <interface>Elkarrizketa-koadro</interface> hau agertuko da: Nahi duzun objektu-estiloa eta atzeko planoaren kolorea hauta ditzakezu <guimenu>Gaiak</guimenu> atalean. Zaila egiten bazaizu koloreak desberdintzea, erabili formak. Objektuen mugimendua bizkortu egin dezakezu. Horretarako, hobespenen <guimenu>Orokorra</guimenu> atalean hautatu ‘erabili mugimendu azkarrak’ koadroa. emese@gnome.hu Hizkuntza Politikarako Sailburuordetza <hizpol@ej.gv.es>, 2008. 