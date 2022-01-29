---
title: Pedazo de título del TFG
author: Andrés Millán Muñoz (<andresmm@outlook.com>)
date: \today
keywords: [TFG]
# lang: spanish     # Roto. (https://github.com/Wandmalfarbe/pandoc-latex-template/issues/278)

book: true
classoption: [oneside]
#geometry: margin=1.5in

bibliography: bibliography.bib
link-citations: true

 # ────────────────────────────────────────────────────────────────── FUENTES ─────

mainfont: 'Crimson Pro Regular'
fontsize: 12pt

#mainfont: Fraunces
#fontsize: 11pt

monofont: JuliaMono
monofontoptions:
 - Scale=0.8

titlepage: true
toc: true
numbersections: true

header-left: "\\textcolor{gray}{\\thetitle}"
header-right: "\\textcolor{gray}{\\textit{\\leftmark}}"
footer-left: "\\hspace{1cm}"
footer-center: "\\thepage"
footer-right: "\\hspace{1cm}"


colorlinks: true
linkcolor: RoyalBlue
urlcolor: RoyalBlue

page-background: ./img/background.png
page-background-opacity: 0.85


# HTML
header-includes:
- |
  ```{=html}
  <link rel="stylesheet" href="https://unpkg.com/bamboo.css">
  <link rel="stylesheet" href="./style.css">
  ```
---
