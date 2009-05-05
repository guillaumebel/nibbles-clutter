��    {      �  �   �      h
     i
     m
     q
     u
     y
     ~
     �
     �
     �
  '   �
    �
  T  �  F   <  B   �  B   �  C   	  B   M  D   �  C   �      2   7  �   j  	   .  
   8     C     K  \  S  .   �  $   �  �       �     �     �  l   �     G     L  /   Y     �     �  
   �     �     �     �     �     �               )  �  �  �  O  ]   �  
   B     M  �   Z  
  �     �          &      <"  e   D"  L   �"  7  �"  �   /$     %     #%  ;  )%  �   e&  )  '  M  .)  �  |*     (,     D,     R,     r,     �,  %   �,     �,      �,  $   -     --  	   <-     F-     T-  �  f-  �   -/  $   �/  9    0  3   Z0  �   �0  .   51  �   d1  c   +2  O   �2  ,   �2  �   3  �   
4  �  �4  ?   j6    �6  j  �7  �   9  U   �9  �   1:  �   �:    �;     �<     �<  O   �<  �   9=  �   >  �   �>  �  �?  *   JB     uB    �B    �C  �  �E    �G     �H  W  �H     J     %J  5  8J     nK     rK     vK     zK     ~K     �K     �K     �K     �K  +   �K  Q  �K  {  ?N     �O     �O     �O     �O     �O     �O     �O  ,  �O  G   R  �   ZR  
   1S     <S     DS     KS  R  SS  .   �T  '   �T  �   �T  �  �U     )Y     ,Y  �   KY     �Y     �Y  /   �Y     Z     !Z     *Z     6Z     MZ      kZ     �Z     �Z     �Z     �Z  |   �Z  �  Q[  �  3]  c   _     u_     �_  �   �_    I`     ga     �a  �  �a     Id  t   Qd  R   �d  a  e  0  {f     �g     �g  c  �g  �   &i  �  �i  c  }l  �  �m  "   �o     �o  '   �o     �o      p  -   4p     bp  '   p  "   �p     �p     �p     �p      q  �  q  �   �r  2   �s  6   �s  4   1t  �   ft  8   u  �   Nu  �   :v  b   �v  8   6w    ow  8  }x  �  �y  F   �{  G  �{  �  }  �   �~  Z   �  �   �    ��    ́     �     ��  e   �  �   k�  �   [�  �   P�  "  �  6   *�     a�  -  o�  [  ��  h  ��  >  b�  %   ��  u  ǐ     =�  0   Q�     !   8   #   E   4      
   W               ?       b       /      0             w   \   )   o   i   A   "          N   X   :   G      _   M   &   d   '   5           <   {               l      U   h           2   s               q   j   x       R   f      +   @                 7   p   v       9   Y                             Q   -       k   n   u   T   3   ;       J   *      ,       L   P   F       (   ^       r      S   g       .       D   C   V       ]   K   m       c   >   a       $       H      z   	   [                                   =      y   `      I      6   1       t   Z          O   e       B      %                0.1 0.2 0.3 1.0 2000 4 February 2000 5 February 2000 7 April 2000 7 February 2000 <application>Iagno</application> Manual <application>Iagno</application> is a computer version of the game Reversi, more popularly called Othello. <application>Iagno</application> is a two player strategy game similar to Go. The board is 8 by 8 with tiles that are black on one side and white on the other side. The object of <application>Iagno</application> is to flip as many of your opponent's tiles to your color as possible without your opponent flipping your tiles. This is done by trapping your opponent's tiles between two tiles of your own color. <application>Iagno</application> was written by Ian Peters (<email>itp@gnu.org</email>). This manual was written by Eric Baudais (<email>baudais@okstate.edu</email>). To report a bug or make a suggestion regarding this application or this manual, follow the directions in this <ulink url="ghelp:gnome-feedback" type="help">document</ulink>. @@image: 'figures/ANIMATION.png'; md5=9028b27e2afa57856d6590908e027ba3 @@image: 'figures/BLOCK.png'; md5=f0b3c08a476b1adbfb34e132a40d5526 @@image: 'figures/FIRST.png'; md5=99b4e5d5a6d570feba3d5db287a6020a @@image: 'figures/PLAYER.png'; md5=6aa2ac5a1cfafc497fa6c7c3f829346c @@image: 'figures/START.png'; md5=ed4b069d10fb75410a1171d5c6c29d66 @@image: 'figures/connect.png'; md5=9a03bf23f04eacfd6feb9de823d6e321 @@image: 'figures/tables.png'; md5=96e520cee7fdbee56636794251a52b0d A copy of the <citetitle>GNU General Public License</citetitle> is included as an appendix to the <citetitle>GNOME Users Guide</citetitle>. You may also obtain a copy of the <citetitle>GNU General Public License</citetitle> from the Free Software Foundation by visiting <ulink type="http" url="http://www.fsf.org">their Web site</ulink> or by writing to <address> Free Software Foundation, Inc. <street>59 Temple Place</street> - Suite 330 <city>Boston</city>, <state>MA</state><postcode>02111-1307</postcode><country>USA</country></address> Added a footnote to why section. Fixed some typos. All the animation option are in the <menuchoice><guimenu>Settings</guimenu><guisubmenu>Preferences </guisubmenu></menuchoice> menus with the <guimenuitem>Appearance</guimenuitem> tab highlighted. Animation Appearance Authors Baudais Black goes first and places a tile on the board such that it traps white's tiles between two of black's tiles. When black places a tile on the board all of white's tiles between black's newly placed tile and another black tile, in a horizontal, vertical, or diagonal row, are flipped over and become black, as shown in <xref linkend="first-shot"/>. Claude Paroz <email>claude@2xlibre.net</email> Connecting to a Iagno network server DOCUMENT AND MODIFIED VERSIONS OF THE DOCUMENT ARE PROVIDED UNDER THE TERMS OF THE GNU FREE DOCUMENTATION LICENSE WITH THE FURTHER UNDERSTANDING THAT: <placeholder-1/> DOCUMENT IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE DOCUMENT OR MODIFIED VERSION OF THE DOCUMENT IS FREE OF DEFECTS MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY, ACCURACY, AND PERFORMANCE OF THE DOCUMENT OR MODIFIED VERSION OF THE DOCUMENT IS WITH YOU. SHOULD ANY DOCUMENT OR MODIFIED VERSION PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT THE INITIAL WRITER, AUTHOR OR ANY CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR OR CORRECTION. THIS DISCLAIMER OF WARRANTY CONSTITUTES AN ESSENTIAL PART OF THIS LICENSE. NO USE OF ANY DOCUMENT OR MODIFIED VERSION OF THE DOCUMENT IS AUTHORIZED HEREUNDER EXCEPT UNDER THIS DISCLAIMER; AND EB Enable/Disable Sound Enabling this option does not force the computer player to move and does not reduce the level of difficulty. Eric Eric Baudais Eric Baudais <email>baudais@okstate.edu</email> February 2008 Feedback First Move First draft completed. Flip Final Results GNOME Documentation Project Game Settings Iagno Iagno Manual V2.8 Iagno Manual V2.9 Iagno also features <link linkend="network-games">multiplayer</link> support with two human players in hotseat or network mode. Iagno is the two player strategy game of Othello, which is also known as Reversi and is similar to Go. The pieces are tiles that are black on one side and white on the other and the objective is for the player to flip his/her opponent's tiles to his/her color, while keeping the opponent from doing the same. Once the board is filled with tiles, the winner is the player with the most of his/her color tiles on the board. Iagno support networked multiplayer games which is provided by <ulink type="http" url="http://www.ggzgamingzone.org">GGZ Gaming Zone</ulink>. By connecting to a Iagno server on the Internet, you can challenge other players in multiplayer games. For news, updates and a list of servers to connect to, see the <ulink type="http" url="http://www.gnome.org/projects/gnome-games/">gnome-games website</ulink>. Iago is Othello's nemesis in the play <citetitle>Othello </citetitle> by William Shakespeare. Important: Introduction It is possible to chat with other players in network games. Ask for advice or help playing the games, but please be polite against other players. It is possible to connect to a server with either a guest account or a normal registered account. Guest accounts allows you to anonymously login, while a normal login account allows you to reserve your own username which is protected by the password that you choose. Joining a game room Known Bugs and Limitations Later his buddies came over and like all programmers he showed off his masterpiece. They all laughed in his face and told Ian that his game was worthless and a pitiful attempt to be a true hacker. After Ian kicked them out he decided to rename his program Iago <placeholder-1/> for the evil which had manifested itself due to his game. Yet it still didn't sound right. All GNOME programs have some part of GNOME in it, Ian thought. After much thought he decided on the name we have come to cherish, <application>Iagno </application>. License Licensed the Iagno Manual under the FDL. Updated the screenshots to the GDP standard theme, MicroGUI. Made the doc compliant with the GDP template. Clarified the playing section. Many of the names used by companies to distinguish their products and services are claimed as trademarks. Where those names appear in any GNOME documentation, and the members of the GNOME Documentation Project are made aware of those trademarks, then the names are in capital letters or initial capital letters. Multiplayer Iagno games have pretty much the same rules as normal Iagno games, except that you are now playing against human players. This means that other strategies might possibly be better than when playing against AI players. Network Games Note: On a stormy night, like those depicted so much in England, a lonely programmer was sitting in front of his computer screen. Ian had finished his masterpiece in gaming. It was a fully interactive game of Othello and he decided to play a pun with the name and called it gnothello for the game was written using GNOME. Once a player has won, the game ends and you can return to the initial network game screen. Then you can play yet another game of addictive Iagno multiplayer! Once you have successfully connected to a server, you can choose which game room to join. To play a multiplayer game of Iagno, select the Iagno room. If you want to host your own game, then click on the Launch button. This creates a new table where other players can participate in a game against you. If there are any other games already started, then you can double-click on an existing game table to join it. The list of game tables on the right shows you the number of available seats, which means the number of players that can join the game table. Once you have successfully joined a game table, then you have to wait until enough players have joined the table. The <guimenuitem>Players List</guimenuitem> menu item allows you to see a list of the players who have joined the game. The game will begin immediately when the total number of players in the the game have been reached. Permission is granted to copy, distribute and/or modify this document under the terms of the GNU Free Documentation License (GFDL), Version 1.1 or any later version published by the Free Software Foundation with no Invariant Sections, no Front-Cover Texts, and no Back-Cover Texts. You can find a copy of the GFDL at this <ulink type="help" url="ghelp:fdl">link</ulink> or in the file COPYING-DOCS distributed with this manual. Player Color and Difficulty Playing Iagno Playing multiplayer Iagno games Quick Moves Option Screenshot of black blocking. Screenshot of the animation settings. Screenshot of the first move. Screenshot of the game settings. Screenshot of the starting position. September 2004 Show Grid Stagger Flips Starting Position The <guimenuitem>Guest Login</guimenuitem> option should be selected if you want an anonymous guest account. If you want to create a new login account, then select the <guimenuitem>First-time Login</guimenuitem> option, with the username, password and email of your choice. If you have already created your account, you can connect by selecting the <guimenuitem>Normal Login</guimenuitem> option, and enter the username and password that you have chosen. The Iagno connection dialog is shown initially when starting a new network game. This dialog allows you to select a server to connect to, and the username that you will have while connected to that server. The Iagno network connection dialog. The Preferences menu with the Appearance tab highlighted. The Preferences menu with the Game tab highlighted. The board starts with four tiles arranged in a box on the middle of the board. The tiles have the same color at each corner, as shown in <xref linkend="start-shot"/>. The computer player for Iagno is easy to beat. The last option of the <guimenuitem>Game</guimenuitem> tab in the <guilabel>Preferences</guilabel> dialog allows you to enable or disable all sounds when playing in <application>Iagno</application>. There are two tile sets which look the same, but have different animations -- woodtrim and classic. This Iagno network dialog allows you to join a game room to find other players. This manual describes version 2.20 of Iagno. This manual is part of a collection of GNOME manuals distributed under the GFDL. If you want to distribute this manual separately from the collection, you can do so by adding a copy of the license to the manual, as described in section 6 of the license. This option causes captured tiles along a row, column, or diagonal to flip one after another, like dominoes, instead of all flipping simultaneously. It has no effect on game play, but looks cool. This option controls the amount of animation present when each tile is flipped, using radio buttons. Selecting the option <guibutton>none </guibutton> will cause the tiles to change color instantaneously. Selecting the <guibutton>option</guibutton> partial will cause the animation to flip the tiles slowly. Selecting the <guibutton>complete </guibutton> option will cause the tiles to flip slowly and smoothly. This option does not affect the outcome of the game in any way. This option is in the <guimenu>Settings</guimenu> menu. Then go to the <guisubmenu>Preferences</guisubmenu> submenu in the <guimenuitem>Game </guimenuitem> tab and it is initially off. When enabled this option makes the computer player place its tiles faster. This option lets you choose the type of board and tiles which are displayed. If you want to make your own tile sets use the classic tile set as a template and change the texture and colors for different effects. When you make a new tile set that works with Iagno send it to Ian Peters <email>itp@gnu.org</email> and he will include it with the next distribution. This option moves all the black tiles to the top of the board and all the white tiles to the bottom of the board at the end of the game. This allows you to visually see the outcome of the game. This option should be enabled for beginning players to see the gameplay more clearly. This option will display a grid on top of the playing surface. The grid is useful to display because it shows the player exactly where they are placing each tile. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the <citetitle>GNU General Public License</citetitle> for more details. This program is free software; you can redistribute it and/or modify it under the terms of the <citetitle>GNU General Public License</citetitle> as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version. Tile Set Tip: To connect to a server, click on the <guimenuitem>Connect</guimenuitem> button. To report a bug or make a suggestion regarding the <application>Iagno</application> application or this manual, follow the directions in the <ulink url="ghelp:gnome-feedback" type="help">GNOME Feedback Page</ulink>. To run <application>Iagno</application>, select <guimenuitem>Iagno</guimenuitem> from the <guisubmenu>Games</guisubmenu> submenu of the <guimenu>Main Menu</guimenu>, or type <command>iagno</command> on the command line. To start the multiplayer game of Iagno, select <menuchoice><guimenu>Game</guimenu><guimenuitem>Network Game</guimenuitem></menuchoice> on the Iagno menu. UNDER NO CIRCUMSTANCES AND UNDER NO LEGAL THEORY, WHETHER IN TORT (INCLUDING NEGLIGENCE), CONTRACT, OR OTHERWISE, SHALL THE AUTHOR, INITIAL WRITER, ANY CONTRIBUTOR, OR ANY DISTRIBUTOR OF THE DOCUMENT OR MODIFIED VERSION OF THE DOCUMENT, OR ANY SUPPLIER OF ANY OF SUCH PARTIES, BE LIABLE TO ANY PERSON FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES OF ANY CHARACTER INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF GOODWILL, WORK STOPPAGE, COMPUTER FAILURE OR MALFUNCTION, OR ANY AND ALL OTHER DAMAGES OR LOSSES ARISING OUT OF OR RELATING TO USE OF THE DOCUMENT AND MODIFIED VERSIONS OF THE DOCUMENT, EVEN IF SUCH PARTY SHALL HAVE BEEN INFORMED OF THE POSSIBILITY OF SUCH DAMAGES. Waiting for other players to join the game What is Iagno? When creating a new table for Iagno games, a preference dialog is displayed which allows you to customize the game, such as set the minimum number of players for the game. Once the total number of seats have been taken, then no more players are allowed to join that game table. When there is tile trapped between two tiles, of an identical color in the same row, the player captures tiles until another tile of the player's color is encountered, as shown in <xref linkend="block-shot"/>. The player can only take tiles that are a direct result of the placing a tile on the board. Once a tile is placed on the board the player cannot remove it and place the tile somewhere else. If a player cannot move they forfeit their turn. The game ends when it is no longer possible for either player to move. <placeholder-1/> When you start Iagno the human player plays black and the computer player plays white with the first level of difficulty. Using the <guimenuitem>Game</guimenuitem> tab in the <guisubmenu>Preferences</guisubmenu> dialog you can set the type of player for each color. Human is of course you, level one is the easiest computer player and level three is the hardest. You can set both to human to play against a friend or set both to computer to watch the computer play itself. White cannot capture the black tile in the center of the middle column because there are white tiles blocking the ends of that column. If it where blacks turn instead, black could place a tile at the top of the column but would only flip the first of the two white tiles. Why is Othello called Iagno? You're probably wondering why this game is called <application>Iagno</application> instead of Othello or even gnothello. The answer is complicated, and involves things like trademarks and lawyers and whatnot. Instead of leaving it at that, I'll tell you a long and complicated lie about the origin of the name <application>Iagno</application>. baudais@okstate.edu translator-credits Project-Id-Version: manuale iagno
