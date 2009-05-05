��    [      �     �      �     �     �     �     �     �     �     �     �     �     �     �  	   �     �     �     �                          	            .     �   ?  �  $	  C  �
  L   �  M   D  H   �    �  _   �  |   Y  �   �  �   �     g     o     {  �   �    8    N     \     b  +   p     �     �     �     �  
   �     �     �  x     �  �     (     5     =     C  7  K     �     �     �     �  �  �  ]  i     �     �     �     �                 ,      1   �   7     �   &   "  3   2"  �   f"  �   d#    d$  r  k%  �   �'  �   �(  �   �)  �  �*     V-     i-  �   o-  �   N.  �    /     �/     �/  X  �/     1     1     	1     1     1     1     1     1     1      1     #1  	   &1     01     41     71     91     <1     >1     @1     B1     D1     G1  1   I1  �   {1  �  f2  f  �3  L   _5  M   �5  H   �5  -  C6  k   q8  �   �8  �   y9  �   b:     *;     1;     F;  �   ^;  7  <  N  >?     �A     �A  +   �A  
   �A     �A  $   �A  #   	B     -B     BB     JB  �   _B  �  �B     �D     �D     �D  
   �D  e  �D     %F     ,F     =F     PF  �  bF  v  3H     �J     �J     �J     �J     �J     K     K     !K  �   'K  ,  �K  /   'M  6   WM    �M    �N    �O  �  �P    gS  �   lT  �   RU  �  KV     +Y     CY  �   KY  �   1Z  �   �Z     �[  )   �[         ,   ;                  )           /   
           7   F   :   U   S           <               T           C   1   2   X             3   @   D   R   ?      -             A   6   Z   Q         B                                 0   M   E   *   >       5                      	              "   L         N   I          4           [   %       G      V       +       =      .       W   #   9       K       P   J           O   $      &      8   '          (   H       !   Y    10 108 11 12 13 138 14 15 172 18 20 2000-2002 210 28 3 42 5 6 7 8 82 9 <application>Five or More</application> Manual <application>Five or More</application> is included in the <filename>GNOME-games</filename> package, which is part of the GNOME desktop environment. This document describes version 2.8 of <application>Five or More</application>. <application>Five or More</application> was written by Róbert Szókovács (<email>szo@appaloosacorp.hu</email>) and Szabolcs Bán (<email>shooby@gnome.hu</email>). This manual was written by Emese Kovács (<email>emese@gnome.hu</email>). For information on how to make comments, suggestions and to report bugs please see the <ulink url="ghelp:gnome-feedback" type="help">feedback document</ulink>. <application>Five or More</application>, is the Gnome port of the once popular Windows game called <application>Color Lines</application> The game's objective is to align as often as possible five or more objects of the same color and shape causing them to disappear. Play as long as possible, and be #1 in the High Scores. @@image: 'figures/glines-gameover.png'; md5=949d09a01ee02944bf3f278ce1ebc2ba @@image: 'figures/glines-startgame.png'; md5=e6e58f72dc4802d89a45a7726f4c7fd7 @@image: 'figures/preferences.png'; md5=745336aefd6ade740f6064bdf4121379 A copy of the <citetitle>GNU General Public License</citetitle> is included as an appendix to the <citetitle>GNOME Users Guide</citetitle>. You may also obtain a copy of the <citetitle>GNU General Public License</citetitle> from the Free Software Foundation by visiting <ulink url="http://www.fsf.org" type="http">their Web site</ulink> or by writing to <address> Free Software Foundation, Inc. <street>59 Temple Place</street> - Suite 330 <city>Boston</city>, <state>MA</state><postcode>02111-1307</postcode><country>USA</country></address> A game of <application>Five or More</application> begins with some random objects on the board. Any changes you make are instantly applied and remembered for the next time you run <application>Five or More</application>. As objects disappear, you get points. The points you score depends on the number of objects you managed to align (see <xref linkend="glines-table1"/>). Your score is shown in the upper right corner of the window. As the game progresses, the board gets more and more crowded (unless you are really good...) and making objects disappear gets harder and harder. The game is over, when the board is full. Authors Basic usage Customizing the Game DOCUMENT AND MODIFIED VERSIONS OF THE DOCUMENT ARE PROVIDED UNDER THE TERMS OF THE GNU FREE DOCUMENTATION LICENSE WITH THE FURTHER UNDERSTANDING THAT: <placeholder-1/> DOCUMENT IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE DOCUMENT OR MODIFIED VERSION OF THE DOCUMENT IS FREE OF DEFECTS MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY, ACCURACY, AND PERFORMANCE OF THE DOCUMENT OR MODIFIED VERSION OF THE DOCUMENT IS WITH YOU. SHOULD ANY DOCUMENT OR MODIFIED VERSION PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL WRITER, AUTHOR OR ANY CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. THIS DISCLAIMER OF WARRANTY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY DOCUMENT OR MODIFIED VERSION OF THE DOCUMENT IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER; AND Each theme consists of a single PNG image file. This image consists of an array of seven rows, corresponding to the different object colors/shapes, and four columns, each of which is a frame in the animation of the object. The actual size of the image is irrelevant, it will be rescaled, but the sub-images must form a four by seven grid to get the correct effect. When using SVG remember that the image size is the page size, if your grid does not fill the page try shrinking the page size. Use an existing theme as a guide. Emese Emese Kovács Emese Kovács <email>emese@gnome.hu</email> Feedback Five or More Five or More Manual V2.8 GNOME Documentation Project Game over! Height How to create new themes If you start using the mouse again, the cursor will disappear. The cursor will reappear if you use the arrow keys again. Instead of using the mouse you can use the keyboard to play <application>Five or More</application>. To use the keyboard, hit an arrow key and a cursor will appear near the center of the board. The arrow keys will move the cursor. When the cursor is over the object you want to move, use the spacebar to select it. Then shift the cursor to the object's destination and hit the space bar again to move the object. Introduction Kovács Large License Many of the names used by companies to distinguish their products and services are claimed as trademarks. Where those names appear in any GNOME documentation, and the members of the GNOME Documentation Project are made aware of those trademarks, then the names are in capital letters or initial capital letters. Medium Number of Types Number of objects Objects per Turn Permission is granted to copy, distribute and/or modify this document under the terms of the GNU Free Documentation License (GFDL), Version 1.1 or any later version published by the Free Software Foundation with no Invariant Sections, no Front-Cover Texts, and no Back-Cover Texts. You can find a copy of the GFDL at this <ulink type="help" url="ghelp:fdl">link</ulink> or in the file COPYING-DOCS distributed with this manual. Playing <application>Five or More</application> is easy. At startup, you find yourself with five objects randomly positioned on the board. Each turn, you are allowed to move one object. You can move it anywhere on the board as long as there is a clear path. Afterward, the computer drops more objects at random positions (the exact number depends on the size of the game). You can preview the next objects to be dropped in the upper right corner of the game's window. If you manage to align five objects of the same shape and color, they disappear and you are given an extra move before more objects drop. Playing Five or More Playing the Game Preferences Score given Scoring in Five or More September 2004 Size Small Starting <application>Five or More</application> presents you with the <interface>game board</interface> and a new game starts. What it looks like is shown in <xref linkend="mainwindow-fig"/>. The <guimenu>Board Size</guimenu> section lets you select the size of the board. Each size has a slightly different balance: larger sizes give you more space to move, but more objects arrive each turn. The parameters for each size are given in <xref linkend="table-sizes"/>. The main window when you start a game. This manual describes version 2.12 of Five or More. This manual is part of a collection of GNOME manuals distributed under the GFDL. If you want to distribute this manual separately from the collection, you can do so by adding a copy of the license to the manual, as described in section 6 of the license. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the <citetitle>GNU General Public License</citetitle> for more details. This program is free software; you can redistribute it and/or modify it under the terms of the <citetitle>GNU General Public License</citetitle> as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version. To make a move, click on one of the objects. When an object is activated it starts rotating (or doing something else depending on the theme, but let us not anticipate…). You can then select its destination by clicking an empty square. If the path to the destination is clear the activated item travels there. If the destination square is occupied by another object nothing will be moved and the selection will be transferred to the new object. If the path is not clear (you cannot jump objects over obstacles!) the application prints a warning in the status bar (bottom of the window) and the original object remains active. To play Five or More one must align colored pieces as the board gets filled with randomly placed pieces. When five or more pieces of the same color get lined up, they disappear. The game ends when the board gets filled up all the way. To report a bug or make a suggestion regarding the <application>Five or More</application> application or this manual, follow the directions in the <ulink url="ghelp:gnome-feedback" type="help">GNOME Feedback Page</ulink>. To run <application>Five or More</application>, select <guimenuitem>Five or More</guimenuitem> from the <guisubmenu>Games</guisubmenu> submenu of the <guimenu>Main Menu</guimenu>, or type <command>glines</command> on the command line. UNDER NO CIRCUMSTANCES AND UNDER NO LEGAL THEORY, WHETHER IN TORT (INCLUDING NEGLIGENCE), CONTRACT, OR OTHERWISE, SHALL THE AUTHOR, INITIAL WRITER, ANY CONTRIBUTOR, OR ANY DISTRIBUTOR OF THE DOCUMENT OR MODIFIED VERSION OF THE DOCUMENT, OR ANY SUPPLIER OF ANY OF SUCH PARTIES, BE LIABLE TO ANY PERSON FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES OF ANY CHARACTER INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF GOODWILL, WORK STOPPAGE, COMPUTER FAILURE OR MALFUNCTION, OR ANY AND ALL OTHER DAMAGES OR LOSSES ARISING OUT OF OR RELATING TO USE OF THE DOCUMENT AND MODIFIED VERSIONS OF THE DOCUMENT, EVEN IF SUCH PARTY SHALL HAVE BEEN INFORMED OF THE POSSIBILITY OF SUCH DAMAGES. Using the Keyboard Width You can configure <application>Five or More</application> by clicking on <guimenuitem>Preferences</guimenuitem> in the applications <guimenu>Settings</guimenu> menu. The following <interface>dialog</interface> will appear: You can select different object styles and background colors in the <guimenu>Themes</guimenu> section. If you find it difficult distinguishing the colours, try the shapes theme. You can speed up the movement of the objects by checking the Use fast moves box in the <guimenu>General</guimenu> preferences section. emese@gnome.hu translator-credits Project-Id-Version: glines-mixer
POT-Creation-Date: 2008-01-03 18:49+0000
PO-Revision-Date: 2008-07-27 23:11+0100
Last-Translator: Joan Duran <jodufi@gmail.com>
Language-Team: Catalan <tradgnome@softcatala.org>
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
Plural-Forms: nplurals=2; plural=(n != 1);
 10 108 11 12 13 138 14 15 172 18 20 2000-2002 210 28 3 42 5 6 7 8 82 9 Manual del <application>Cinc o més</application> El <application>Cinc o més</application> està inclòs en el paquet <filename>gnome-games</filename>, el qual és part de l'entorn d'escriptori GNOME. Aquest document descriu la versió 2.8 del <application>Cinc o més</application>. En Róbert Szókovács (<email>szo@appaloosacorp.hu</email>) i en Szabolcs Bán (<email>shooby@gnome.hu</email>) han escrit el <application>Cinc o més</application>. En Emese Kovács (<email>emese@gnome.hu</email>) ha escrit aquest manual. Per a més informació de com fer algun suggeriment o informar d'un error vegeu el <ulink url="ghelp:gnome-feedback" type="help">document de comentaris</ulink>. El <application>Cinc o més</application> és el la versió per a GNOME del que va ser un joc popular del Windows anomenat <application>Color Lines</application>. L'objectiu és alinear, el més sovint possible, cinc objectes o més del mateix color i forma fent-los desaparèixer. Jugueu el màxim possible i sigueu el número 1 en les puntuacions màximes. @@image: 'figures/glines-gameover.png'; md5=949d09a01ee02944bf3f278ce1ebc2ba @@image: 'figures/glines-startgame.png'; md5=e6e58f72dc4802d89a45a7726f4c7fd7 @@image: 'figures/preferences.png'; md5=745336aefd6ade740f6064bdf4121379 S'inclou una còpia de la <citetitle>Llicència pública general de GNU</citetitle> en l'apèndix de la <citetitle>Guia d'usuaris del GNOME</citetitle>. També podeu obtenir una copia de la <citetitle>Llicència pública general de GNU</citetitle> de la Free Software Foundation visitant el <ulink type="http" url="http://www.fsf.org">seu lloc web</ulink> o escrivint a <address> Free Software Foundation, Inc. <street>59 Temple Place</street> - Suite 330 <city>Boston</city>, <state>MA</state><postcode>02111-1307</postcode><country>USA</country></address> Una partida del <application>Cinc o més</application> comença amb alguns objectes aleatoris en el tauler. Qualsevol canvi que realitzeu s'aplicarà instantàniament i es recordarà per a la següent vegada que executeu el <application>Cinc o més</application>. Obtindreu punts en fer desaparèixer objectes. El punts que obteniu depenen del número d'objectes que heu pogut alinear (vegeu <xref linkend="glines-table1"/>). Es mostra la puntuació en la cantonada superior dreta de la finestra. A mesura que la partida avança, el tauler s'omple més i més (a no ser que sigueu molt bo...) i fer desaparèixer els objectes és més i més difícil. La partida s'acaba quan el tauler està ple. Autors Utilització bàsica Personalització el joc EL DOCUMENT I LES VERSIONS MODIFICADES DEL DOCUMENT S'OFEREIXEN SOTA ELS TERMES DE LA LLICÈNCIA DE DOCUMENTACIÓ LLIURE DE GNU, TENINT EN COMPTE QUE: <placeholder-1/> EL DOCUMENT S'OFEREIX «TAL COM ÉS», SENSE CAP TIPUS DE GARANTIA, NI EXPLÍCITA NI IMPLÍCITA; AIXÒ INCLOU, SENSE LIMITAR-S'HI, LES GARANTIES QUE EL DOCUMENT O LA VERSIÓ MODIFICADA DEL DOCUMENT NO TINGUI DEFECTES, SIGUI COMERCIALITZABLE, SIGUI ADEQUAT PER A UN ÚS CONCRET O NO INFRINGEIXI CAP LLEI. TOT EL RISC PEL QUE FA A LA QUALITAT, EXACTITUD I RENDIMENT DEL DOCUMENT O LA VERSIÓ MODIFICADA DEL DOCUMENT ÉS VOSTRE. EN CAS QUE EL DOCUMENT RESULTÉS DEFECTUÓS EN QUALSEVOL ASPECTE, VÓS (NO PAS L'ESCRIPTOR INICIAL, L'AUTOR O CAP ALTRE COL·LABORADOR) ASSUMIU TOT EL COST DE MANTENIMENT, REPARACIÓ O CORRECCIÓ. AQUESTA RENÚNCIA DE GARANTIA CONSTITUEIX UNA PART ESSENCIAL D'AQUESTA LLICÈNCIA. NO S'AUTORITZA L'ÚS DE CAP DOCUMENT O VERSIÓ MODIFICADA DEL DOCUMENT EXCEPTE SOTA AQUESTA RENÚNCIA DE GARANTIA; I  Cada tema consisteix en una fitxer d'imatge PNG. Aquesta imatge consisteix en una matriu de set files, corresponents als diferents objectes de colors/formes, i quatre columnes, cada un de les quals és un fotograma de l'animació de l'objecte. La mida de la imatge és irrellevant, serà redimensionada, però les sub-imatges han de formar una graella de quatre per set per a aconseguir l'efecte correcte. Quan utilitzeu un SVG recordeu que la mida de la imatge és la mida de la pàgina, si la graella no omple la pàgina proveu d'ajustar-ne la mida. Utilitzeu un tema existent com a guia. Emese Emese Kovács Emese Kovács <email>emese@gnome.hu</email> Comentaris Cinc o més Manual del Cinc o més (versió 2.8) Projecte de documentació del GNOME Final de la partida. Alçada Com crear temes nous Si torneu a utilitzar el ratolí, el cursor desapareixerà. El cursor tornarà a aparèixer si torneu a utilitzar les tecles de cursor. Podeu jugar al <application>Cinc o més</application> amb el teclat en comptes del ratolí. Per a utilitzar-lo, premeu una tecla del cursor i apareixerà un cursor prop del centre del tauler. Les tecles de cursor mouran el cursor. Quan el cursor està sobre de l'objecte que voleu moure, utilitzeu la barra espaiadora per a seleccionar-lo. Llavors desplaceu el cursor al destí de l'objecte i premeu la barra espaiadora per a moure'l. Introducció Kovács Gran Llicència Molts dels noms que les empreses utilitzen per a distingir els seus productes i serveis es consideren marques comercials. Quan aquests noms apareguin en qualsevol documentació del GNOME, si els membres del Projecte de documentació del GNOME han estat avisats pel que fa a les marques, els noms apareixeran en majúscules o amb les inicials en majúscules. Mitjà Número de tipus Número d'objectes Objectes per torn Teniu permís per a copiar, distribuir i/o modificar aquest document, sota els termes de la Llicència de documentació lliure GNU (GFDL), versió 1.1 o qualsevol versió publicada posteriorment per la Free Software Foundation, sense seccions invariants, sense texts de portada i sense texts de contraportada. Podeu trobar una còpia de la GFDL en aquest <ulink type="help" url="ghelp:fdl">enllaç</ulink> o en el fitxer COPYING-DOCS distribuït amb aquest manual. Jugar al <application>Cinc o més</application> és fàcil. En iniciar us trobareu amb cinc objectes en el tauler en posicions aleatòries. A cada torn podreu moure un objecte. El podeu moure a qualsevol part del tauler mentre hi hagi un camí lliure. Després, l'ordinador deixarà més objectes en posicions aleatòries (el número exacte depèn de la mida del tauler). En la cantonada superior dreta de la finestra del joc podeu previsualitzar els objectes següents a deixar-se. Si aconseguiu alinear cinc objectes de la mateixa forma i color, desapareixeran i tindreu un moviment addicional abans que es deixin més objectes. Jugar al Cinc o més Jugar al joc Preferències Puntuació obtinguda Puntuació en el Cinc o més Stembre de 2004 Mida Petit En iniciar el <application>Cinc o més</application> us trobareu amb el <interface>tauler de joc</interface> i s'iniciarà una partida nova. Això s'assembla al que es mostra a <xref linkend="mainwindow-fig"/>. La secció <guimenu>Mida del tauler</guimenu> us permet seleccionar la mida del tauler. Cada mida té un equilibri diferents: les mides grans us donen més espai per a moure-us, però us arribaran més objectes cada torn. El paràmetres de cada mida estan indicats a la <xref linkend="table-sizes"/>. La finestra principal quan inicieu una partida. Aquest manual descriu la versió 2.12 del Cinc o més. Aquest manual forma part d'una col·lecció de manuals del GNOME distribuïts sota la GFDL. Si voleu distribuir aquest manual independentment de la col·lecció, podeu fer-ho afegint una còpia de la llicència al manual, tal com es descriu a la secció 6 de la llicència. Aquest programa es distribueix amb l'esperança de que sigui útil, però SENSE CAP GARANTIA; fins i tot sense la garantia implícita de COMERCIALITZACIÓ o ADEQUACIÓ A UN ÚS CONCRET. Per a més detalls vegeu la <citetitle>Llicència pública general de GNU</citetitle>. Aquest programa és programari lliure; podeu redistribuir-lo i/o modificar-lo sota els termes de la <citetitle>Llicència pública general de GNU</citetitle> tal com publica la Free Software Foundation; tant en la versió 2 de la llicència, o (a la vostra opció) una versió posterior. Per a fer un moviment, feu clic en un dels objectes. Quan l'objecte estigui activat començarà a donar voltes (o alguna altra cosa depenent del tema, però no ens avançem…). Llavors podreu seleccionar la seva destinació fent clic a una casella buida. Si la ruta fins al destí està lliure, l'element activat hi viatjarà. Si la casella de destí està ocupada per un altre objecte, no es mourà res i la selecció serà transferida a l'objecte nou. Si la ruta no està lliure (no podeu fer saltar els objectes sobre els obstacles) l'aplicació mostrarà un avís en la barra d'estat (part inferior de la finestra) i l'objecte original es mantindrà actiu. Per a jugar al cinc o més s'han d'alinear les fitxes del mateix color mentre el tauler s'omple amb fitxes ubicades aleatòriament. Quan s'alineen cinc o més fitxes del mateix color, aquestes desapareixen. La partida acaba quan el tauler s'omple completament. Per a informar d'un error o fer algun suggeriment quant al <application>Cinc o més</application> o aquest manual, seguiu les indicacions a la <ulink url="ghelp:gnome-feedback" type="help">pàgina de comentaris del GNOME</ulink>. Per a executar el <application>Cinc o més</application>, seleccioneu <guimenuitem>Cinc o més</guimenuitem> en el submenú <guimenu>Jocs</guimenu> del <guimenu>Menú principal</guimenu> o introduïu <command>glines</command> en la línia d'ordres. EN CAP CAS I SOTA CAP INTERPRETACIÓ LEGAL, JA SIGUI PER AGREUJAMENT (INCLOENT-HI LA NEGLIGÈNCIA), CONTRACTE O ALTRE CAS, L'AUTOR, L'ESCRIPTOR ORIGINAL, QUALSEVOL DELS COL·LABORADORS O DISTRIBUÏDORS DEL DOCUMENT O UNA VERSIÓ MODIFICADA DEL DOCUMENT NI CAP PROVEÏDOR D'AQUESTES PARTS NO SERAN RESPONSABLES DAVANT DE NINGÚ PER CAP DANY DIRECTE, INDIRECTE, ESPECIAL, ACCIDENTAL O CONSECUTIU DE QUALSEVOL TIPUS; AIXÒ INCLOU, SENSE LIMITAR-S'HI, ELS DANYS PER PÈRDUA DE CLIENTS, INTERRUPCIONS DE LA FEINA, FALLADA O MALFUNCIONAMENT DE L'ORDINADOR, O QUALSEVOL ALTRA PÈRDUA O DANY RELACIONAT AMB L'ÚS DEL DOCUMENT I LES VERSIONS MODIFICADES DEL DOCUMENT, FINS I TOT SI S'HA INFORMAT AQUESTA PART DE LA POSSIBILITAT D'AQUESTS DANYS. Utilització del teclat Amplada Podeu configurar el <application>Cinc o més</application> si feu clic a <guimenuitem>Preferències</guimenuitem> en el menú de l'aplicació <guimenu>Paràmetres</guimenu>. Apareixerà el <interface>diàleg</interface> següent: Podeu seleccionar diferents estils d'objectes i de colors de fons en la secció <guimenu>Temes</guimenu>. Si trobeu que és difícil distingir els colors, proveu el tema «shapes». Podeu accelerar el moviment dels objectes activant la casella de verificació «Utilitza moviments ràpids» en la secció de preferències <guimenu>General</guimenu>. emese@gnome.hu Joan Duran <jodufi@gmail.com>, 2007, 2008 