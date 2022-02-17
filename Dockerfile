FROM dockershelf/latex:full
LABEL maintainer="andresmm@outlook.com"

# https://github.com/Asmilex/Raytracing.git
RUN apt-get update && apt-get install make git wget sudo
RUN wget https://github.com/jgm/pandoc/releases/download/2.17.1.1/pandoc-2.17.1.1-1-amd64.deb && dpkg -i pandoc-2.17.1.1-1-amd64.deb
