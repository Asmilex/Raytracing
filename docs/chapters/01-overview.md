# Overview

Ejemplo! [@Shirley2020RTW1]
You can make chapters by using `h1` Markdown headings.

Paragraphs are separated by a blank line.

2nd paragraph. *Italic*, **bold**, and `monospace`. Itemized lists
look like:

  * this one
  * that one
  * the other one

Note that --- not considering the asterisk --- the actual text
content starts at 4-columns in.

> Block quotes are
> written like so.
>
> They can span multiple paragraphs,
> if you like.

Use 3 dashes for an em-dash. Use 2 dashes for ranges (ex., "it's all
in chapters 12--14"). Three dots ... will be converted to an ellipsis.
Unicode is supported. ☺



An h2 header
------------

Here's a numbered list:

 1. first item
 2. second item
 3. third item

Note again how the actual text starts at 4 columns in (4 characters
from the left side). Here's a code sample:

    # Let me re-iterate ...
    for i in 1 .. 10 { do-something(i) }

As you probably guessed, indented 4 spaces. By the way, instead of
indenting the block, you can use delimited blocks, if you like:

~~~
define foobar() {
    print "Welcome to flavor country!";
}
~~~

(which makes copying & pasting easier). You can optionally mark the
delimited block for Pandoc to syntax highlight it:

~~~python
import time
# Quick, count to ten!
for i in range(10):
    # (but not *too* quick)
    time.sleep(0.5)
    print(i)
~~~



### An h3 header ###

Now a nested list:

 1. First, get these ingredients:

      * carrots
      * celery
      * lentils

 2. Boil some water.

 3. Dump everything in the pot and follow
    this algorithm:

        find wooden spoon
        uncover pot
        stir
        cover pot
        balance wooden spoon precariously on pot handle
        wait 10 minutes
        goto first step (or shut off burner when done)

    Do not bump wooden spoon or it will fall.

Notice again how text always lines up on 4-space indents (including
that last line which continues item 3 above).

Here's a link to [a website](http://foo.bar), to a [local
doc](local-doc.html), and to a [section heading in the current
doc](#an-h2-header). Here's a footnote [^1].

[^1]: Some footnote text.

Tables can look like this:

Name           Size  Material      Color
------------- -----  ------------  ------------
All Business      9  leather       brown
Roundabout       10  hemp canvas   natural
Cinderella       11  glass         transparent

Table: Shoes sizes, materials, and colors.

(The above is the caption for the table.) Pandoc also supports
multi-line tables:

--------  -----------------------
Keyword   Text
--------  -----------------------
red       Sunsets, apples, and
          other red or reddish
          things.

green     Leaves, grass, frogs
          and other things it's
          not easy being.
--------  -----------------------

A horizontal rule follows.

***

Here's a definition list:

apples
  : Good for making applesauce.

oranges
  : Citrus!

tomatoes
  : There's no "e" in tomatoe.

Again, text is indented 4 spaces. (Put a blank line between each
term and  its definition to spread things out more.)

Here's a "line block" (note how whitespace is honored):

| Line one
|   Line too
| Line tree

and images can be specified like so:

![example image](example-image.jpg "An exemplary image")

Inline math equation: $\omega = d\phi / dt$. Display
math should get its own line like so:

$$I = \int \rho R^{2} dV$$

And note that you can backslash-escape any punctuation characters
which you wish to be displayed literally, ex.: \`foo\`, \*bar\*, etc.
# Crinis mixtaque factisque ille

## Aut nunc furori ad latarumque Philomela

Lorem markdownum includite volenti monticolae videre vocem hac sparsit puta
gelidis vestros egressus sex. Undis eris per auguris armis. Est saevior pater.
Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.

## Gaudet Silenus iuvenis

Mulciber denique faces ingratus, in umeros umeri cum, iram ira custos non.
Pariterque admissa nubes, in ait ecce setae summis sacrorum me gaudete tellus.
Ille tu perire ille, artificis caede.

```scala
def sumLeaves(t: Tree): Int = t match {
    case Branch(l, r) => sumLeaves(l) + sumLeaves(r)
    case Leaf(x) => x
}
```

Cephea rector minorque, quem corpora,
Argus. Superi hoc tenuavit timebant ossibus totque non serpere animo corpore
superas gelidae, comitate deus Iunonigenaeque
pectora.

- Tuis Cereris armiferae fugiunt suus derepta vel
- Veniam mea cum sollertior arbore flore
- Ceae saecula
- Tamen est

## Dies tunc in enim

Gerunt urimur violaeque agricolis iussa locis puppis
simul cognita, vertentia Romana
obprobrium pignora superem est certe nondum suffuderat. Nox Pasiphaeia domo:
**abiit** catenas utro crimine gramina ingemuere mixtae. Quem trabibus etiamnum
orbe addita, eiaculatur videri cervo artus. Nutritur cupidine silentia Maeoniam
aere enim gemuit adgreditur, telasque *annis* nos cum Arctonque ingens lateri
cum iaculoque ferus.

## Et dextra utque per lenius portus eburnae

Cui vittas aris ibi putat dicere; factum sedere antiqua? Cognita Lyncides iuste
insuetum lacerum in sinamus arces; aves aevum spatiumque de utrumque moveret in?
Tertia ordine, Epidaurius, *has sed et* et novat: quod superare concubitusque
retia quoque, ne totiens.

Est paenitet Cerealia sparsit; carne insignia in maris; tibi Nec, que Peleu meum
buxus. Propoetides formae magna auro ad gerat cohibentur facienda partem at
nunc, foret? **Ad stirpe**! Ut latius pararet: vestibus cumque pedibus ficta
prior summas cancer ipsa Marte Buten es
terruit. Opifex dixit oculos Oete quoque, quot silvarum abrepti nutrix concita
obsidis consistere fibula saxum, Antigonen minabitur tota.

# Vagata eiectatamque sidera satis reducet

## Talem ex aliquo ingemuit

Lorem markdownum solus miserabile sitae. Tantum Syron limenque cupidine: litore
modo coniuge: in huc, illo crimen novena, vocisque gratia, quae. Sua manusque
patris nec meritorum pedibus hominis virgine, ruere tamen virtus aliter. Tunc
ego. Solitaque remittant fagus omnia eat.

$$r_d^i(t+1) = \min\{r_s,\max\{0, r_d^i(t) + \beta(n_t - \lvert N_i(t)\rvert)\}\}$$

Obstitit silentia et novi non, huic metitur, coronantur lucos. Bracchia aura;
donis quod volucris illi futurae, ut
*venturorumque tellus* arma: saxumque.
fdlsfksdjkfsjdf
## Vera tum est putes adspicit noxque

Hora et vidit figere tangi! Omni bis *prior nunc* capilli, pulsat tuam Pallante,
*suis*. Solae decore ipso armorum coitusque paro audita *viveret tibi* apparuit
flammasque lapides. Cursu anas usus eundo anticipata, intabescere quae concita
fallit. Dea corpore fabrae: nec Neptunus membris, falsa murice; fac Marte quam
in.

Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et.

## Arachnes deus tum penetralia tempore aurea populante

Lentae spissisque *carne*. Fixit inquit cautes et iugis novus sim quisquam nisi
haesurum vel deorum fetibus virgo.

Sub nautae, tegebat clamat. Credas Parrhasio. Commemorat nescio liceatque
excipit! Uris clipeo ego visa amplexas meos ibitis condidit Taenaria, si. Tua
ora tempus patrio revulsos, tellus curru facies, Gange gemit agitata!

Ruinam ipsaque sibi ovis Teucer Iovis tibi; erat versus neque victa attonitus
doque, quod! Dixit carmina, eo, per capillis quid lina, qua, ille. Siqua
caelestum flammas ferre super et saevissime inmisit quoque suis sic aspergine
vis praerupit. Et puellae summa eventu.
Placeat ut medio *plectrumque inferni* Talia; pertimui opem.

# Images and Tables

## LaTeX Table with Caption

At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr.

\begin{longtable}[]{llllllll}
\caption[Nam liber tempor cum soluta nobis eleifend option congue.]{Nam liber tempor cum soluta nobis eleifend option congue nihil imperdiet doming id quod mazim placerat facer possim assum. Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat.} \\
\toprule
Test Nr. & Position & Radius & Rot & Grün & Blau &
beste Fitness & Abweichung\tabularnewline
\midrule
\endhead
1 & 20 \% & 20 \% & 20 \% & 20 \% & 20 \% & 7,5219 &
0,9115\tabularnewline
2 & 0 \% & 25 \% & 25 \% & 25 \% & 25 \% & 8,0566 &
1,4462\tabularnewline
3 & 0 \% & 0 \% & 33 \% & 33 \% & 33 \% & 8,7402 & 2,1298\tabularnewline
4 & 50 \% & 20 \% & 10 \% & 10 \% & 10 \% & 6,6104 &
0,0000\tabularnewline
5 & 70 \% & 0 \% & 10 \% & 10 \% & 10 \% & 7,0696 &
0,4592\tabularnewline
6 & 20 \% & 50 \% & 10 \% & 10 \% & 10 \% & 7,0034 &
0,3930\tabularnewline
\bottomrule
\end{longtable}

At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr.

## Image with Caption

![Nam liber tempor cum soluta nobis eleifend option congue nihil imperdiet doming id quod mazim placerat facer possim assum. Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat.](image.png)

## Markdown Table without Caption

Lorem markdownum Letoia, et alios: figurae flectentem annis aliquid Peneosque abesse, obstat gravitate. Obscura atque coniuge, per de coniunx, sibi medias
commentaque virgine anima tamen comitemque petis, sed. In Amphion vestros
hamos ire arceor mandere spicula, in licet aliquando.

Test Nr. | Position | Radius | Rot | Grün | Blau | beste Fitness | Abweichung |
|---|---|---|---|---|---|---|---|
1 |  20 % |  20 % |  20 % |  20 % |  20 % |  7,5219 |  0,9115 |
2 |   0 % |  25 % |  25 % |  25 % |  25 % |  8,0566 |  1,4462 |
3 |   0 % |   0 % |  33 % |  33 % |  33 % |  8,7402 |  2,1298 |
4 |  50 % |  20 % |  10 % |  10 % |  10 % |  6,6104 |  0,0000 |
5 |  70 % |   0 % |  10 % |  10 % |  10 % |  7,0696 |  0,4592 |
6 |  20 % |  50 % |  10 % |  10 % |  10 % |  7,0034 |  0,3930 |
7 |  40 % |  15 % |  15 % |  15 % |  15 % |  6,9122 |  0,3018 |

Porrigitur et Pallas nuper longusque cratere habuisse sepulcro pectore fertur.
Laudat ille auditi; vertitur iura tum nepotis causa; motus. Diva virtus! Acrota destruitis vos iubet quo et classis excessere Scyrumve spiro subitusque mente Pirithoi abstulit, lapides.
