import pandas as pd
import sys, argparse

parser = argparse.ArgumentParser(description='Parse an MSI Afterburner monitoring log file.')
parser.add_argument('-i', metavar='<inputfile>', help='Log filename', required=True)
parser.add_argument('--consecutive', '-c', help='Read consecutive log files as one', action="store_true")
parser.add_argument('--frametime', help='Parse frametime', action="store_true")

data = []
index = []

args = parser.parse_args()

framerates = []
frametimes = []
index = [0, 0]
log = open(args.i, "r", encoding="ISO-8859-1")

size_data = 0
lista_df = [] # El log puede contener varios benchmarks
df = pd.DataFrame() # El dataframe temporal

for line_raw in log:
    line = list(map(str.strip, line_raw.split(',')))

    # Quitar las cabeceras que pone Afterburner al principio del log
    if 'Framerate' in line:
        # Hay que tener cuidado, porque el log puede contener varios benchmarks
        # Cuando nos encontremos de nuevo las cabeceras, habrá que crear un nuevo dataframe
        # Guardaremos el antiguo en una lista
        if not df.empty:
            lista_df.append(df)

        size_data = len(line)
        line.pop(0)
        line[0] = 'Date & time'
        # Inicializar el dataframe con las cabeceras
        df = pd.DataFrame(columns=line)

    elif len(line) == size_data:
        # El resto de líneas que tengan el mismo tamaño contienen los datos propiamente dichos.
        # Quitar la primera entrada, pues tienen información de la línea
        line.pop(0)
        # Añadir la línea al dataframe
        df = df.append(pd.Series(line, index=df.columns), ignore_index=True)

# Guardar el último
lista_df.append(df)
log.close()

ultimo_df = lista_df[-1]
print(df)