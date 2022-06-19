FROM dockershelf/latex:full-stable
LABEL maintainer="andresmm@outlook.com"

# https://github.com/Asmilex/Raytracing.git
RUN apt-get update && apt-get install make git wget sudo chafa python3-pip
RUN wget https://github.com/jgm/pandoc/releases/download/2.18/pandoc-2.18-1-amd64.deb && dpkg -i pandoc-2.18-1-amd64.deb
RUN pip install pandoc-eqnos pandoc-fignos pandoc-tablenos && \
    ln -s /usr/bin/python3 /usr/bin/python

# Instalar Crimson Pro y Julia Mono

RUN cd /usr/share/fonts/opentype && \
    # Crimson Pro
    mkdir crimson-pro && \
    cd crimson-pro && \
    wget https://github.com/Fonthausen/CrimsonPro/raw/master/fonts/otf/CrimsonPro-Regular.otf && \
    wget https://github.com/Fonthausen/CrimsonPro/raw/master/fonts/otf/CrimsonPro-Italic.otf && \
    wget https://github.com/Fonthausen/CrimsonPro/raw/master/fonts/otf/CrimsonPro-Bold.otf && \
    # Julia Mono
    cd /usr/share/fonts/truetype && \
    mkdir julia-mono && \
    cd julia-mono && \
    wget https://github.com/cormullion/juliamono/raw/master/JuliaMono-Regular.ttf && \

    fc-cache -f -v

WORKDIR /data
ENTRYPOINT ["/usr/bin/pandoc"]
