import csv
import numpy as np
import matplotlib.pyplot as plt


#Gráficos para criar:
# 1. Gráfico de barras com o tempo médio de inicialização e resolução por dimensão e número de threads.
# 2. Gráfico de barras com o tempo médio de resolução por dimensão e número de threads.
# 3. Gráfico de barras com o tempo médio de inicialização por dimensão e número de threads.
# 4. Aceleração média por dimensão e número de threads. (calcular aceleração média por jogo, depois média dessas aceleracoes por dimensao)
    # 4.1 colocar todas as dimensoes em um grafico só
# 5. Mesma coisa da aceleração, mas com eficiencia.



# Leitura e processamento dos dados
data = []
with open('data.csv', newline='') as csvfile:
    reader = csv.DictReader(csvfile)
    for row in reader:
        row['gameNumber'] = int(row['gameNumber'])
        row['dimension'] = int(row['dimension'])
        row['nThreads'] = int(row['nThreads'])
        row['initTime'] = float(row['initTime'])
        row['resolutionTime'] = float(row['resolutionTime'])
        data.append(row)

dimensions = sorted(set(row['dimension'] for row in data))
n_threads = sorted(set(row['nThreads'] for row in data))

def mean_times_chart(which="all"):

    # Agrupar dados por dimensão e número de threads
    grouped_data = {}
    for dim in dimensions:
        grouped_data[dim] = {}
        for thr in n_threads:
            entries = [row for row in data if row['dimension'] == dim and row['nThreads'] == thr]
            if entries:
                avg_init = sum(e['initTime'] for e in entries) / len(entries)
                avg_resolution = sum(e['resolutionTime'] for e in entries) / len(entries)
                grouped_data[dim][thr] = {
                    'avgInitTime': avg_init,
                    'avgResolutionTime': avg_resolution,
                    'avgTotalTime': avg_init + avg_resolution
                }

    # Configurações do gráfico
    bar_width = 0.4
    colors = {'init': '#1f77b4', 'resolution': '#ff7f0e'}

    # Criar uma figura com subplots para cada dimensão
    fig, axes = plt.subplots(
        nrows=len(dimensions), 
        figsize=(10, 4 * len(dimensions)),
        squeeze=False
    )
    axes = axes.flatten()

    for i, dim in enumerate(dimensions):
        ax = axes[i]
        init_times = []
        resolution_times = []
        threads = []
        
        for thr in n_threads:
            if thr in grouped_data[dim]:
                init_times.append(grouped_data[dim][thr]['avgInitTime'])
                resolution_times.append(grouped_data[dim][thr]['avgResolutionTime'])
                threads.append(str(thr))

        x = np.arange(len(threads))
        
        if(which == "resolution"):
            ax.bar(x, resolution_times, bar_width, color=colors['resolution'], label='Resolução')
        else:
            ax.bar(x, init_times, bar_width, color=colors['init'], label='Inicialização')
            if(which == "all"):
                ax.bar(x, resolution_times, bar_width, bottom=init_times, color=colors['resolution'], label='Resolução')


        #somente resolucao 


        # Remover eixos redundantes
        if i < len(dimensions) - 1:
            ax.set_xlabel('')
            ax.set_xticklabels([])
        else:
            ax.set_xlabel('Número de Threads')

        ax.set_ylabel('Tempo (s)' if i % 2 == 0 else '')  # Mostrar em eixos alternados

        # Adicionar anotação ao invés de título
        ax.text(0.95, 0.95, f'Dimensão {dim}', transform=ax.transAxes,
                fontsize=12, verticalalignment='top', horizontalalignment='right',
                bbox=dict(boxstyle='round,pad=0.2', facecolor='lightgray', alpha=0.5))

        # Só mostrar legenda no primeiro subplot
        if i == 0:
            ax.legend(loc='upper left')
        else:
            ax.legend().remove()

        ax.set_xticks(x)
        ax.set_xticklabels(threads)

    plt.tight_layout()
    plt.show()


mean_times_chart()
