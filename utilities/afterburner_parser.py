import pandas as pd
import sys, argparse
import os

parser = argparse.ArgumentParser(description='Parse an MSI Afterburner monitoring log file.')
parser.add_argument('-i', metavar='<inputfile>', help='Log filename', required=True)

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
        df.loc[len(df)] = line

# Guardar el último
lista_df.append(df)
log.close()

ultimo_df = lista_df[-1]

# Save the df to a csv file within the same directory
filename = os.path.splitext(os.path.basename(args.i))[0]
csv_path = './csv/' + filename + ".csv"

if not os.path.exists('./csv'):
    os.mkdir('./csv')

ultimo_df.to_csv(csv_path, index=False)