POT-Creation-Date: 2008-04-22 02:56+0000
PO-Revision-Date: 2008-04-29 11:59+0100
Last-Translator: Alessio Treglia <quadrispro@ubuntu-it.org>
Language-Team: Italiano <tp@lists.linux.it>
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
 0.1 0.2 0.3 1.0 2000 4 febbraio 2000 5 febbraio 2000 7 aprile 2000 7 febbraio 2000 Manuale di <application>Iagno</application> <application>Iagno</application> è una versione per computer del gioco Reversi, più conosciuto con il nome di Othello. <application>Iagno</application> è un gioco di strategia a due giocatori simile a Go. Sulla scacchiera, di dimensioni 8 per 8, vengono poste le pedine, di colore bianco su un lato e nero sull'altro. L'obiettivo di <application>Iagno</application> è capovolgere al proprio colore il maggior numero possibile di pedine del proprio avversario senza che egli faccia lo stesso. È possibile fare questo racchiudendo le pedine dell'avversario fra altre due del proprio colore. <application>Iagno</application> è stato scritto da Ian Peters (<email>itp@gnu.org</email>). Questo manuale è stato scritto da Eric Baudais (<email>baudais@okstate.edu</email>). Per segnalare un problema o inviare suggerimenti su questa applicazione o su questo manuale, seguire le istruzioni contenute in questo <ulink url="ghelp:gnome-feedback" type="help">documento</ulink>. fatto fatto fatto fatto fatto fatto fatto Una copia della <citetitle>GNU General Public License</citetitle> è inclusa nell'appendice del <citetitle>Manuale dell'utente GNOME</citetitle>. Inoltre è possibile ottenere una copia della <citetitle>GNU General Public License</citetitle> dalla Free Software Foundation visitando <ulink url="http://www.fsf.org" type="http">il sito web</ulink> oppure scrivendo a <address> Free Software Foundation, Inc. <street>51 Franklin Street, Fifth Floor</street><city>Boston</city>, <state>MA</state><postcode>02110-1301</postcode><country>USA</country></address> Aggiunta una nota a piè di pagina. Corretti alcuni errori tipografici. Tutte le opzioni dell'animazione sono raggiungibili dal menù <menuchoice><guimenu>Impostazioni</guimenu><guisubmenu>Preferenze</guisubmenu></menuchoice> nella scheda <guimenuitem>Aspetto</guimenuitem> evidenziata. Animazione Aspetto Autori Baudais Il nero gioca per primo e posiziona una pedina sulla scacchiera in modo da racchiudere quelle del bianco fra due delle sue. Tutte le pedine del bianco racchiuse in riga orizzontale, verticale o diagonale dall'ultima pedina appena posizionata e un'altra nera, vengono girate e diventano nere, come mostrato in <xref linkend="first-shot"/>. Claude Paroz <email>claude@2xlibre.net</email> Collegarsi a un server di rete di Iagno QUESTO DOCUMENTO E LE SUE VERSIONI MODIFICATE SONO FORNITI IN BASE AI TERMINI DELLA GNU FREE DOCUMENTATION LICENSE, SOTTO INTESO CHE: <placeholder-1/> IL DOCUMENTO VIENE FORNITO SENZA GARANZIE DI ALCUN TIPO,ESPLICITE O IMPLICITE, INCLUSE, MA SENZA LIMITAZIONE, LE GARANZIE ATTESTANTI CHE IL DOCUMENTO O LE SUE VERSIONI MODIFICATE SIANO PRIVI DI DIFETTI, COMMERCIALIZZABILI, IDONEI A UN DETERMINATO SCOPO O CHE NON VIOLINO DIRITTI DI TERZI. SI DECLINA QUALUNQUE RESPONSABILITÀ RIGUARDO AI RISCHI INERENTI LA QUALITÀ, L'ACCURATEZZA E LE PRESTAZIONI DEL DOCUMENTO O DI UNA SUA VERSIONE MODIFICATA. QUALORA UN DOCUMENTO O UNA SUA VERSIONE MODIFICATA DOVESSERO PRESENTARE QUALUNQUE TIPO DI DIFETTO,IL COSTO DI EVENTUALI INTERVENTI DI ASSISTENZA, RIPARAZIONE O CORREZIONE SARÀ A CARICO DELL'UTENTE (NON DEL REDATTORE INIZIALE, DELL'AUTORE O DI ALTRI COLLABORATORI). QUESTA LIMITAZIONE DELLA GARANZIA COSTITUISCE PARTE ESSENZIALE DELLA LICENZA. L'USO DEL DOCUMENTO O DELLE SUE VERSIONI MODIFICATE È CONSENTITO SOLO ENTRO I TERMINI DI QUESTA LIMITAZIONE DELLA GARANZIA; E EB Abilitare/disabilitare i suoni Abilitando quest'opzione non verranno forzate le mosse del computer, dunque non comporterà una riduzione del livello di difficoltà. Eric Eric Baudais Eric Baudais <email>baudais@okstate.edu</email> Febbraio 2008 Commenti Prima mossa Prima bozza terminata. Raggruppa le pedine alla fine Progetto di documentazione GNOME Impostazioni di gioco Iagno Manuale di Iagno V2.8 Manuale di Iagno V2.9 Iagno supporta il <link linkend="network-games">multiplayer</link> per due giocatori umani in locale o in modalità di rete. Iagno è il gioco di strategia per due giocatori chiamato Otello, altrimenti conosciuto come Reversi e simile a Go. I pezzi sono dei dischetti colorati di nero su un lato nero e di bianco sull'altro. L'obiettivo del giocatore è girare i pezzi dell'avversario per farli diventare del proprio colore, cercando di fermare l'avversario che cercherà di fare lo stesso. Quando la scacchiera sarà stata riempita dai dischetti, il giocatore con più pezzi su di essa sarà il vincitore. Iagno ha il supporto per le partite in rete multigiocatore fornite da <ulink type="http" url="http://www.ggzgamingzone.org">GGZ Gaming Zone</ulink>. Collegandosi a un server su Internet di <application>Iagno</application> è possibile sfidare altri giocatori in partite multigiocatore. Per avere notizie, aggiornamenti e una lista di server ai quali connettersi, consultare il <ulink type="http" url="http://www.gnome.org/projects/gnome-games/">sito web di gnome-games</ulink>. Iago è la nemesi di Othello nella tragedia <citetitle>Othello </citetitle> di William Shakespeare. Importante: Introduzione È possibile chiacchierare con gli altri giocatori durante le partite di rete. Si possono chiedere aiuti e consigli, ma bisogna essere sempre gentili nei confronti degli altri giocatori. È possibile collegarsi a un server utilizzando un accesso anonimo oppure un normale account già registrato. Gli account anonimi consentono di accedere in anonimato, mentre un accesso normale consente di riservare il proprio nome utente, protetto da una password a scelta dell'utente. Entrare in una stanza da gioco Problemi noti e limiti Successivamente, una volta arrivati i suoi amici, come tutti i programmatori mostrò loro il suo capolavoro. Tutti gli risero in faccia e gli dissero che il suo inutile gioco non era altro che un pietoso tentativo di dimostrarsi un vero hacker. Dopo averli cacciati fuori, a causa della malignità dimostrata nei confronti del suo programma, Ian decise di rinominarlo in Iago <placeholder-1/>. Ma il suono di tale nome non lo soddisfaceva pienamente, così Ian ragionò sul fatto che tutti i nomi dei programmi di GNOME contengono qualche parte della stessa parola GNOME. Dopo molti pensieri, egli decide per il nome che noi oggi stiamo custodendo, <application>Iagno </application>. Licenza Manuale di Iagno rilasciato nei termini della licenza FDL. Adattate le schermate al tema standard del GDP, MicroGUI. Documento redatto secondo il modello del GDP. Resa più chiara la sezione giocare. Molti dei nomi utilizzati dalle aziende per distinguere i loro prodotti e servizi sono rivendicati come marchi commerciali. Dove questi nomi appaiono nella documentazione di GNOME, e i membri del progetto di documentazione di GNOME sono informati di questi marchi commerciali, allora i nomi sono in lettere maiuscole o con la lettera iniziale maiuscola. Le partite multigiocatore di Iagno hanno praticamente le stesse regole delle partite normali, fatta eccezione per il fatto che si sta giocando contro altri giocatori umani. Ciò significa che probabilmente le strategie altrui saranno migliori rispetto a quando si gioca contro l'intelligenza artificiale. Partite di rete Nota: In una notte tempestosa, come quelle tanto descritte in Inghilterra, un solitario programmatore era seduto davanti allo schermo del proprio computer. Ian aveva appena terminato il suo capolavoro di gioco, si trattava di un gioco interattivo di Othello ed egli decise di giocare con il suo nome, chiamandolo gnothello, proprio perché scritto usando GNOME. Quando un giocatore vince, la partita termina e sarà possibile tornare alla schermata iniziale della partita di rete. Sarà possibile giocare un'altra entusiasmante partita multigiocatore di Iagno! Una volta connessi al server è possibile scegliere la stanza nella quale entrare. Per partecipare a una partita multigiocatore di Iagno, selezionare la stanza Iagno. Se invece si desidera ospitare una partita è sufficiente fare clic sul pulsante Avvia. Verrà creato un nuovo tavolo al quale altri giocatori potranno partecipare per iniziare delle nuove sfide. Se sono presenti altre partite già iniziate è possibile fare clic sopra un tavolo di gioco già avviato per parteciparvi. L'elenco dei tavoli di gioco sulla destra mostra il numero di posti disponibili, che rappresentano il numero di giocatori che possono prendere parte al tavolo di gioco. Dopo aver preso parte a un tavolo di gioco sarà necessario attendere che un numero sufficiente di giocatori entri nel gioco. La voce di menù <guimenuitem>Elenco giocatori</guimenuitem> consente di vedere un elenco di giocatori che partecipano al gioco. Una volta raggiunto il numero totale di giocatori per il gioco, la partita inizierà immediatamente. Questo documento può essere copiato, distribuito e/o modificato solo in conformità con i termini della GNU Free Documentation License (GFDL) Versione 1.1 o delle versioni successive pubblicate dalla Free Software Foundation senza sezioni invariabili, frontespizi e testi di copertina. Una copia della GFDL è disponibile a questo <ulink type="help" url="ghelp:fdl">collegamento</ulink> o nel file COPYING-DOCS distribuito con questo manuale. Colore dei giocatori e difficoltà Giocare a Iagno Giocare partite multigiocatore di Iagno Opzione Usare mosse rapide Schermata di un blocco del nero. Schermata delle impostazioni dell'animazione. Schermata della prima mossa. Schermata delle impostazioni del gioco. Schermata della posizione iniziale Settembre 2004 Mostra griglia Capovolgimenti sfalsati Posizione iniziale L'opzione <guimenuitem>Accesso anonimo</guimenuitem> va selezionata se si desidera utilizzare un account anonimo. Se si desidera creare un nuovo account normale, selezionare l'opzione <guimenuitem>Primo accesso</guimenuitem>, con un nome utente, una password e un indirizzo email a scelta. Se si ha già creato un account, è possibile collegarsi selezionando l'opzione <guimenuitem>Accesso normale</guimenuitem>, inserendo il proprio nome utente e la propria password. Avviando una nuova partita di rete viene mostrata la finestra di dialogo di connessione di Iagno. In questa finestra è possibile scegliere un server al quale connettersi e il nome utente da utilizzare per la connessione. La finestra di dialogo della connessione di Iagno. Il menù Preferenze con la scheda Aspetto evidenziata. Il menù Preferenze con la scheda Gioco evidenziata. La scacchiera iniziale contiene quattro pedine sistemate in un riquadro centrale. Le pedine agli angoli hanno lo stesso colore, come mostrato in <xref linkend="start-shot"/>. Il giocatore del computer di Iagno è facile da battere. L'ultima opzione della scheda <guimenuitem>Gioco</guimenuitem> della finestra <guilabel>Preferenze</guilabel> consente di abilitare o disabilitare tutti i suoni durante lo svolgimento di una partita di <application>Iagno</application>. Sono disponibili due collezioni di pedine, «woodtrim» e «classic», le quali possono sembrare uguali ma in realtà hanno delle animazioni differenti. Questo dialogo di rete di Iagno consente di partecipare a una partita per cercare altri giocatori. Questo manuale si riferisce alla versione 2.20 di Iagno. Questo manuale è parte di una collezione di manuali di GNOME distribuita sotto la GFDL. Se si vuole distribuire questo manuale separatamente dalla collezione, lo si può fare aggiungendo una copia della licenza al manuale, come descritto nella sezione 6 della licenza. Se si abilita questa opzione, quando verranno catturate delle pedine lungo una riga, una colonna o una diagonale, queste, invece di venire capovolte tutte contemporaneamente, verranno girate una dopo l'altra, in un modo simile al domino. Tale opzione non ha alcun effetto sul gioco, lo renderà solo più carino. Utilizzando la casella di scelta relativa a questa opzione si può controllare la qualità dell'animazione che viene riprodotta quando viene girato un pezzo. Selezionando l'opzione <guibutton>Nessuna</guibutton> il colore del pezzi verrà cambiato all'istante. Selezionando l'opzione <guibutton>Parziale</guibutton> i pezzi verranno girati lentamente, mentre selezionando <guibutton>Completa</guibutton> l'animazione risulterà più lenta e curata nei dettagli. Questa opzione non modifica in nessun modo il risultato della partita. L'opzione è presente nel menù <guimenu>Impostazioni</guimenu>. Andare nel sottomenù <guisubmenu>Preferenze</guisubmenu>, dunque fare clic sulla scheda <guimenuitem>Gioco </guimenuitem>, in modo predefinito l'opzione è deselezionata. Se si abilita tale opzione il computer posizionerà in modo più veloce le proprie pedine. Questa opzione consente di scegliere il tipo della scacchiera e delle pedine da visualizzare. Se si desidera creare una propria collezione di pedine utilizzare come modello la collezione classica e cambiare il disegno e i colori per i differenti effetti. Quando si crea una nuova collezione funzionante correttamente con Iagno inviarla a Ian Peters <email>itp@gnu.org</email>, che la includerà con la successiva distribuzione. Con questa opzione, alla fine della partita, tutte le pedine nere verranno raggruppate nella parte alta della scacchiera e le pedine bianche nella parte bassa. Ciò faciliterà la visione del risultato finale del gioco. Questa opzione può risultare utile ai principianti per una visione più chiara del gioco. Questa opzione mostrerà una griglia sulla superficie della scacchiera. È utile visualizzare la griglia poiché mostra al giocatore l'esatta posizione delle pedine che si stanno posizionando. Questo programma è distribuito nella speranza che sia utile, ma SENZA ALCUNA GARANZIA; senza neppure la garanzia implicita di NEGOZIABILITÀ o di APPLICABILITÀ PER UN PARTICOLARE SCOPO. Si veda la <citetitle>GNU General Public License</citetitle> per avere maggiori dettagli. Questo programma è software libero; è lecito redistribuirlo e/o modificarlo secondo i termini della <citetitle>GNU General Public License</citetitle> come è pubblicata dalla Free Software Foundation; o la versione 2 della licenza o (a propria scelta) una versione successiva. Raccolta di pedine Suggerimento: Per collegarsi a un server è sufficiente fare clic sul pulsante <guimenuitem>Connetti</guimenuitem>. Per segnalare un problema o inviare suggerimenti sull'applicazione <application>Iagno</application> o su questo manuale, seguire le istruzioni presenti alla <ulink url="ghelp:gnome-feedback" type="help">pagina di commenti su GNOME</ulink>. Per avviare <application>Iagno</application>, selezionare <guimenuitem>Iagno</guimenuitem> dal sottomenù <guisubmenu>Giochi</guisubmenu> del menù <guimenu>Applicazioni</guimenu>, oppure digitare <command>iagno</command> sulla riga di comando. Per iniziare una partita multigiocatore di Iagno, selezionare <menuchoice><guimenu>Gioco</guimenu><guimenuitem>Partita in rete</guimenuitem></menuchoice> dal menù dell'applicazione. IN NESSUNA CIRCOSTANZA E PER NESSUNA RAGIONE LEGALE, INCLUSI I PRINCIPI DI COLPA (INCLUSA LA NEGLIGENZA), ACCORDO CONTRATTUALE O ALTRO, SARÀ POSSIBILE CONSIDERARE L'AUTORE, IL REDATTORE INIZIALE, GLI ALTRI COLLABORATORI, QUALUNQUE DISTRIBUTORE DEL DOCUMENTO O DI UNA SUA VERSIONE MODIFICATA O QUALUNQUE FORNITORE DELLE PERSONE CITATE, RESPONSABILE NEI CONFRONTI DI QUALUNQUE PERSONA PER DANNI DIRETTI, INDIRETTI, SPECIALI, INCIDENTALI O CONSEGUENTI DI QUALUNQUE NATURA, INCLUSI, MA SENZA LIMITAZIONE, I DANNI PER PERDITA DI AVVIAMENTO, INTERRUZIONE DEL LAVORO, GUASTO O MALFUNZIONAMENTO DEL COMPUTER O QUALUNQUE ALTRO DANNO O PERDITA DERIVANTE O CORRELATA ALL'USO DEL DOCUMENTO O DI UNA SUA VERSIONE MODIFICATA, ANCHE QUALORA LE PERSONE CITATE FOSSERO STATE INFORMATE DELLA POSSIBILITÀ DI TALI DANNI. Aspettare che altri giocatori partecipino alla partita Cos'è Iagno? Quando si crea un nuovo tavolo per partite di Iagno viene mostrato un dialogo di preferenze che consente di personalizzare alcuni aspetti del gioco, come il numero minimo di giocatori per la partita. Una volta occupati tutti i posti liberi, nessun giocatore potrà più partecipare al tavolo di gioco. Quando una pedina si trova racchiusa fra altre due dello stesso colore e nella stessa riga, il giocatore cattura le pedine fino a quando non ne incontra un'altra del suo stesso colore, come mostrato in <xref linkend="block-shot"/>. Il giocatore può prendere solo le pedine che sono il risultato del posizionamento di un'altra pedina sulla scacchiera. Una volta posizionata una pedina il giocatore non può più rimuoverla e spostarla da qualche altra parte. Se un giocatore non può muovere salta il suo turno. Il gioco finisce quando nessun giocatore può più effettuare altre mosse. <placeholder-1/> Quando si avvia Iagno il giocatore umano gioca con il nero e il computer con il bianco con il primo livello di difficoltà. Usando la scheda <guimenuitem>Gioco</guimenuitem> nella finestra <guisubmenu>Preferenze</guisubmenu> è possibile impostare il tipo di giocatore per ogni colore. Per umano si intende il giocatore, il livello uno e il livello tre sono rispettivamente il più semplice e il più difficile per il computer. È possibile impostare come umani entrambi i giocatori se si desidera giocatore contro un amico, oppure impostarli entrambi come computer per guardare il computer giocare contro se stesso. Il bianco non può catturare la pedina nera al centro della colonna di mezzo, poiché ci sono delle pedine bianche che bloccano entrambi i lati finali della stessa. Se fosse il turno del nero, invece, lo stesso potrebbe posizionare una pedina in testa alla colonna, capovolgendo solo la prima delle due pedine bianche. Perché è un Othello chiamato Iagno? Probabilmente ci si starà chiedendo perché questo gioco è chiamato <application>Iagno</application> invece che Othello oppure gnothello. La risposta è complicata e riguarda cose come marchi registrati e avvocati e non. Piuttosto che lasciare le cose così come sono, verrà raccontata una lunga e complicata bugia sull'origine del nome <application>Iagno</application>. baudais@okstate.edu Alessio Treglia <quadrispro@ubuntu-it.org>, 2008 