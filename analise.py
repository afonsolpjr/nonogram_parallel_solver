import csv
import numpy as np
import matplotlib.pyplot as plt


#Gráficos para criar:
# 1. Gráfico de barras com o tempo médio de inicialização e resolução por dimensão e número de threads.
# 2. Gráfico de barras com o tempo médio de resolução por dimensão e número de threads.
# 3. Gráfico de barras com o tempo médio de inicialização por dimensão e número de threads.
# 4. Aceleração média por dimensão e número de threads. (calcular aceleração média por jogo, depois média dessas aceleracoes por dimensao)
    # 4.1 colocar todas as dimensoes em um grafico só
# 5. Mesma coisa da aceleração, mas com eficiencia 



# Leitura e processamento dos dados
data = []
with open('data.csv', newline='') as csvfile:
    reader = csv.DictReader(csvfile)
    for row in reader:
        row['gameId'] = int(row['gameId'])
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
        
        if(which!="resolution"):
            ax.bar(x, init_times, bar_width, color=colors['init'], label='Inicialização')
            if(which=="all"):
                ax.bar(x, resolution_times, bar_width, bottom=init_times, color=colors['resolution'], label='Resolução')
        else:
            ax.bar(x, resolution_times, bar_width, color=colors['resolution'], label='Resolução')



        # Somente o último mostra o eixo X completo
        if i < len(dimensions) - 1:
            ax.set_xticks([])
            ax.set_xlabel('')
        else:
            ax.set_xticks(x)
            ax.set_xticklabels(threads)
            ax.set_xlabel('Número de Threads')

        ax.set_ylabel('Tempo (s)' if i % 2 == 0 else '')

        # Anotação da dimensão no canto
        ax.text(0.95, 0.95, f'Dimensão {dim}', transform=ax.transAxes,
                fontsize=12, verticalalignment='top', horizontalalignment='right',
                bbox=dict(boxstyle='round,pad=0.2', facecolor='lightgray', alpha=0.5))

        if i == 0:
            ax.legend(loc='upper left')
        else:
            ax.legend().remove()

    plt.tight_layout()
    if(which=="all"):
        filename = 'graficos/avg_times_all.png'
    elif(which=="init"):
        filename = 'graficos/avg_times_init.png'
    else:
        filename = 'graficos/avg_times_resolution.png'
    plt.savefig(filename)
    plt.close()

def calc_speedups():
    #objetivo: aceleracao media por dimensao
    speed_ups = {
    }

    for dimension in dimensions:
        speed_ups[dimension] = {
            'avgInit': [],
            'avgResolution': [],
            'avgTotal': []
        }
        #calcular aceleracao media por jogo
        game_ids = set(row['gameId'] for row in data if row['dimension']==dimension)


        game_sequential_data = {}
        for n_thread in n_threads:

            initSpeedUps = [] 
            resolutionSpeedUps = []
            totalSpeedUps = []

            for game_id in game_ids:
                
                entries = [row for row in data if row['gameId']==game_id and row['nThreads']==n_thread]
                avgInitTime = sum(exec['initTime'] for exec in entries)/len(entries)
                avgResolutionTime = sum(exec['resolutionTime'] for exec in entries)/len(entries)
                # print("Tempo medio de execs init e res para o jogo {} = {} {}".format(game_id,avgInitTime,avgResolutionTime))
                if(n_thread==1):
                    game_sequential_data[game_id] = {
                        'sequentialAvgInit' : avgInitTime,
                        'sequentialAvgResolution' : avgResolutionTime,
                        'sequentialAvgTotal' : avgResolutionTime + avgInitTime
                    }
                
                initSpeedUps.append(game_sequential_data[game_id]['sequentialAvgInit']/avgInitTime)
                resolutionSpeedUps.append(game_sequential_data[game_id]['sequentialAvgResolution']/avgResolutionTime)
                totalSpeedUps.append( game_sequential_data[game_id]['sequentialAvgTotal']/(avgResolutionTime+avgInitTime) )

            # print(initSpeedUps,resolutionSpeedUps,totalSpeedUps)
            speed_ups[dimension]['avgInit'].append(sum(initSpeedUps)/len(initSpeedUps))
            speed_ups[dimension]['avgResolution'].append(sum(resolutionSpeedUps)/len(resolutionSpeedUps))
            speed_ups[dimension]['avgTotal'].append(sum(totalSpeedUps)/len(totalSpeedUps))
    return speed_ups
        

        
        #calcular media para todos os jogos

def plot_speedup_chart(which = "all"):
    speed_ups = calc_speedups()

    match which:
        case "all":
            key = 'avgTotal'
            texto = "tempo total"
        case "init":
            key = 'avgInit'
            texto = "tempo de inicialização"
        case "resolution":
            key = 'avgResolution'
            texto = "tempo de resolução"
    
    for n_dim in dimensions:
       
        # print("Printando:",
        #     "\n\t Eixo x:{}".format(list(n_threads).sort()),
        #     "\n\t Eixo y = {}".format(speed_ups[n_dim][key]))

        plt.scatter(sorted(list(n_threads)),speed_ups[n_dim][key],
                    marker='.')
        plt.plot(sorted(list(n_threads)),speed_ups[n_dim][key],
                    label="n={}".format(n_dim),
                    linestyle='dotted')
        
        plt.xlabel("p (Nº de threads)")
        plt.ylabel("S(n,p)")
        plt.title("Aceleração S(n,p) do {}, por número de threads".format(texto))
        plt.legend()

    # plt.show()

    plt.savefig("graficos/speedup_{}.png".format(which))
    plt.close()
    
def plot_eff_chart(which="all"):
    speed_ups = calc_speedups()

    match which:
        case "all":
            key = 'avgTotal'
            texto = "tempo total"
        case "init":
            key = 'avgInit'
            texto = "tempo de inicialização"
        case "resolution":
            key = 'avgResolution'
            texto = "tempo de resolução"
    
    for n_dim in dimensions:
       
        # print("Printando:",
        #     "\n\t Eixo x:{}".format(list(n_threads).sort()),
        #     "\n\t Eixo y = {}".format(speed_ups[n_dim][key]))
        efficiencies = []
        for i in range(len(n_threads)):
            efficiencies.append(speed_ups[n_dim][key][i]/n_threads[i])

        plt.scatter(sorted(list(n_threads)),efficiencies,
                    marker='.')
        plt.plot(sorted(list(n_threads)),efficiencies,
                    label="n={}".format(n_dim),
                    linestyle='dotted')
        
        plt.xlabel("p (Nº de threads)")
        plt.ylabel("E(n,p)")
        plt.title("Eficiencia E(n,p) do {}, por número de threads".format(texto))
        plt.legend()

    # plt.show()

    plt.savefig("graficos/efficiency_{}.png".format(which))
    plt.close()


tempos = ["all","init","resolution"]
for tempo in tempos:
    plot_speedup_chart(tempo)
    mean_times_chart(tempo)
    plot_eff_chart(tempo)

    
    
            



# mean_times_chart()

# mean_times_chart("init")

# mean_times_chart("resolution")
