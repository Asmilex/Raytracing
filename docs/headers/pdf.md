---
# lang: spanish     # Roto. (https://github.com/Wandmalfarbe/pandoc-latex-template/issues/278)
date: \today
book: true
classoption: [twoside]

mainfont: 'Crimson Pro Regular'
mainfontoptions:
 - BoldFont=CrimsonPro-Bold
 - ItalicFont=CrimsonPro-Italic
fontsize: 13pt

monofont: JuliaMono
monofontoptions:
 - Scale=0.8

titlepage: true
subtitle: "Doble grado en ingeniería informática y matemáticas \\ \\href{https://github.com/asmilex/Raytracing/}{github.com/asmilex/Raytracing/}"
institute: "Universidad de Granada"
author:
- "\\textbf{Presentado por}: Andrés Millán Muñoz,"
- "\\textbf{Tutorizado por}: Carlos Ureña Almagro, María del Carmen Segovia García"
#logo: ./img/ugr.png
logo-width: 100mm
titlepage-rule-height: 0
titlepage-background: "./img/ugrA4.pdf"

toc: true
toc-depth: 4
numbersections: true
colorlinks: true

header-left: "\\hspace{0.1cm}"
header-right: "\\textcolor{gray}{\\textit{\\leftmark}}"
footer-left: "\\hspace{1cm}"
footer-center: "\\thepage"
footer-right: "\\hspace{1cm}"

geometry:
- top=30mm
- left=20mm
- right=20mm

---
